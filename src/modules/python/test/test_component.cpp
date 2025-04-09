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

class App1 (mge.application):
    
    def __init__(self):
        super().__init__()
    
    def setup(self):
        pass

    def async_setup(self):
        pass

    def  teardown(self):
        pass

    def run(self):
        pass

mge.component.register("mge::application", "App1", App1)
    )raw");
        std::set<std::string, std::less<>> names;

        mge::component<mge::application>::implementations(
            [&](std::string_view n) { names.insert(std::string(n)); });

        EXPECT_TRUE(names.contains("App1"));
    }

    TEST_F(test_component, create_component_fails_due_to_base_check)
    {
        context->eval(R"raw(
import mge

class App2 (mge.application):
    
    def __init__(self):
        super().__init__()
    
    def setup(self):
        pass

    def async_setup(self):
        pass

    def  teardown(self):
        pass

    def run(self):
        pass

mge.component.register("mge::application", "App2", App2)
    )raw");
        try {
            auto target = mge::application::create("App2");
            EXPECT_TRUE(false) << "Should not be able to create App2 component";
        } catch (const mge::exception e) {
            EXPECT_TRUE(std::string(e.what()).find(
                            "Can only have one application instance") !=
                        std::string::npos)
                << "Exception message does not contain expected text: "
                << e.what();
        }
    }

} // namespace mge::python
