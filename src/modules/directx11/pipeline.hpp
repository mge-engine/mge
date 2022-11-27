#include "dx11.hpp"
#include "dx11_fwd.hpp"
#include "mge/graphics/pipeline.hpp"

namespace mge::dx11 {

    class pipeline : public mge::pipeline
    {
    public:
        pipeline(render_context& context);
        virtual ~pipeline();
    };

} // namespace mge::dx11