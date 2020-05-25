#include "mge/asset/asset_locator.hpp"
#include "mge/asset/asset_type.hpp"

using namespace mge::string_literals;
using namespace std::string_literals;

namespace opengl {

    class asset_locator : public mge::asset_locator
    {
    public:
        asset_locator()          = default;
        virtual ~asset_locator() = default;
        virtual mge::path locate(std::string_view       name,
                                 const mge::asset_type &type) override
        {
            mge::path asset_path(name.begin(), name.end());
            if (type.type() == "vertex-shader") {
                asset_path.append(".vs");
            } else if (type.type() == "fragment-shader") {
                asset_path.append(".fs");
            } else if (type.type() == "compute-shader") {
                asset_path.append(".cs");
            } else if (type.type() == "shader") {
                // do nothing
            } else {
                return mge::path();
            }

            if (type.subtype() == "glsl") {
                asset_path.append("glsl");
            } else if (type.subtype() == "spirv") {
                asset_path.append("spirv");
            } else {
                return mge::path();
            }

            if (mge::asset::exists(asset_path)) {
                return asset_path;
            }

            return mge::path();
        }
    };

    MGE_REGISTER_IMPLEMENTATION(asset_locator, mge::asset_locator);

} // namespace opengl