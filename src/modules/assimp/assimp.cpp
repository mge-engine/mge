// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/image.hpp"
#include "mge/graphics/memory_image.hpp"

#include <assimp/IOStream.hpp>  // C++ stream interface
#include <assimp/IOSystem.hpp>  // C++ IOSystem interface
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

namespace mge {

    MGE_DEFINE_TRACE(ASSIMP);

    class assimp_loader : public asset_loader
    {
    public:
        assimp_loader() = default;
        ~assimp_loader() = default;

        std::any load(const mge::asset& a) override;

        std::span<mge::asset_type> handled_types() const override;

        bool can_improve(const mge::asset&      asset,
                         const mge::asset_type& type) const override
        {
            if (type == mge::asset_type("text", "plain")) {
                return true;
            }
            return false;
        }

        mge::asset_type improve(const mge::asset&      asset,
                                const mge::asset_type& type) const override
        {
            if (type == mge::asset_type("text", "plain")) {
                if (asset.path().extension() == ".obj") {
                    return mge::asset_type("model", "obj");
                }
            }

            return asset_type::UNKNOWN;
        }
    };

    class assimp_iostream : public Assimp::IOStream
    {
    public:
        assimp_iostream(mge::input_stream_ref stream)
            : m_stream(stream)
        {}

        size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override
        {
            return m_stream->read(pvBuffer, pSize * pCount) / pSize;
        }

        size_t Write(const void* /*pvBuffer*/,
                     size_t /*pSize*/,
                     size_t /*pCount*/) override
        {
            return 0;
        }

        aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override
        {
            mge::input_stream::offset_type offset_after_seek = 0;
            switch (aiOrigin(pOrigin)) {
            case aiOrigin_SET:
                offset_after_seek =
                    m_stream->seek(pOffset, mge::input_stream::POS_BEG);
                break;
            case aiOrigin_CUR:
                offset_after_seek =
                    m_stream->seek(pOffset, mge::input_stream::POS_CUR);
                break;
            case aiOrigin_END:
                offset_after_seek =
                    m_stream->seek(pOffset, mge::input_stream::POS_END);
                break;
            default:
                MGE_THROW(mge::illegal_argument) << "Invalid seek origin";
            }

            return offset_after_seek == -1 ? aiReturn_FAILURE
                                           : aiReturn_SUCCESS;
        }

        size_t Tell() const override
        {
            return m_stream->tell();
        }

        size_t FileSize() const override
        {
            auto pos = m_stream->position();
            if (pos == -1) {
                return 0; // Unable to determine size
            }
            auto offset = m_stream->seek(0, mge::input_stream::POS_END);
            if (offset == -1) {
                return 0; // Unable to determine size
            }
            // Restore the original position
            m_stream->seek(pos, mge::input_stream::POS_BEG);
            return offset;
        }

        void Flush() override {}

    private:
        mge::input_stream_ref m_stream;
    };

    class assimp_iosystem : public Assimp::IOSystem
    {
    public:
        assimp_iosystem(mge::input_stream_ref stream)
            : m_stream(stream)
        {
            m_assimp_stream = std::make_shared<assimp_iostream>(stream);
        }

        ~assimp_iosystem() = default;

        bool Exists(const char* pFile) const override
        {
            return true;
        }

        char getOsSeparator() const override
        {
            return '/';
        }

        Assimp::IOStream* Open(const char* pFile, const char* pMode) override
        {
            return m_assimp_stream.get();
        }

        void Close(Assimp::IOStream* pFile) override
        {
            // No need to close the stream, it is managed by the asset system
        }

    private:
        mge::input_stream_ref            m_stream;
        std::shared_ptr<assimp_iostream> m_assimp_stream;
    };

    std::any assimp_loader::load(const mge::asset& a)
    {
        MGE_DEBUG_TRACE(ASSIMP) << "Loading asset: " << a.path();

        Assimp::Importer      importer;
        mge::input_stream_ref stream = a.data();
        assimp_iosystem       iosystem(stream);

        importer.SetIOHandler(&iosystem);
        return std::any();
    }

    std::span<mge::asset_type> assimp_loader::handled_types() const
    {
        using namespace mge::literals;
        static asset_type supported[] = {"model/obj"_at};
        return supported;
    }

    MGE_REGISTER_IMPLEMENTATION(assimp_loader, mge::asset_loader, obj);
} // namespace mge