#include "dx12.hpp"
#include "common.hpp"
#include "mge/graphics/command_list.hpp"


namespace dx12 {
    class render_context;

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        ~command_list();
        void clear(const mge::rgba_color& c) override;
    protected:
        void on_finish() override;
    };
}