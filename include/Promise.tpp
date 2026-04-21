#pragma once
#include "Promise.hpp"

namespace nitron
{

template <typename ResultType>
Promise<ResultType>::promise_aborted::promise_aborted() :
    std::runtime_error("could not execute nitron::Promise<ResultType>::get(), promise was aborted.")
{}

template <typename ResultType>
Promise<ResultType>::promise_used::promise_used() :
    std::runtime_error("could not execute nitron::Promise<ResultType>::get(), promise was already used.")
{}

template <typename ResultType>
template <typename Functor, typename... Args>
requires std::invocable<Functor, Args...>
Promise<ResultType>::Promise(Functor functor, Args... args) :
    future(std::async(std::launch::async, functor, args...))
{}

template <typename ResultType>
Promise<ResultType>::Status Promise<ResultType>::getStatus() const
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return status;
}

template <typename ResultType>
void Promise<ResultType>::block()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    if (status != Status::PENDING)
        return;
    try
    {
        result = future.get();
        status = Status::SUCCEED;
    }
    catch(const std::exception& e)
    {
        exception = std::make_exception_ptr(e);
        status = Status::FAILED;
    }
}

template <typename ResultType>
Promise<ResultType>::Status Promise<ResultType>::getStatus()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    if (status != Status::PENDING || future.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
        return status;
    block();
    return status;
}

template <typename ResultType>
ResultType Promise<ResultType>::get()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    Status current = getStatus();
    switch (current)
    {
    case Status::USED:
        throw promise_used();
        break;
    case Status::ABORTED:
        throw promise_aborted();
        break;
    case Status::PENDING:
        block();
        break;
    default:
        break;
    }
    if (status == Status::FAILED)
    {
        status = Status::USED;
        std::rethrow_exception(exception.value());
    }
    status = Status::USED;
    return std::move(result.value());
}

template <typename ResultType>
void Promise<ResultType>::abort()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    if(getStatus() == Status::PENDING)
    status = Status::ABORTED;
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
bool Promise<ResultType>::used()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return getStatus() == Status::USED;
}

template <typename ResultType>
bool Promise<ResultType>::used() const
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return status == Status::USED;
}

template <typename ResultType>
bool Promise<ResultType>::aborted()
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return getStatus() == Status::ABORTED;
}

template <typename ResultType>
bool Promise<ResultType>::aborted() const
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return status == Status::ABORTED;
}

template <typename ResultType>
template <typename NextType, typename Functor>
requires std::invocable<Functor, ResultType>
Promise<NextType> Promise<ResultType>::then(Functor functor)
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    return Promise([](){ return NextType(); });
}

template <typename ResultType>
template <typename NextType, typename Functor>
requires std::invocable<Functor, std::exception_ptr>
void Promise<ResultType>::except(Functor functor)
{
    std::lock_guard<std::recursive_mutex> lock (mtx);
    functor(exception.value());
}


} // namespace nitron
