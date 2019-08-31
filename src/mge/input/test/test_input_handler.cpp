#include "test/googletest.hpp"
#include "mge/input/input_handler.hpp"

namespace mge {
class test_input_handler : public input_handler
{
public:
    test_input_handler()
    {}

    void on_mouse_action(int button, mouse_action action, int x, int y)
    {
        input_handler::on_mouse_action(button, action, x, y);
    }

    void on_key_action(key k, key_action action)
    {
        input_handler::on_key_action(k, action);
    }


    void on_mouse_move(int x, int y)
    {
        input_handler::on_mouse_move(x, y);
    }

    void on_character(unsigned int ch)
    {
        input_handler::on_character(ch);
    }

    void add_modifier(modifier_value m)
    {
        input_handler::add_modifier(m);
    }

    void remove_modifier(modifier_value m)
    {
        input_handler::remove_modifier(m);
    }

};

TEST(input_handler, on_key_action) {
    bool key_action_handler_called = false;
    input_handler::key_action_handler h = [&](key k, key_action action, modifier m) {
        EXPECT_TRUE(m.empty());
        EXPECT_EQ(key::ZERO, k);
        EXPECT_EQ(key_action::PRESS, action);
        key_action_handler_called = true;
    };
    test_input_handler ih;
    ih.set_key_action_handler(h);
    ih.on_key_action(key::ZERO, key_action::PRESS);
    EXPECT_TRUE(key_action_handler_called);
}

}