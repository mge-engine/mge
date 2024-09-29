#include "type_matches.hpp"
#include "mge/script/type.hpp"

namespace mge::python {

    static match_type pod_type_matches(PyObject* object,
                                       const mge::script::type_identifier& id)
    {
        auto id_idx = id.type_index();
        if (id_idx == std::type_index(typeid(bool))) {
            if (PyBool_Check(object)) {
                return match_type::EXACT_MATCH;
            } else if (PyLong_Check(object)) {
                return match_type::CAST_MATCH;
            } else if (Py_IsNone(object)) {
                return match_type::CAST_MATCH;
            }
        } else if (id_idx == std::type_index(typeid(int8_t)) ||
                   id_idx == std::type_index(typeid(uint8_t)) ||
                   id_idx == std::type_index(typeid(int16_t)) ||
                   id_idx == std::type_index(typeid(uint16_t)) ||
                   id_idx == std::type_index(typeid(int32_t)) ||
                   id_idx == std::type_index(typeid(uint32_t)) ||
                   id_idx == std::type_index(typeid(int64_t)) ||
                   id_idx == std::type_index(typeid(uint64_t))) {
            if (PyLong_Check(object)) {
                return match_type::EXACT_MATCH;
            } else if (PyBool_Check(object)) {
                return match_type::CAST_MATCH;
            } else if (Py_IsNone(object)) {
                return match_type::CAST_MATCH;
            }
        } else if (id_idx == std::type_index(typeid(float)) ||
                   id_idx == std::type_index(typeid(double)) ||
                   id_idx == std::type_index(typeid(long double))) {
            if (PyFloat_Check(object)) {
                return match_type::EXACT_MATCH;
            } else if (PyLong_Check(object)) {
                return match_type::CAST_MATCH;
            } else if (Py_IsNone(object)) {
                return match_type::CAST_MATCH;
            }
        }

        return match_type::NO_MATCH;
    }

    static match_type const_ref_matches(PyObject* object,
                                        const mge::script::type_identifier& id)
    {
        mge::script::type_data_ref t = mge::script::type_data::get(id);
        if (!t) {
            return match_type::NO_MATCH;
        } else {
            return type_matches(
                object,
                t->reference_specific().referencee->identifier());
        }
    }

    match_type type_matches(PyObject*                           object,
                            const mge::script::type_identifier& id)
    {
        if (id.kind() == mge::script::type_identifier::TYPE_VOID) {
            return match_type::NO_MATCH;
        }
        if (id.kind() == mge::script::type_identifier::TYPE_POD) {
            return pod_type_matches(object, id);
        } else if (id.kind() == mge::script::type_identifier::TYPE_REFERENCE &&
                   id.is_const()) {
            return const_ref_matches(object, id);
        }

        return match_type::NO_MATCH;
    }
} // namespace mge::python