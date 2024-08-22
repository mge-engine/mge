#include "mge/core/exception.hpp"
#include "test/googletest.hpp"

#pragma once

#define EXPECT_THROW_MGE_EXCEPTION(statement, message)                         \
    try {                                                                      \
        statement;                                                             \
        FAIL() << "Expected mge::exception thrown";                            \
    } catch (const mge::exception& e) {                                        \
        EXPECT_STREQ(message, e.what());                                       \
    } catch (const std::exception& e) {                                        \
        FAIL() << "Expected exception of type mge::exception but got "         \
               << e.what();                                                    \
    } catch (...) {                                                            \
        FAIL() << "Expected exception of type mge::exception but got unknown " \
                  "exception";                                                 \
    }