#include "asset_test.hpp"

namespace mge {
    void asset_test::SetUp() {}

    void asset_test::TearDown() {}

    MGE_REGISTER_IMPLEMENTATION(asset_test_application,
                                mge::application,
                                asset_test);

} // namespace mge

APPLICATIONTEST_MAIN(asset_test);