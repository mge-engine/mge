#include "command_list.hpp"
#include "render_context.hpp"
#include "error.hpp"
#include "mge/graphics/render_context.hpp"

namespace dx12 {
    command_list::command_list(render_context& context)
        :mge::command_list(context)
    {
        ID3D12CommandList *cl = nullptr;
        auto rc = dx12_device(context)->CreateCommandList(0, 
                                                          D3D12_COMMAND_LIST_TYPE_DIRECT, 
                                                          dx12_command_allocator(context),
                                                          nullptr /* pipeline state */, 
                                                          IID_PPV_ARGS(&cl));
        CHECK_HRESULT(rc , ID3D12Device, CreateCommandList);
    }

    command_list::~command_list()
    {}

    void
    command_list::on_finish()
    {}

    void
    command_list::clear(const mge::rgba_color& c)
    {}

}