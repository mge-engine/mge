// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/window.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/core/clear_function.hpp"
#include "mge/core/log.hpp"
#include "mge/application/application.hpp"
#include "mge/core/clock.hpp"

MGE_DEFINE_LOG(WINDOW);


namespace mge {
    window::window(const rectangle &rect,
                   const window_options &options)
        :m_rect(rect)
        ,m_options(options)
        ,m_visible(false)
        ,m_quit_listener(0)
    {}

    window::~window()
    {
        uninstall_display_thread();
        m_render_context.reset();
    }

    const render_context&
    window::render_context() const
    {
        return *m_render_context;
    }

    render_context&
    window::render_context()
    {
        return *m_render_context;
    }

    void
    window::assign_thread()
    {
        render_context().assign_thread();
    }

    void
    window::show()
    {
        if(!m_visible) {
            on_show();
            m_visible = true;

            install_display_thread();

        }
    }

    void
    window::install_display_thread()
    {
        MGE_DEBUG_LOG(WINDOW) << "Install window display thread";
        m_display_thread = std::make_shared<display_thread>(this);
        m_display_thread->start();

        m_quit_listener = application::instance().add_quit_listener([&]{
            m_display_thread->set_quit();
        });

        application::instance().add_application_thread(m_display_thread.get());
    }

    void
    window::uninstall_display_thread()
    {
        if(m_display_thread) {
            MGE_DEBUG_LOG(WINDOW) << "Uninstall window display thread";
            m_display_thread->set_quit();
            application::instance().remove_application_thread(m_display_thread.get());
            if(m_display_thread->joinable()) {
                m_display_thread->join();
            }
            m_display_thread.reset();
        }
    }

    void
    window::hide()
    {
        if(m_visible) {
            on_hide();
            m_visible = false;
        }
    }

    bool
    window::visible() const
    {
        return m_visible;
    }

    void
    window::set_redraw_listener(const window::redraw_listener& listener)
    {
        m_redraw_listener = listener;
    }

    void
    window::clear_redraw_listener()
    {
        clear_function(m_redraw_listener);
    }

    void
    window::set_close_listener(const window::close_listener& listener)
    {
        m_close_listener = listener;
    }

    void
    window::clear_close_listener()
    {
        clear_function(m_close_listener);
    }

    void
    window::set_closing_listener(const window::closing_listener& listener)
    {
        m_closing_listener = listener;
    }

    void
    window::clear_closing_listener()
    {
        clear_function(m_closing_listener);
    }

    void
    window::refresh(mge::thread *display_thread,
                    float interpolation)
    {
        if(m_redraw_listener) {
            try {
                redraw_context context {
                    this,
                    display_thread,
                    render_context(),
                    interpolation
                };
                m_redraw_listener(context);
            } catch(const mge::exception& ex) {
                MGE_DEBUG_LOG(WINDOW) << "Exception in redraw: " << ex.details();
                m_redraw_listener = nullptr;
            }
        }
    }

    void
    window::close()
    {
        request_close();
    }

    void
    window::on_close()
    {
        if(m_closing_listener) {
            if(!m_closing_listener()) {
                return; // close rejected
            }
        }
        if(m_close_listener) {
            m_close_listener();
        }
        uninstall_display_thread();
    }

    window::display_thread::display_thread(window *w)
        :m_window(w)
        ,m_quit(false)
    {}

    window::display_thread::~display_thread()
    {}

    void
    window::display_thread::set_quit()
    {
        m_quit = true;
    }

    void
    window::display_thread::run()
    {
        m_window->assign_thread();
        application *app = nullptr;
        try {
            app = &application::instance();
        } catch(const mge::exception& ex) {
            MGE_ERROR_LOG(WINDOW) << "Acquiring application object failed: " << ex;
            mge::rethrow();
        }
        MGE_DEBUG_LOG(WINDOW) << "Display loop started";
        auto next_game_tick = clock::now();
        auto update_rate = app->update_rate();
        auto skip_ticks = 1000000000 / update_rate;
        auto max_frame_skip = app->max_frame_skip();
        MGE_DEBUG_LOG(WINDOW) << "Update rate: " << update_rate << " Hz";
        MGE_DEBUG_LOG(WINDOW) << "Skip ticks : " << skip_ticks << " ns";
        MGE_DEBUG_LOG(WINDOW) << "Maximum skipped frames: " << max_frame_skip;
        while(!m_quit) {
            next_game_tick = clock::now();
            update_rate = app->update_rate();
            skip_ticks = 1000000000 / update_rate;
            max_frame_skip = app->max_frame_skip();
            decltype(max_frame_skip) loops = 0;
            for(;;) {
                auto update_ts = clock::now();
                if(!(update_ts > next_game_tick && loops < max_frame_skip)) {
                    break;
                }
                exec_work_item();
                next_game_tick += skip_ticks;
                ++loops;
            }
            auto display_ts = clock::now();
            float interpolation = 0.0f;
            if(display_ts < next_game_tick) {
                interpolation = 1.0f - (float) (next_game_tick - display_ts)
                        / (float) skip_ticks;
            } else {
                interpolation = (float) (next_game_tick + skip_ticks - display_ts)
                        / (float) skip_ticks;
            }
            try {
                m_window->refresh(this, interpolation);
            } catch(const mge::exception& ex) {
                MGE_ERROR_LOG(WINDOW) << "Exception in window refresh: " << ex;
                m_quit = true;
            } catch(const std::exception& ex) {
                MGE_ERROR_LOG(WINDOW) << "Exception in window refresh: " << ex.what();
                m_quit = true;
            } catch(...) {
                MGE_ERROR_LOG(WINDOW) << "Error in window refresh";
                m_quit = true;
            }
        }
        MGE_DEBUG_LOG(WINDOW) << "Display loop finished";
    }

    void
    window::display_thread::exec_work_item()
    {
        auto task = m_window->m_tasks.pop_front();
        if(task) {
            task->run();
        }
    }

    void
    window::await(const std::function<void ()> &f)
    {
        if(!m_display_thread || !m_display_thread->joinable()) {
            f();
        } else if(mge::this_thread::get_id() == m_display_thread->get_id()) {
            f();
        } else {
            task t(f);
            std::shared_ptr<task> tref(&t, [](auto p){});
            m_tasks.push_back(tref);
            t.wait();
        }
    }




}
