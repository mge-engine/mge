#include "dx12.hpp"
#include "common.hpp"
#include "mge/graphics/command_list.hpp"
#include "win32/com_unique_ptr.hpp"

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
    private:
        COM_UNIQUE_PTR(ID3D12CommandList) m_command_list;
    };
}