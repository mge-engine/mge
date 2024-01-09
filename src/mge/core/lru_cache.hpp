#pragma once

#include <boost/bimap/bimap.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <optional>

namespace mge {

    template <typename K, typename V> class lru_cache
    {
    public:
        typedef boost::bimaps::bimap<boost::bimaps::unordered_set_of<K>,
                                     boost::bimaps::list_of<V>>
            cache_type;

        lru_cache(size_t max_size)
            : m_max_size(max_size)
        {}

        void insert(const K& k, const V& v)
        {
            auto it = m_cache.left.find(k);
            if (it != m_cache.left.end()) {
                m_cache.right.erase(m_cache.project_right(it));
            }
            m_cache.insert({k, v});
            if (m_cache.size() > m_max_size) {
                m_cache.right.erase(m_cache.right.begin());
            }
        }

        std::optional<V> get(const K& k)
        {
            auto it = m_cache.left.find(k);
            if (it == m_cache.left.end()) {
                return std::optional<V>();
            } else {
                m_cache.right.relocate(m_cache.right.end(),
                                       m_cache.project_right(it));
                return it->second;
            }
        }

    private:
        cache_type m_cache;
        size_t     m_max_size;
    };

} // namespace mge