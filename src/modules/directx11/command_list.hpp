#pragma once
#include "mge/graphics/command_list.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

namespace mge::dx11 {

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        ~command_list();

        void clear(const rgba_color& c) override;
        void execute() override;

    private:
        render_context&                     m_dx11_context;
        com_unique_ptr<ID3D11DeviceContext> m_deferred_context;
        com_unique_ptr<ID3D11CommandList>   m_command_list;
    };

} // namespace mge::dx11