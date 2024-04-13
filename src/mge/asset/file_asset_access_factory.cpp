#include "mge/asset/file_asset_access_factory.hpp"
#include "mge/asset/file_asset_access.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    MGE_REGISTER_IMPLEMENTATION(file_asset_access_factory,
                                mge::asset_access_factory,
                                file,
                                fs);

    file_asset_access_factory::file_asset_access_factory()
        : m_configured(false)
    {}

    void file_asset_access_factory::configure(const mge::properties& p)
    {
        if (p.exists("directory")) {
            m_directory = p.get<std::string>("directory");
        } else {
            MGE_THROW(bad_configuration)
                << "File asset access factory requires 'directory' property";
        }
        m_configured = true;
    }

    bool file_asset_access_factory::is_configured() const
    {
        return m_configured;
    }

    asset_access_ref file_asset_access_factory::access(const mge::path& p)
    {
        if (!m_configured) {
            MGE_THROW(illegal_state)
                << "File asset access factory is not configured";
        }
        auto      rel_path = p.lexically_relative(m_mount_point);
        mge::path file_path = m_directory / rel_path;
        return std::make_shared<file_asset_access>(p, file_path);
    }

    bool file_asset_access_factory::asset_exists(const mge::path& p)
    {
        if (!m_configured) {
            MGE_THROW(illegal_state)
                << "File asset access factory is not configured";
        }
        auto      rel_path = p.lexically_relative(m_mount_point);
        mge::path file_path = m_directory / rel_path;
        return std::filesystem::exists(file_path);
    }

    void file_asset_access_factory::gist(std::ostream& os) const
    {
        os << "{type: file, mount_point: " << m_mount_point.string()
           << ", directory: " << m_directory.string() << "}";
    }

} // namespace mge