// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"
#include "mge/core/system_error.hpp"


TEST(system_error, construct)
{
    mge::system_error err;
    err << mge::system_error::error();
    mge::test_stream_any_output(err);

}

TEST(system_error, construct_from_boost_error_code)
{
    mge::system_error err;
    boost::system::error_code ec;
    err << mge::system_error::error(ec);
    mge::test_stream_any_output(err);
}
