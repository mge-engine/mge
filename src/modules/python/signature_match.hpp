// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/signature.hpp"
#include "value_classification.hpp"
namespace mge::python {

    namespace {
        template <typename Iterator> struct return_signature_member
        {
            const mge::script::signature* operator()(const Iterator& it) const
            {
                return it->signature;
            }
        };
    } // namespace

    template <typename Iterator,
              typename ClassificationList,
              typename ExtractSignature = return_signature_member<Iterator>>
    Iterator best_match(const Iterator            begin,
                        const Iterator            end,
                        const ClassificationList& value_classes,
                        const ExtractSignature& extractor = ExtractSignature())
    {
        Iterator result = end;
        size_t   best_match_count = 0;

        const size_t value_classes_size = value_classes.size();
        for (Iterator it = begin; it != end; ++it) {
            size_t exact_match_count = 0;
            bool   match_failed = false;
            auto   current_signature = extractor(it);
            if (current_signature->size() == value_classes_size) {
                for (size_t i = 0; i < value_classes_size && !match_failed;
                     ++i) {
                    auto match =
                        value_classes[i].match(current_signature->at(i));
                    switch (match) {
                    case value_classification::NO_MATCH:
                        match_failed = true;
                        continue;
                    case value_classification::MATCH_EXACT:
                        ++exact_match_count;
                        break;
                    default:
                        break;
                    }
                }

                if (exact_match_count > best_match_count) {
                    result = it;
                    best_match_count = exact_match_count;
                } else if (result == end) {
                    result = it;
                }
            }
        }

        return result;
    }

} // namespace mge::python