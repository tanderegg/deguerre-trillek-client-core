#ifndef TEXTURE_TARGET_GL_HH_INCLUDED
#define TEXTURE_TARGET_GL_HH_INCLUDED

#include <render_target.hh>
#include <graphics_gl.hh>

namespace trillek {

    class graphics_device_gl;

    class texture_target_gl : public texture_target {
    public:
        class slot {
        private:
            uint32_t mMipmapLevel;
            uint32_t mDepthOffset;

        public:
            slot(uint32_t pMipmapLevel, uint32_t pDepthOffset)
                : mMipmapLevel(pMipmapLevel), mDepthOffset(pDepthOffset)
            {
            }

            virtual ~slot()
            {
            }

            virtual GLuint handle() const = 0;
            virtual GLenum binding() const = 0;
            virtual uint32_t width() const = 0;
            virtual uint32_t height() const = 0;
            virtual uint32_t depth() const = 0;
            virtual bool is_mipmap() const = 0;

            uint32_t mipmap_level() const { return mMipmapLevel; } 
            uint32_t depth_offset() const { return mDepthOffset; } 
        };

        struct impl {
            virtual void apply_state(texture_target_gl& pTarget) = 0;
            virtual void select(texture_target_gl& pTarget) = 0;
            virtual void deselect(texture_target_gl& pTarget) = 0;
            virtual ~impl() = 0;
        };

        texture_target_gl(std::weak_ptr<graphics_device_gl> pDevice);

        virtual ~texture_target_gl();

        virtual void select();

        virtual void deselect();

        virtual void copy_to_texture(texture& pTex);

        virtual void attach(render_slot_t pSlot,
            const std::shared_ptr<texture>& pTex,
            uint32_t pMipmapLevel, uint32_t pDepthOffset);

        virtual void resolve();

        slot* get_slot(uint32_t pSlot) { return mSlots[pSlot].get(); }

    private:
	std::weak_ptr<graphics_device_gl> mDevice;
        std::unique_ptr<impl> mPImpl;
        std::unique_ptr<slot> mSlots[RENDER_SLOT_LAST];
    };

}

#endif // TEXTURE_TARGET_GL_HH_INCLUDED
