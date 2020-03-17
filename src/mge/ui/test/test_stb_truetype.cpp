// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/file.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <string_view>

#include <ft2build.h>
#include FT_FREETYPE_H

#if 0
static void check_buffer(const mge::buffer& fontdata)
{
    stbtt_fontinfo fontinfo = {};
    auto rc = stbtt_InitFont(&fontinfo,
                             fontdata.data(),
                             stbtt_GetFontOffsetForIndex(fontdata.data(), 0));
    if (!rc) {
        std::cout << "Cannot init font" << std::endl;
    }
    int length = 0;
    const char *name = stbtt_GetFontNameString(&fontinfo,
                                               &length,
                                               STBTT_PLATFORM_ID_MICROSOFT,
                                               1,
                                               0,
                                               0x0409);
    if (!name) {
        std::cout << "No font name found" << std::endl;
    } else {
        std::string_view nv(name, length);
        std::cout << "Font name: " << nv << std::endl;
    }
}

TEST(stb_truetype, metadata)
{
    mge::file fontdir("c:/windows/fonts");
    auto files = fontdir.list();
    decltype(files) fontfiles;
    for(const auto& f : files) {
        if (f.path().extension() == mge::path(".ttf")) {
            fontfiles.push_back(f);
        }
    }

    for(const auto f : fontfiles) {
        std::cout << "Checking " << f << std::endl;
        check_buffer(f.data());
    }
}
#endif

static void ft_check_buffer(const mge::buffer& data)
{
    static FT_Library library;
    static bool initialized;
    if (!initialized) {
        FT_Error err = FT_Init_FreeType(&library);
        if (err) {
            MGE_THROW(mge::runtime_exception) << "ft error " << err;
        }
        initialized = true;
    }

    FT_Face face;
    FT_Error err = FT_New_Memory_Face(library,
                                      data.data(),
                                      data.size(),
                                      0,
                                      &face);
    if (err) {
        MGE_THROW(mge::runtime_exception) << "Cannot initialize face: " << err;
    }
    if (face->family_name) {
        std::cout << "Family name: " << face->family_name << std::endl;
    } else {
        std::cout << "No family name" << std::endl;
    }
    FT_Done_Face(face);
}

TEST(freetype, metadata)
{
    mge::file fontdir("c:/windows/fonts");
    auto files = fontdir.list();
    decltype(files) fontfiles;
    for(const auto& f : files) {
        if (f.path().extension() == mge::path(".ttf")) {
            fontfiles.push_back(f);
        }
    }

    for(const auto f : fontfiles) {
        std::cout << "Checking " << f << std::endl;
        ft_check_buffer(f.data());
    }
}
