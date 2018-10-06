// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"

namespace mge {
    configuration::configuration()
    {}

    configuration::configuration(const configuration&)
    {}

    configuration::configuration(configuration&&)
    {}

    configuration&
    configuration::operator =(const configuration&)
    {
        return *this;
    }

    configuration&
    configuration::operator =(configuration&&)
    {
        return *this;
    }

    configuration::~configuration()
    {}
}
