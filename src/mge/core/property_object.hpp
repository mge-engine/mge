#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/properties.hpp"

namespace mge {
    class MGE_CORE_EXPORT property_object
            : mge::noncopyable
    {
    public:
        property_object() = default;
        ~property_object() = default;

        std::string get_property(const std::string& key) const;
        std::string get_property(const std::string& key,
                                 const std::string& default_value) const;

    private:
        properties m_properties;
    };
}

//namespace moge {

//    /**
//     * Object that can hold key/value properties.
//     */
//    class MOGE_CORE_EXPORT property_object :
//            moge::noncopyable
//    {
//    public:
//        property_object();
//        ~property_object();
//        void set_property(const std::string& key,
//                          const std::string& value);
//        std::string get_property(const std::string& key) const;
//        std::string get_property(const std::string& key,
//                                 const std::string& defaultvalue) const;
//        bool has_property(const std::string& key) const;
//        void erase_property(const std::string& key);
//        void clear_properties();

//        template <typename T>
//        T get_property(const std::string& key) const
//        {
//            return m_properties.get<T>(key);
//        }

//        template <typename T>
//        T get_property(const std::string& key, const std::string& defaultvalue) const
//        {
//            return m_properties.get<T>(key, defaultvalue);
//        }
//    private:
//        properties m_properties;
//    };
//}
