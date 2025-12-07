// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

namespace mge::glfw {

    /**
     * @brief Singleton class managing GLFW initialization and termination.
     */
    class glfw_instance
    {
    public:
        /**
         * @brief Get the singleton instance.
         * @return Reference to the singleton instance.
         */
        static glfw_instance& instance();

        // Delete copy and move constructors/assignment operators
        glfw_instance(const glfw_instance&) = delete;
        glfw_instance& operator=(const glfw_instance&) = delete;
        glfw_instance(glfw_instance&&) = delete;
        glfw_instance& operator=(glfw_instance&&) = delete;

        /**
         * @brief Destructor, terminates GLFW.
         */
        ~glfw_instance();

    private:
        /**
         * @brief Private constructor, initializes GLFW.
         */
        glfw_instance();
    };

} // namespace mge::glfw
