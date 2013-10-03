#ifndef PRIMITIVE_HH_INCLUDED
#define PRIMITIVE_HH_INCLUDED

#include <graphics_constants.hh>
#include <maths.hh>
#include <vector3.hh>

namespace trillek {

    class graphics_device;

    struct vertex_element_t {
        vertdata_meaning_t mMeaning;
        vertdata_type_t mType;
        uint32_t mIndex;
    };

    class vertex_format {
    public:
        virtual ~vertex_format();

        void add_element(vertdata_meaning_t pMeaning, vertdata_type_t pType);

        unsigned size() const {
            return mSize;
        }

        unsigned elements() const {
            return mElements.size();
        }

        bool has_normal() const {
            return mFlags[K_HAS_NORMAL];
        }

        bool has_tangent() const {
            return mFlags[K_HAS_TANGENT];
        }

        bool has_color() const {
            return mFlags[K_HAS_COLOR];
        }

        const vertex_element_t& operator[](unsigned i) const {
            return mElements[i];
        }

    protected:
        vertex_format(std::string pDescription);

        virtual void update_device() = 0;

    private:
        enum {
            K_HAS_NORMAL,
            K_HAS_TANGENT,
            K_HAS_COLOR,
            K_LAST_FLAG
        };

        std::string mDescription;
        std::bitset<K_LAST_FLAG> mFlags;
        std::vector<vertex_element_t> mElements;
        unsigned mSize;
        unsigned mNumTextures;
    };

#if 0
    class resource {
    public:
        virtual void sleep() = 0;
        virtual void wake() = 0;
    };
#endif

    // XXX On multi-head systems, buffers need to be tied to a device.
    class vertex_buffer {
    public:
        virtual ~vertex_buffer();

        virtual void select() = 0;

        virtual void deselect() = 0;

        const vertex_format& format() {
            return *mFormat;
        }

    protected:
        friend class mesh_builder;
        friend class vertex_buffer_builder_base;

        // Hooks for mesh_builder.
        virtual void* lock(uint32_t pVertexStart, uint32_t pVertexCount) = 0;
        virtual void unlock() = 0;

        buffer_lifetime_t mLifetime;
        uint32_t mVertexCount;
        std::shared_ptr<vertex_format> mFormat;

        vertex_buffer(buffer_lifetime_t pLifetime,
                std::shared_ptr<vertex_format> pFormat,
                uint32_t pVertexCount)
            : mLifetime(pLifetime),
              mVertexCount(pVertexCount),
              mFormat(std::move(pFormat))
        {
        }
    };

    class vertex_buffer_builder_base {
    public:
        vertex_buffer_builder_base(vertex_buffer& pBuffer)
                : mBuffer(pBuffer) {
            mData = mBuffer.lock(0, mBuffer.mVertexCount);
        }

        ~vertex_buffer_builder_base() {
            mBuffer.unlock();
        }

    protected:
        vertex_buffer& mBuffer;
        void* mData;
    };

    template<typename T>
    class vertex_buffer_builder : public vertex_buffer_builder_base {
    public:
        vertex_buffer_builder(vertex_buffer& pBuffer)
                : vertex_buffer_builder_base(pBuffer)
        {
        }

        T& operator[](unsigned i) {
            return reinterpret_cast<T*>(mData)[i];
        }
    };

    // XXX On multi-head systems, buffers need to be tied to a device.
    class index_buffer {
    public:
        virtual ~index_buffer();

        virtual void select() = 0;

        virtual void deselect() = 0;

    protected:
        friend class index_buffer_lock;

        // Hooks for mesh_builder.
        virtual void* lock(uint32_t pIndexStart, uint32_t pIndexCount) = 0;
        virtual void unlock() = 0;
    };

    class index_buffer_lock {
    private:
        index_buffer& mIB;
        uint16_t* mData;

    public:
        index_buffer_lock(index_buffer& pIB,
                    uint32_t pIndexStart, uint32_t pIndexCount)
                : mIB(pIB) {
            mData = reinterpret_cast<uint16_t*>(
                mIB.lock(pIndexStart, pIndexCount)
            );
        }

        ~index_buffer_lock() {
            mIB.unlock();
        }

        uint16_t& operator[](unsigned i) {
            return mData[i];
        }
    };

