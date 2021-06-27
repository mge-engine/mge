#pragma once
#include "mge/core/semantic_version.hpp"
#include "mge/graphics/dllexport.hpp"

#include <iosfwd>
#include <string>
#include <string_view>

namespace mge {

    /**
     * @brief A shader language.
     *
     * A shader language consists of a name and a version.
     *
     */
    class MGEGRAPHICS_EXPORT shader_language
    {
    public:
        /**
         * @brief Create a shader language object.
         *
         * @param name      shader language name
         * @param version   shader language version
         */
        shader_language(std::string_view name, const semantic_version &version);

        ~shader_language();

        /**
         * @brief Get shader language name.
         *
         * @return shader language name
         */
        std::string_view name() const;

        /**
         * @brief Get shader language version.
         *
         * @return shader language version
         */
        const semantic_version &version() const;

        /**
         * @brief Comparison operator.
         *
         * @param l compared language
         * @return @c true if equal
         */
        bool operator==(const shader_language &l) const;

        /**
         * @brief Return this language is compatible with the other
         *
         * @param other
         * @return @c true if compatible, i.e. same name, and this has less or
         * equal version.
         */
        bool compatible(const shader_language &other) const;

    private:
        std::string      m_name;
        semantic_version m_version;
    };

    MGEGRAPHICS_EXPORT std::ostream &operator<<(std::ostream &         os,
                                                const shader_language &l);
} // namespace mge