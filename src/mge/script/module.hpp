// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>
namespace mge::script {

    class MGESCRIPT_EXPORT module
    {
    public:
        /**
         * Root module.
         */
        module();

        module(const std::string& name);

        module(const module& other) :m_data(other.m_data) {}

        module& operator=(const module& other)
        {
            m_data = other.m_data;
            return *this;
        }

        module(module&& other) :m_data(std::move(other.m_data)) {}

        module(const module_data_ref& data) :m_data(data)
        {
            if (!m_data) {
                MGE_THROW(illegal_argument) << "Module data must not be null";
            }
        }

        module& operator=(module&& other)
        {
            m_data = std::move(other.m_data);
            return *this;
        }

        /**
         * Destructor.
         */
        ~module();

        bool                is_root() const;
        const std::string&  name() const;
        mge::script::module parent() const;
        static module       root();

        template <typename T, typename... Args>
        inline module& operator()(const T& arg0, const Args&... args)
        {
            add(arg0);
            return *this;
        }

        inline module& operator()() { return *this; }

    private:
        void add(const type& t);
        void add(const function& f);

        module_data_ref m_data;
    };

} // namespace mge::script