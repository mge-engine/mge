// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_access_factory.hpp"
#include "mge/asset/asset_not_found.hpp"
#include "mge/core/file.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    class file_asset_access
        : public asset_access
    {
    public:
        file_asset_access(const file& asset_file)
            :m_file(asset_file)
            ,m_properties_file(asset_file)
        {
            auto p = asset_file.path();
            if (!p.has_filename()) {
                MGE_THROW(illegal_argument) << "Asset file name must have a file name component";
            }

            if (p.has_extension()) {
                auto ext = p.extension();
                auto ext_str = ext.string();
                ext_str += ".properties";
                p.replace_extension(path(ext_str));
            } else {
                p.replace_extension(path(".properties"));
            }
            m_properties_file = file(p);
        }

        ~file_asset_access() = default;

        size_t size() const override
        {
            return m_file.size();
        }

        input_stream_ref data() const override
        {
            return m_file.open_for_input();
        }

        asset_type type() const override
        {
            return asset_type::UNKNOWN;
        }

        bool has_properties() const override
        {
            return m_properties_file.exists();
        }

        properties_ref properties() const override
        {
            return properties_ref();
        }

    private:
        file m_file;
        file m_properties_file;
    };

    class file_asset_access_factory
        : public asset_access_factory
    {
    public:
        file_asset_access_factory()
        {}

        virtual ~file_asset_access_factory()
        {}

        void configure(const configuration& config) override
        {
            m_directory = config.value("directory");
            file dir_file(m_directory);
            if (!dir_file.is_directory()) {
                MGE_THROW(runtime_exception) << "Directory '" << m_directory << "' does not exist";
            }
        }

        asset_access_ref create_asset_access(const mge::path& p) override
        {
            auto relpath = p.lexically_relative(mountpoint());
            auto filepath = m_directory;
            filepath /= relpath;

            file asset_file(filepath);
            if (!asset_file.exists()) {
                MGE_THROW(asset_not_found) << "Asset \"" << p << "\" not found"
                    << "(file '" << filepath << "' not found)";
            }
            return std::make_shared<file_asset_access>(asset_file);
        }

        void gist(std::ostream& os) const override
        {
            os << "file " << m_directory;
        }

        mge::path m_directory;
    };

    MGE_REGISTER_IMPLEMENTATION(file_asset_access_factory,
        asset_access_factory, system, file);
}