#include "dx11_test.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/window.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/command_list.hpp"

namespace dx11 {

class test_command_list : public dx11test_with_window
{};

TEST_F(test_command_list, create)
{
    auto l = window->render_context().create_command_list();
    EXPECT_TRUE(l->native());
    EXPECT_FALSE(l->finished());
}

}