#ifndef MESH_GL_HH_INCLUDED
#define MESH_GL_HH_INCLUDED

#include <primitive.hh>
#include <graphics_gl.hh>

namespace trillek {

    class graphics_device_gl;
    class vertex_format_gl;

    class mesh_gl : public mesh {
    private:
        GLuint mBuffer;

    public:
        void select();

        void deselect();

        void draw();

        mesh_gl(const std::shared_ptr<graphics_device_gl>& pDevice,
                buffer_lifetime_t pLifetime, primitive_type_t pType,
                const std::shared_ptr<vertex_format_gl>& pFormat,
                uint32_t pPrimitiveCount);

        ~mesh_gl();
    };

}


#endif // MESH_GL_HH_INCLUDED
