// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/module.hpp"
#include "mge/reflection/module_details.hpp"
#include "test/googletest.hpp"

namespace mge::reflection {

    TEST(module_details, root)
    {
        auto root_details = module_details::root();
        EXPECT_EQ(root_details->name(), "");
        EXPECT_TRUE(root_details->is_root());

        auto root = module();
        EXPECT_TRUE(root.is_root());
    }

    TEST(module, construct)
    {
        mge::reflection::module mod;
        EXPECT_TRUE(mod.is_root());
        mge::reflection::module mod2("::");
        EXPECT_TRUE(mod2.is_root());

        mge::reflection::module mod3("::test");
        EXPECT_EQ(mod3.name(), "test");
        EXPECT_EQ(mod3.full_name(), "::test");
        EXPECT_FALSE(mod3.is_root());

        mge::reflection::module mod4("::test::submodule");
        EXPECT_EQ(mod4.name(), "submodule");
        EXPECT_EQ(mod4.full_name(), "::test::submodule");
        EXPECT_FALSE(mod4.is_root());

        auto mod4parent = mod4.parent();
        EXPECT_EQ(mod4parent.name(), "test");
        EXPECT_EQ(mod4parent.full_name(), "::test");
        EXPECT_FALSE(mod4parent.is_root());
    }

    TEST(module, add_child_module)
    {
        mge::reflection::module parent("::add_test_parent");

        // Create a standalone child module details manually
        auto child_details =
            std::make_shared<module_details>(nullptr, "add_test_child");

        EXPECT_EQ(parent.full_name(), "::add_test_parent");
        EXPECT_FALSE(child_details->parent());

        // Add child to parent through module_details
        parent.details()->add(child_details);

        // Verify child is now under parent
        EXPECT_EQ(child_details->parent(), parent.details());
        EXPECT_EQ(child_details->full_name(),
                  "::add_test_parent::add_test_child");

        // Verify parent's children list
        const auto& children = parent.details()->children();
        bool        found = false;
        for (const auto& ch : children) {
            if (ch->name() == "add_test_child") {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);
    }

    TEST(module, cannot_add_root_module_as_child)
    {
        mge::reflection::module parent("::neg_test_parent1");
        auto                    root = module_details::root();

        EXPECT_THROW(parent.details()->add(root), mge::illegal_state);
    }

    TEST(module, add_same_child_twice_is_idempotent)
    {
        mge::reflection::module parent("::neg_test_parent2");
        auto                    child_details =
            std::make_shared<module_details>(nullptr, "neg_test_child2");

        parent.details()->add(child_details);
        size_t initial_child_count = parent.details()->children().size();

        // Adding the same child again should not add it twice
        parent.details()->add(child_details);

        EXPECT_EQ(parent.details()->children().size(), initial_child_count);
        EXPECT_EQ(child_details->parent(), parent.details());
    }

    TEST(module, cannot_add_child_with_existing_parent)
    {
        mge::reflection::module parent1("::neg_test_parent3");
        mge::reflection::module parent2("::neg_test_parent4");
        auto                    child_details =
            std::make_shared<module_details>(nullptr, "neg_test_child3");

        parent1.details()->add(child_details);

        // Attempting to add to a different parent should throw
        EXPECT_THROW(parent2.details()->add(child_details),
                     mge::illegal_state);

        // Child should still be under parent1
        EXPECT_EQ(child_details->parent(), parent1.details());
    }

} // namespace mge::reflection