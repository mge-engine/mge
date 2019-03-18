/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#pragma once

namespace mge {
    struct color_name
    {
        const char *n;
        float r;
        float g;
        float b;
    };

    /**
     * Find color by name in stored names.
     */
    const color_name * find_color(const char *name);
}

