#pragma once
#include "Promise.hpp"

namespace nitron
{

template <typename ResultType>
template <typename Functor, typename... Args>
requires std::invocable<Functor, Args...>
Promise<ResultType>::Promise(Functor functor, Args... args) :
    future(std::async(std::launch::async, functor, args...))
{}

template <typename ResultType>
Promise<ResultType>::Status Promise<ResultType>::getStatus()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    if (status == Status::PENDING && future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        blockAndEvaluate();
    }
    return status;
}

template <typename ResultType>
Promise<ResultType>::Status Promise<ResultType>::getStatus() const
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return status;
}

template <typename ResultType>
ResultType Promise<ResultType>::get()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    blockAndEvaluate();
    if (status == Status::FAILED)
    {
        std::rethrow_exception(exception.value());
    }
    return result.value();
}

template <typename ResultType>
bool Promise<ResultType>::pending()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return getStatus() == Status::PENDING;
}

template <typename ResultType>
bool Promise<ResultType>::pending() const
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return status == Status::PENDING;
}

template <typename ResultType>
bool Promise<ResultType>::succeed()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return getStatus() == Status::SUCCEED;
}

template <typename ResultType>
bool Promise<ResultType>::succeed() const
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return status == Status::SUCCEED;
}

template <typename ResultType>
bool Promise<ResultType>::failed()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return getStatus() == Status::FAILED;
}

template <typename ResultType>
bool Promise<ResultType>::failed() const
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return status == Status::FAILED;
}

template <typename ResultType>
template <typename NextType, typename Functor>
requires std::invocable<Functor, ResultType>
Promise<NextType> Promise<ResultType>::then(Functor functor)
{
    return Promise<NextType>(
        [future, functor = std::move(functor)]() mutable -> NextType {
            return functor(future.get());
        }
    );
}

template <typename ResultType>
template <typename NextType, typename Functor>
requires std::invocable<Functor, std::exception_ptr>
Promise<NextType> Promise<ResultType>::except(Functor functor)
{
    return Promise<NextType>(
        [future, functor = std::move(functor)]() mutable -> NextType {
            try
            {
                future.get();
            }
            catch (...)
            {
                functor(std::current_exception());
            }
            return {};
        }
    );
}

template <typename ResultType>
void Promise<ResultType>::blockAndEvaluate()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    if (status != Status::PENDING)
    {
        return;
    }
    try
    {
        result = future.get();
        status = Status::SUCCEED;
    }
    catch(std::exception const& e)
    {
        exception = std::make_exception_ptr(e);
        status = Status::FAILED;
    }
}

} // namespace nitron