    class mesh {
    public:
        virtual ~mesh();

        virtual void select() = 0;

        virtual void deselect() = 0;

        virtual void draw() = 0;

    protected:
        friend class mesh_builder;

        primitive_type_t mType;
        uint32_t mVertexStart;
        uint32_t mVertexCount;
        uint32_t mIndexStart;
        uint32_t mIndexCount;
        uint16_t mMinIndex;

        std::weak_ptr<graphics_device> mDevice;
        std::shared_ptr<vertex_buffer> mVertexBuffer;

        mesh(std::weak_ptr<graphics_device> pDevice)
            : mDevice(std::move(pDevice))
        {
        }
    };


    class mesh_builder {
    public:
        mesh_builder(mesh& pMesh)
                : mMesh(pMesh) {
            setup();
        }

        ~mesh_builder();

        void position(float_t x, float_t y, float_t z);
        void position(const float_t* pP);
        void position(const point3_t& pP);

        void normal(float_t x, float_t y, float_t z);
        void normal(const float_t* pN);
        void normal(const vector3_t& pN);

        void color(float_t r, float_t g, float_t b);
        void color(float_t r, float_t g, float_t b, float_t a);
        void color(uint8_t r, uint8_t g, uint8_t b);
        void color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        void advance();

    private:
        mesh& mMesh;

        void setup();
        // void build_indexes();

        uint32_t mVertexSize;
        uint32_t mTextures;

        mutable float *mCurrPosition;
        mutable float *mCurrNormal;
        mutable uint8_t *mCurrColor;
        mutable float *mTexCoords[MAX_TEXTURE_UNITS];
    };


    inline void
    mesh_builder::position(float_t x, float_t y, float_t z) {
        float_t* dest = mCurrPosition;
        *dest++ = x;
        *dest++ = y;
        *dest = z;
    }

    inline void
    mesh_builder::position(const float_t* pP) {
        position(pP[0], pP[1], pP[2]);
    }


    inline void
    mesh_builder::position(const point3_t& pP) {
        position(pP.x, pP.y, pP.z);
    }


    inline void
    mesh_builder::normal(float_t x, float_t y, float_t z) {
        float_t* dest = mCurrNormal;
        *dest++ = x;
        *dest++ = y;
        *dest = z;
    }


    inline void
    mesh_builder::normal(const float_t* pN) {
        normal(pN[0], pN[1], pN[2]);
    }


    inline void
    mesh_builder::normal(const vector3_t& pN) {
        normal(pN.x, pN.y, pN.z);
    }

    inline void
    mesh_builder::color(float_t r, float_t g, float_t b) {
        uint8_t* dest = mCurrColor;
        *dest++ = (uint8_t)(r * 0xFF);
        *dest++ = (uint8_t)(g * 0xFF);
        *dest++ = (uint8_t)(b * 0xFF);
        *dest = 0xFF;
    }

    inline void
    mesh_builder::color(float_t r, float_t g, float_t b, float_t a) {
        uint8_t* dest = mCurrColor;
        *dest++ = (uint8_t)(r * 0xFF);
        *dest++ = (uint8_t)(g * 0xFF);
        *dest++ = (uint8_t)(b * 0xFF);
        *dest = (uint8_t)(a * 0xFF);
    }

    inline void
    mesh_builder::color(uint8_t r, uint8_t g, uint8_t b) {
        uint8_t* dest = mCurrColor;
        *dest++ = r;
        *dest++ = g;
        *dest++ = b;
        *dest = 0xFF;
    }

    inline void
    mesh_builder::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        uint8_t* dest = mCurrColor;
        *dest++ = r;
        *dest++ = g;
        *dest++ = b;
        *dest = a;
    }

    inline void
    mesh_builder::advance() {
        mCurrPosition = (float_t*)((uint8_t*)mCurrPosition + mVertexSize);
        mCurrNormal = (float_t*)((uint8_t*)mCurrNormal + mVertexSize);
        mCurrColor = (uint8_t*)((uint8_t*)mCurrColor + mVertexSize);
        for (unsigned i = 0; i < mTextures; ++i) {
            mTexCoords[i] = (float_t*)((uint8_t*)mTexCoords[i] + mVertexSize);
        }
    }

}

#endif // PRIMITIVE_HH_INCLUDED
