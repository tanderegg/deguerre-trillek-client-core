#ifndef RENDER_TARGET_HH_INCLUDED
#define RENDER_TARGET_HH_INCLUDED

#include <graphics_constants.hh>
#include <window.hh>
#include <vector2.hh>

namespace trillek {

    class texture;

    class render_target {
    protected:
        change_tracker mChanges;

        render_target();

    public:
        virtual void select() = 0;

        virtual void deselect() = 0;

        virtual void copy_to_texture(texture& pTex) = 0;

        virtual ~render_target();

        virtual format_t get_format() const = 0;

        virtual vector2i_t get_dimensions() = 0;
    };


    class window_target : public render_target {
    public:
        virtual bool swap_buffers() = 0;

    protected:
	window_target(std::weak_ptr<window> pWindow);

	std::weak_ptr<window> mWindow;
    };


    class texture_target : public render_target {
    public:
        enum render_slot_t {
            DEPTH_STENCIL = 0,
            COLOR0,
            COLOR1,
            COLOR2,
            COLOR3,
            COLOR4,
            RENDER_SLOT_LAST
        };

        virtual void attach(render_slot_t pSlot,
            const std::shared_ptr<texture>& pTex,
            uint32_t pMipmapLevel = 0, uint32_t pDepthOffset = 0) = 0;

        virtual void resolve() = 0;
    };

}


#endif // RENDER_TARGET_HH_INCLUDED
