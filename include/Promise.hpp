#pragma once
#include <thread>
#include <future>
#include <optional>
#include <exception>
#include <mutex>

namespace nitron
{

template <typename ResultType>
class Promise
{
public:
    enum struct Status { PENDING, SUCCEED, FAILED, USED, ABORTED };

    struct promise_aborted : std::runtime_error
    {
        promise_aborted();
    };
    struct promise_used : std::runtime_error
    {
        promise_used();
    };

    template <typename Functor, typename... Args>
    requires std::invocable<Functor, Args...>
    Promise(Functor functor, Args... args);

    Status getStatus();
    Status getStatus() const;
    bool pending();
    bool pending() const;
    bool succeed();
    bool succeed() const;
    bool failed();
    bool failed() const;
    bool used();
    bool used() const;
    bool aborted();
    bool aborted() const;

    void abort();
    ResultType get();

    /// @brief this method is incomplete
    template <typename NextType, typename Functor>
    requires std::invocable<Functor, ResultType>
    Promise<NextType> then(Functor functor);
    
    /// @brief this method is incomplete
    template <typename NextType, typename Functor>
    requires std::invocable<Functor, std::exception_ptr>
    void except(Functor functor);

private:
    void block();
    std::recursive_mutex mtx;
    std::future<ResultType> future;
    std::optional<ResultType> result;
    std::optional<std::exception_ptr> exception;
    Status status = Status::PENDING;
};

} // namespace nitron

#include "Promise.tpp"