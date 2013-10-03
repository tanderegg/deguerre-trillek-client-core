#include <mesh_gl.hh>
#include <graphics_device_gl.hh>
#include <translate_constants_gl.hh>
#include <vertex_format_gl.hh>


namespace trillek {

void
mesh_gl::select()
{
//    mIndexBuffer->select();
    mVertexBuffer->select();
    glUseProgram(0);
}


void
mesh_gl::deselect()
{
//    mIndexBuffer->deselect();
    mVertexBuffer->deselect();
}


void
mesh_gl::draw()
{
    gl::select_guard<mesh_gl> sel(*this);
    std::shared_ptr<graphics_device>(mDevice)
        ->draw_primitive(mType, mVertexStart, mVertexCount);
}


mesh_gl::mesh_gl(const std::shared_ptr<graphics_device_gl>& pDevice,
                buffer_lifetime_t pLifetime, primitive_type_t pType,
                const std::shared_ptr<vertex_format_gl>& pFormat,
                uint32_t pPrimitiveCount)
    : mesh(pDevice)
{
    mType = pType;
    mVertexStart = 0;
    mVertexCount = translate_vertex_count_gl(pType, pPrimitiveCount);
    // mIndexStart = 0;
    // mIndexCount = translate_index_count_gl(pType, pPrimitiveCount);
    // mMinIndex = 0;
    // mIndexBuffer = pDevice->make_index_buffer(pLifetime, mIndexCount);
    mVertexBuffer = pDevice->make_vertex_buffer(
            std::static_pointer_cast<vertex_format>(pFormat),
            mVertexCount, pLifetime);
}


mesh_gl::~mesh_gl()
{
}


}

