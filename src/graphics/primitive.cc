#include <primitive.hh>

namespace trillek {

namespace {

    unsigned vertdata_type_size(vertdata_type_t pType) {
        switch (pType) {
        case VERTDATA_TYPE_FLOAT:
            return 4;

        case VERTDATA_TYPE_FLOAT2:
            return 8;

        case VERTDATA_TYPE_FLOAT3:
            return 12;

        case VERTDATA_TYPE_FLOAT4:
            return 16;

        case VERTDATA_TYPE_BYTE4:
            return 4;

        default:
            throw std::logic_error("vertdata_type_size");
        }
    }

}


void
vertex_format::add_element(vertdata_meaning_t pMeaning, vertdata_type_t pType) {
    ensure_capacity(mElements, 1);

    unsigned i = mElements.size();
    mElements.push_back(vertex_element_t());
    vertex_element_t& el = mElements.back();
    el.mMeaning = pMeaning;
    el.mType = pType;
    el.mIndex = i;

    switch (pMeaning) {
    case VERTDATA_POSITION:
        break;

    case VERTDATA_NORMAL:
        mFlags[K_HAS_NORMAL] = true;
        break;

    case VERTDATA_COLOR:
        mFlags[K_HAS_COLOR] = true;
        break;

    case VERTDATA_TEXCOORD:
        ++mNumTextures;
        break;

    default:
        throw std::logic_error("vertex_format::add_element");
    }

    mSize += vertdata_type_size(pType);

    update_device();
}


vertex_format::vertex_format(std::string pDescription)
        : mDescription(pDescription)
{
    mElements.reserve(4);
    mSize = 0;
    mNumTextures = 0;
}


vertex_format::~vertex_format() {
}


vertex_buffer::~vertex_buffer() {
}


mesh::~mesh() {
}


void
mesh_builder::setup() {
    vertex_buffer& vertBuffer = *mMesh.mVertexBuffer;

    const vertex_format& format = vertBuffer.format();
    mVertexSize = format.size();

    uint8_t* vertData
        = (uint8_t*)vertBuffer.lock(mMesh.mVertexStart, mMesh.mVertexCount);

    uint32_t textures = 0;
    uint32_t elements = format.elements();
    for (unsigned i = 0; i < elements; ++i) {
        const vertex_element_t& e = format[i];
        switch (e.mMeaning) {
        case VERTDATA_POSITION:
            mCurrPosition = (float_t*)vertData;
            break;

        case VERTDATA_NORMAL:
            mCurrNormal = (float_t*)vertData;
            break;

        case VERTDATA_TEXCOORD:
            mTexCoords[textures] = (float_t*)vertData;
            ++textures;
            break;

        case VERTDATA_COLOR:
            mCurrColor = (uint8_t*)vertData;
            break;

        default:
            throw std::logic_error("mesh_builder::setup");
        }
        vertData += vertdata_type_size(e.mType);
    }

    mTextures = textures;
}


/*
// XXX This should be render system-specific if D3D doesn't support all of
// these primitive types.
void
mesh_builder::build_indexes() {
    index_buffer_lock ib(*mMesh.mIndexBuffer, mMesh.mIndexStart, mMesh.mIndexCount);;
    
    uint32_t icount = mMesh.mIndexCount;

    switch (mMesh.mType) {
        case PRIM_POINTS:
        case PRIM_LINES:
        case PRIM_TRIANGLES:
        case PRIM_TRIANGLE_STRIP:
        case PRIM_TRIANGLE_FAN:
        case PRIM_LINE_STRIP:
        case PRIM_QUADS:
        case PRIM_QUAD_STRIP:
        case PRIM_POLYGON:
        {
            // vertex count == index count
            for (uint32_t i = 0; i < icount; ++i) {
                ib[i] = (uint16_t)i;
            }
            break;
        }

        default:
        {
            throw std::logic_error("mesh_builder::build_indexes");
        }
    }
}
*/


mesh_builder::~mesh_builder() {
    // build_indexes();
    mMesh.mVertexBuffer->unlock();
}

}
