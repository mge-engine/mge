#include "mge/asset/asset_type.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/noncopyable.hpp"
namespace mge {

    MGE_DECLARE_REF(asset_handle);

    class MGEASSET_EXPORT asset_handle : public noncopyable
    {
    public:
        asset_handle();
        virtual ~asset_handle() = default;
        virtual asset_type type() const = 0;
    };
} // namespace mge