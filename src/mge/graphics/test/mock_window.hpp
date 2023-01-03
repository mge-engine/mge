// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/window.hpp"
#include "test/googlemock.hpp"

class MOCK_window : public mge::window
{
public:
    MOCK_window()
        : mge::window(mge::extent(800, 600),
                      mge::window_options::standard_options())
    {}

    virtual ~MOCK_window() = default;

    MOCK_METHOD(void, on_show, (), (override));
    MOCK_METHOD(void, on_hide, (), (override));

    using window::on_close;
};