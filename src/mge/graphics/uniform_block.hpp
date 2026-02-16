// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/std140.hpp"
#include "mge/graphics/uniform_data_type.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace mge {

    /**
     * @brief A uniform block represents data of uniforms that is grouped
     * together.
     *
     * A uniform block is created from a program's uniform buffer metadata.
     * The block layout follows std140 rules. Multiple block instances can
     * be created from the same program, and one can be bound per draw call.
     */
    class MGEGRAPHICS_EXPORT uniform_block
    {
    public:
        /**
         * @brief Layout information for a single member in the block.
         */
        struct member_layout
        {
            std::string       name;       //!< member name
            uniform_data_type type;       //!< member data type
            uint32_t          array_size; //!< array element count (1 if scalar)
            size_t            offset;     //!< byte offset in the block (std140)
            size_t            size;       //!< total byte size of this member
        };

        /**
         * @brief Create a uniform block from program uniform buffer metadata.
         *
         * @param buffer_info the uniform buffer description from a linked
         *                    program
         */
        uniform_block(const program::uniform_block_metadata& buffer_info);

        ~uniform_block();

        uniform_block(const uniform_block&) = delete;
        uniform_block& operator=(const uniform_block&) = delete;
        uniform_block(uniform_block&& other) noexcept;
        uniform_block& operator=(uniform_block&& other) noexcept;

        /**
         * @brief Name of the uniform block.
         */
        const std::string& name() const noexcept
        {
            return m_name;
        }

        /**
         * @brief Total byte size of the block (std140 padded).
         */
        size_t data_size() const noexcept
        {
            return m_data_size;
        }

        /**
         * @brief Pointer to the CPU-side data buffer.
         */
        const void* data() const noexcept
        {
            return m_data;
        }

        /**
         * @brief Version number, incremented on each write.
         */
        uint64_t version() const noexcept
        {
            return m_version;
        }

        /**
         * @brief Member layouts in the block.
         */
        const std::vector<member_layout>& members() const noexcept
        {
            return m_members;
        }

        /**
         * @brief Write raw bytes to a member by name.
         *
         * @param member_name name of the member
         * @param data pointer to the source data
         * @param size size of the source data in bytes
         */
        void
        set_data(const std::string& member_name, const void* data, size_t size);

        /**
         * @brief Write a typed value to a member by name.
         *
         * @tparam T value type (must match the member's uniform_data_type)
         * @param member_name name of the member
         * @param value the value to write
         */
        template <typename T>
        void set(const std::string& member_name, const T& value)
        {
            set_data(member_name, &value, sizeof(T));
        }

        /**
         * @brief Synchronize values from global uniforms.
         *
         * For each member in this block, looks up a matching global uniform
         * by name and copies its value if the type matches. Type mismatches
         * are logged as warnings and ignored.
         */
        void sync_from_globals();

    private:
        /**
         * @brief Cached lookup info for a global uniform.
         */
        struct uniform_cache
        {
            uniform_base* uniform{nullptr}; //!< pointer to global uniform
            const void*   data{nullptr};    //!< cached data pointer
            uint64_t      last_version{0};  //!< last synced version
            size_t        data_size{0};     //!< cached data size
            bool          checked{false};   //!< whether lookup was attempted
        };

        void compute_layout(const program::uniform_block_metadata& buffer_info);
        void ensure_uniform_cache();
        void update_uniform_cache();

        std::string                m_name;
        std::vector<member_layout> m_members;
        void*                      m_data{nullptr};
        size_t                     m_data_size{0};
        uint64_t                   m_version{0};
        std::vector<uniform_cache> m_uniform_cache; //!< cached uniform lookups
        uint64_t m_cache_registry_generation{0}; //!< registry generation when
                                                 //!< cache was built
    };

} // namespace mge