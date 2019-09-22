// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log.hpp"
#include "common.hpp"
#include <iostream>

MGE_DEFINE_LOG(DX12);

namespace dx12 {

    std::ostream& operator <<(std::ostream& os, const D3D_FEATURE_LEVEL& l)
    {
        switch(l) {
#define CASE_LEVEL(L) case L: return os << #L
        CASE_LEVEL(D3D_FEATURE_LEVEL_1_0_CORE);
        CASE_LEVEL(D3D_FEATURE_LEVEL_9_1);	
        CASE_LEVEL(D3D_FEATURE_LEVEL_9_2);	
        CASE_LEVEL(D3D_FEATURE_LEVEL_9_3);	
        CASE_LEVEL(D3D_FEATURE_LEVEL_10_0);
        CASE_LEVEL(D3D_FEATURE_LEVEL_10_1);
        CASE_LEVEL(D3D_FEATURE_LEVEL_11_0);
        CASE_LEVEL(D3D_FEATURE_LEVEL_11_1);
        CASE_LEVEL(D3D_FEATURE_LEVEL_12_0);
        CASE_LEVEL(D3D_FEATURE_LEVEL_12_1);
        default:
            return os << "INVALID(" << (int)l << ")";
        }
    }

}