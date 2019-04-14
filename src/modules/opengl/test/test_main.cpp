#include "test/googletest.hpp"
#include "mge/application/application.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/component.hpp"

using mge::application;

class opengltest_application : public application
{
public:
    opengltest_application()
    {}

    ~opengltest_application()
    {}

    void start() override
    {
        int argc = this->argc();
        char **argv = (char **) this->argv();
        ::testing::InitGoogleTest(&argc, argv);
        try {
            set_return_code(RUN_ALL_TESTS());
        } catch(const std::exception& ex) {
            std::cerr << "Exception caught: " << ex.what() << std::endl;
            set_return_code(1);
        }

        if(!is_quit()) {
            set_quit();
        }
    }
};

MGE_REGISTER_IMPLEMENTATION(opengltest_application,
                            application, opengltest_application);

MGE_MAINFUNCTION


