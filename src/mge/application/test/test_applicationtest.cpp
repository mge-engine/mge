#include "test/applicationtest.hpp"

class applicationtest_application : public mge::test_application
{};

MGE_REGISTER_IMPLEMENTATION(applicationtest_application, mge::application,
                            applicationtest);

APPLICATIONTEST_MAIN(applicationtest)