#ifndef GRAPHICS_DEVICE_GL_HH_INCLUDED
#define GRAPHICS_DEVICE_GL_HH_INCLUDED

#include <graphics_gl.hh>
#include <graphics_device.hh>

namespace trillek {

    class graphics_device_gl
            : public graphics_device,
              public std::enable_shared_from_this<graphics_device_gl>
    {
    public:
        graphics_device_gl();

        virtual ~graphics_device_gl();

        virtual std::unique_ptr<vertex_format>
        make_vertex_format(std::string pName);

        virtual std::unique_ptr<mesh>
        make_mesh(primitive_type_t pType,
                const std::shared_ptr<vertex_format>& pFmt,
                uint32_t mCount, buffer_lifetime_t pLifetime);

        virtual std::unique_ptr<vertex_buffer>
        make_vertex_buffer(std::shared_ptr<vertex_format> pFmt,
                uint32_t pVertCount, buffer_lifetime_t pLifetime);

        virtual void draw_primitive(primitive_type_t pType, uint32_t pVertexStart, uint32_t pPrimitiveCount);

        virtual std::shared_ptr<window_target> make_window_target(
                const std::shared_ptr<window>& pWindow);

/*
        virtual std::unique_ptr<texture_target> make_texture_target();
*/

        virtual void clear(clear_flags_t pFlags, const rgba_t& pColor,
                float_t pZ, uint32_t pStencil);

        virtual void begin_frame_internal();
        virtual void end_frame_internal();

        virtual void update_transforms_internal(bool pForce);

        virtual void update_graphics_state_internal(bool pForce);

        virtual void update_vertex_buffer_internal();

        virtual void update_index_buffer_internal();

        virtual void update_render_target_internal();

        virtual void update_viewport_internal();

    private:
        matrix4_t mModelViewXform;

        void pre_draw_primitive();
        void post_draw_primitive(uint32_t pPrimitiveCount);
    };

}


#endif // GRAPHICS_DEVICE_GL_HH_INCLUDED
