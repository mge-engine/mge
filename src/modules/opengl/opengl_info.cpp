#include "opengl_info.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/semantic_version.hpp"
#include "mge/core/trace.hpp"
#include "opengl.hpp"
#include <iostream>

namespace mge {

    MGE_USE_TRACE(OPENGL);

    namespace opengl {

        opengl_info::opengl_info() : major_version(0), minor_version(0)
        {

            glGetIntegerv(GL_MAJOR_VERSION, &major_version);
            glGetIntegerv(GL_MINOR_VERSION, &minor_version);

            MGE_TRACE(OPENGL, INFO)
                << "OpenGL version " << major_version << "." << minor_version;

            glsl_version_str =
                (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

            auto vit = glsl_version_str.begin();
            while (vit != glsl_version_str.end() &&
                   (*vit == '.' || std::isdigit(*vit))) {
                ++vit;
            }
            std::string plain_glsl_version(glsl_version_str.begin(), vit);

            shader_language = mge::shader_language(
                "glsl", mge::semantic_version(plain_glsl_version));

            MGE_TRACE(OPENGL, INFO) << "GLSL version " << glsl_version_str;

            std::string extension_string =
                ((const char *)glGetString(GL_EXTENSIONS));

            auto it = boost::make_split_iterator(
                extension_string, boost::token_finder(boost::is_space()));

            MGE_TRACE(OPENGL, INFO) << "OpenGL extensions:";
            while (!it.eof()) {
                if (!it->empty()) {
                    MGE_TRACE(OPENGL, INFO) << *it;
                    extensions.insert(std::string(it->begin(), it->end()));
                }
                ++it;
            }
        }

        opengl_info::~opengl_info() {}

    } // namespace opengl
} // namespace mge