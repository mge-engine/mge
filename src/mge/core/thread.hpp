#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/noncopyable.hpp"
#include "mge/core/stdexceptions.hpp"

#include <chrono>
#include <functional>
#include <thread>
namespace mge {

    /** A thread.
     *
     * This class enhances @c std::thread by some convenience methods.
     * A thread cannot be copied, or created on the stack.
     */
    class MGECORE_EXPORT thread : noncopyable,
                                  public std::enable_shared_from_this<thread>
    {
    public:
        /**
         * @brief Logical thread id.
         */
        using id = std::thread::id;

        /**
         * @brief Native handle type.
         */
        using native_handle_type = std::thread::native_handle_type;

#ifdef MGE_OS_WINDOWS
        using system_id = uint32_t;
#else
#    error missing port
#endif
        /**
         * @brief Constructor.
         */
        explicit thread();

        virtual ~thread();
#if 0
        /**
         * @brief Get the current thread.
         *
         * @return current thread, or null pointer if the current thread is not
         * managed
         */
        static thread *this_thread();

        void start();

        inline void start(std::function<void()> &&f)
        {
            if (!f) {
                MGE_THROW(mge::illegal_argument)
                    << "Invalid function parameter for thread start";
            }
            m_running_thread = std::thread([fc = std::move(f), this] {
                try {
                    this->on_start();
                    fc();
                    this->on_finish();
                } catch (...) {
                    this->on_exception(std::current_exception());
                }
            });
        }

        /**
         * @brief Run thread logic. The default method does nothing.
         */
        virtual void run();

        /**
         * @brief Joins this thread.
         */
        void join();

        /**
         * @brief Detaches this thread.
         */
        void detach();

        /**
         * Returns @c true if thread can be joined. To be joinable,
         * the thread must run and be not equal to this thread (a
         * thread cannot join itself).
         * @return @c true if thread can be joined
         */
        bool joinable();

    private:
        void on_start();
        void on_finish();
        void on_exception(const std::exception_ptr &eptr);

        std::thread m_running_thread;
#endif
    };

    namespace this_thread {

        MGECORE_EXPORT mge::thread::system_id system_id();

        inline mge::thread::id get_id() { return ::std::this_thread::get_id(); }

        template <class Rep, class Period>
        inline void
        sleep_for(const std::chrono::duration<Rep, Period> &sleep_duration)
        {
            ::std::this_thread::sleep_for(sleep_duration);
        }
    } // namespace this_thread

} // namespace mge