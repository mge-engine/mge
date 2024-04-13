#include "mge/asset/file_asset_access.hpp"
#include "mge/asset/asset_type.hpp"
#include <filesystem>
#include <fstream>

namespace mge {
    file_asset_access::file_asset_access(const mge::path& asset_path,
                                         const mge::path& file_path)
        : m_asset_path(asset_path)
        , m_file_path(file_path)
    {}

    file_asset_access::~file_asset_access() {}

    size_t file_asset_access::size() const
    {
        try {
            return std::filesystem::file_size(m_file_path);
        } catch (const std::filesystem::filesystem_error& e) {
            MGE_THROW(runtime_exception)
                << "Cannot determine size of asset: " << m_asset_path.string()
                << " (" << m_file_path.string() << "): " << e.what();
        }
    }

    input_stream_ref file_asset_access::data() const
    {
        return std::make_shared<file_input_stream>(m_file_path);
    }

    asset_type file_asset_access::type() const { return asset_type::UNKNOWN; }

    bool file_asset_access::has_properties() const
    {
        if (!m_properties) {
            mge::path p = m_file_path / ".properties";
            return std::filesystem::exists(p);
        } else {
            return true;
        }
    }

    properties_ref file_asset_access::properties() const
    {
        if (!m_properties) {
            mge::path     p = m_file_path / ".properties";
            std::ifstream pinput(p.string());
            m_properties = std::make_shared<mge::properties>(pinput);
        }
        return m_properties;
    }

} // namespace mge