#ifndef VERTEX_BUFFER_GL_HH_INCLUDED
#define VERTEX_BUFFER_GL_HH_INCLUDED

#include <graphics_gl.hh>
#include <primitive.hh>

namespace trillek {

struct vertex_buffer_gl : public vertex_buffer {
    GLuint mHandleGL;
    GLuint mLifetimeGL;
    uint32_t mVertexSize;

    vertex_buffer_gl(buffer_lifetime_t pLifetime,
                std::shared_ptr<vertex_format> pFormat,
                uint32_t pVertexCount);

    ~vertex_buffer_gl();

    void* lock(uint32_t pVertexStart, uint32_t pVertexCount);

    void unlock();

    void select();

    void deselect();

};

}

#endif // VERTEX_BUFFER_GL_HH_INCLUDED
