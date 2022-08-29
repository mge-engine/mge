#include "value_classification.hpp"
#include "mge/core/singleton.hpp"
#include "python_type.hpp"

#include <map>

namespace mge::python {

    class match_map
    {
    public:
        match_map()
        {
#define EXACT_MATCH(type, c_type)                                              \
    type##_map[std::type_index(typeid(c_type))] =                              \
        value_classification::MATCH_EXACT
#define MATCH(type, c_type)                                                    \
    type##_map[std::type_index(typeid(c_type))] = value_classification::MATCH

#define ALL_EXACT_MATCH(type, c_type)                                          \
    EXACT_MATCH(type, c_type);                                                 \
    EXACT_MATCH(type, const c_type);                                           \
    EXACT_MATCH(type, volatile c_type);                                        \
    EXACT_MATCH(type, const volatile c_type);                                  \
    EXACT_MATCH(type, const c_type&);                                          \
    EXACT_MATCH(type, const volatile c_type&)

#define ALL_MATCH(type, c_type)                                                \
    MATCH(type, c_type);                                                       \
    MATCH(type, const c_type);                                                 \
    MATCH(type, volatile c_type);                                              \
    MATCH(type, const volatile c_type);                                        \
    MATCH(type, const c_type&);                                                \
    MATCH(type, const volatile c_type&)

            ALL_EXACT_MATCH(bool, bool);
            ALL_MATCH(bool, int8_t);
            ALL_MATCH(bool, uint8_t);
            ALL_MATCH(bool, int16_t);
            ALL_MATCH(bool, uint16_t);
            ALL_MATCH(bool, int32_t);
            ALL_MATCH(bool, uint32_t);
            ALL_MATCH(bool, int64_t);
            ALL_MATCH(bool, uint64_t);

            //---
            ALL_EXACT_MATCH(long, int8_t);
            ALL_EXACT_MATCH(long, uint8_t);
            ALL_EXACT_MATCH(long, int16_t);
            ALL_EXACT_MATCH(long, uint16_t);
            ALL_EXACT_MATCH(long, int32_t);
            ALL_EXACT_MATCH(long, uint32_t);
            ALL_EXACT_MATCH(long, int64_t);
            ALL_EXACT_MATCH(long, uint64_t);
            ALL_MATCH(long, float);
            ALL_MATCH(long, double);
            //---
            ALL_EXACT_MATCH(float, int8_t);
            ALL_EXACT_MATCH(float, uint8_t);
            ALL_EXACT_MATCH(float, int16_t);
            ALL_EXACT_MATCH(float, uint16_t);
            ALL_EXACT_MATCH(float, int32_t);
            ALL_EXACT_MATCH(float, uint32_t);
            ALL_EXACT_MATCH(float, int64_t);
            ALL_EXACT_MATCH(float, uint64_t);
            ALL_EXACT_MATCH(float, float);
            ALL_EXACT_MATCH(float, double);
            //---
            EXACT_MATCH(unicode, std::string);
            EXACT_MATCH(unicode, const std::string&);
            EXACT_MATCH(unicode, const char*);
            EXACT_MATCH(unicode, std::string_view);
            EXACT_MATCH(unicode, const std::string_view&);

#undef ALL_EXACT_MATCH
#undef ALL_MATCH
#undef EXACT_MATCH
#undef MATCH
        }

        using type_map =
            std::map<std::type_index, value_classification::match_type>;

        type_map bool_map;
        type_map long_map;
        type_map float_map;
        type_map unicode_map;
    };

    static mge::singleton<match_map> match_maps;

    value_classification::value_classification()
        : m_type_class(TC_NONE)
        , m_python_type(nullptr)
    {}

    value_classification::value_classification(PyObject* obj)
        : m_type_class(TC_NONE)
        , m_python_type(nullptr)
    {
        if (obj) {
            if (Py_IsNone(obj))
                m_type_class = TC_NONE;
            else if (PyBool_Check(obj))
                m_type_class = TC_BOOL;
            else if (PyLong_Check(obj))
                m_type_class = TC_LONG;
            else if (PyFloat_Check(obj))
                m_type_class = TC_FLOAT;
            else if (PyUnicode_Check(obj))
                m_type_class = TC_UNICODE;
            else {
                m_type_class = TC_OBJECT;
                PyTypeObject* tp = Py_TYPE(obj);
                m_python_type = python_type::python_type_of(tp);
            }
        }
    }

    value_classification::match_type
    value_classification::match(const std::type_index& arg_type) const
    {
        switch (m_type_class) {
        case TC_BOOL: {
            auto it = match_maps->bool_map.find(arg_type);
            if (it != match_maps->bool_map.end()) {
                return it->second;
            }
            break;
        }
        case TC_FLOAT: {
            auto it = match_maps->float_map.find(arg_type);
            if (it != match_maps->float_map.end()) {
                return it->second;
            }
            break;
        }
        case TC_LONG: {
            auto it = match_maps->long_map.find(arg_type);
            if (it != match_maps->long_map.end()) {
                return it->second;
            }
            break;
        }
        case TC_UNICODE: {
            auto it = match_maps->unicode_map.find(arg_type);
            if (it != match_maps->unicode_map.end()) {
                return it->second;
            }
            break;
        }
        case TC_NONE:
            return NO_MATCH;
        case TC_OBJECT:
            if (m_python_type) {
                return m_python_type->type_index() == arg_type ? MATCH_EXACT
                                                               : NO_MATCH;
            } else {
                return NO_MATCH;
            }
        }
        return NO_MATCH;
    }
} // namespace mge::python
