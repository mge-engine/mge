#include "mge/application/loop_target.hpp"
#include "pythontest.hpp"

namespace mge::python {

    class test_component : public pythontest
    {
    public:
        test_component() = default;
        virtual ~test_component() = default;
    };

    TEST_F(test_component, create_component_class)
    {
        context->eval(R"raw(
import mge

class LoopTarget1 (mge.loop_target):
    
    def __init__(self):
        super().__init__()
        
    def is_quit(self):
        return False

    def update(self, cycle):
        return

    def update(self, cycle, elapsed, delta):
        return

    def present(self, cycle, peek):
        return

mge.component.register("mge::loop_target", "LoopTarget1", LoopTarget1)
    )raw");
        std::set<std::string, std::less<>> names;

        mge::component<mge::loop_target>::implementations(
            [&](std::string_view n) { names.insert(std::string(n)); });

        EXPECT_TRUE(names.contains("LoopTarget1"));
    }

    TEST_F(test_component, create_component)
    {
        context->eval(R"raw(
import mge

class LoopTarget2 (mge.loop_target):
    
    def __init__(self):
        super().__init__()
        
    def is_quit(self):
        return False

    def update(self, cycle):
        return

    def update(self, cycle, elapsed, delta):
        return

    def present(self, cycle, peek):
        return

mge.component.register("mge::loop_target", "LoopTarget2", LoopTarget2)
    )raw");

        auto target =
            mge::loop_target::create("mge::loop_target", "LoopTarget2");
    }

} // namespace mge::python
