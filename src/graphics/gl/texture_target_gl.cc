#include "texture_target_gl.hh"
#include "texture_gl.hh"
#include "graphics_device_gl.hh"

namespace trillek {

namespace {

    void
    throw_framebuffer_error(GLenum pStatus) {
        switch (pStatus) {
            case GL_FRAMEBUFFER_UNSUPPORTED:
                throw std::logic_error("Unsupported FBO");
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                throw std::logic_error("Incomplete FBO attachment");
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                throw std::logic_error("Missing FBO attachment");
/*
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                throw std::logic_error("Incomplete layer targets in FBO");
*/
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                throw std::logic_error("Incomplete draw buffer in FBO");
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                throw std::logic_error("Incomplete read buffer in FBO");
/*
            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                throw std::logic_error("Incomplete FBO dimensions");
            case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
                throw std::logic_error("Incomplete FBO formats");
*/
            default:
                throw std::logic_error("Unknown FBO error");
        }
    }

    inline void
    check_framebuffer_status() {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            throw_framebuffer_error(status);
        }
    }

}


struct texture_slot_gl : public texture_target_gl::slot {
    std::shared_ptr<texture_gl> mTex;

    texture_slot_gl(std::shared_ptr<texture_gl> pTex,
                uint32_t pMipmapLevel, uint32_t pDepthOffset)
        : texture_target_gl::slot(pMipmapLevel, pDepthOffset),
          mTex(std::move(pTex))
    {
    }

    ~texture_slot_gl()
    {
    }

    virtual GLuint handle() const
    {
        return mTex->handle();
    }

    virtual uint32_t width() const
    {
        return mTex->width();
    }

    virtual uint32_t height() const
    {
        return mTex->height();
    }

    virtual uint32_t depth() const
    {
        return mTex->depth();
    }

    virtual bool is_mipmap() const
    {
        return mTex->mipmap_levels() > 1;
    }

    virtual GLenum binding() const
    {
        return mTex->binding();
    }
};



struct texture_target_gl_fbo : public texture_target_gl::impl {
    GLuint mFramebuffer;

    texture_target_gl_fbo()
    {
        glGenFramebuffers(1, &mFramebuffer);
    }

    ~texture_target_gl_fbo()
    {
        glDeleteFramebuffers(1, &mFramebuffer);
    }

    void
    apply_state(texture_target_gl& pTarget)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
   
        auto color0 = pTarget.get_slot(texture_target::COLOR0);
        if (color0)
        {
            texture_target_gl::slot& c0 = *color0;
            if (c0.depth() == 0) {
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                   GL_COLOR_ATTACHMENT0, c0.binding(), c0.handle(),
                   c0.mipmap_level());
            } else {
                glFramebufferTexture3D(GL_FRAMEBUFFER,
                   GL_COLOR_ATTACHMENT0, c0.binding(), c0.handle(),
                   c0.mipmap_level(), c0.depth_offset());
            }
        } else {
            // Clears the texture (note that the binding is irrelevent)
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        }
   
        auto depthStencil = pTarget.get_slot(texture_target::DEPTH_STENCIL);
        if (depthStencil)
        {
            texture_target_gl::slot& ds = *depthStencil;
            if (!color0) {
                throw std::logic_error("Cannot set DEPTH_STENCIL target without COLOR0 target");
            }
            texture_target_gl::slot& c0 = *color0;
            if (ds.width() != c0.width() || ds.height() != c0.height()) {
                throw std::logic_error("DEPTH_STENCIL and COLOR0 targets have different dimensions");
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT, ds.binding(), ds.handle(),
                ds.mipmap_level());
        }
        else
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void
    select(texture_target_gl& pTarget)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer);
    }

    void
    deselect(texture_target_gl& pTarget)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        auto color0 = pTarget.get_slot(texture_target::COLOR0);
        if (!color0) {
            return;
        }
        texture_target_gl::slot& c0 = *color0;
        if (!c0.is_mipmap()) {
           return;
        }

        glActiveTexture(GL_TEXTURE0);

        gl::preserve_2d_texture tex2d;
        glBindTexture(GL_TEXTURE_2D, c0.handle());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
};


#if 0
struct texture_target_gl_aux : public texture_target_gl::impl {
    GLuint mNum;

    texture_target_gl_aux(GLuint pNum = 0)
        : mNum(pNum)
    {
    }

    ~texture_target_gl_aux()
    {
    }

    void
    apply_state(texture_target_gl& pTarget)
    {
    }

    void
    select(texture_target_gl& pTarget)
    {
        glDrawBuffer(GL_AUX0 + mNum);
        glReadBuffer(GL_AUX0 + mNum);
    }

    void
    deselect(texture_target_gl& pTarget)
    {
        std::shared_ptr<texture_slot_gl> color0
            = pTarget.get_slot(texture_target::COLOR0);
        texture_target_gl::slot& c0 = *color0;

        preserve_2d_texture_gl tex2d;
        glBindTexture(c0.binding(), c0.handle());
        glCopyTexSubImage2D(c0.binding(), 0, 0, 0, 0, 0,
                c0.width(), c0.height());
        glDrawBuffer(GL_BACK);
        glReadBuffer(GL_BACK);
    }
};
#endif


texture_target_gl::texture_target_gl(std::weak_ptr<graphics_device_gl> pDevice)
    : mDevice(pDevice), mPImpl(new texture_target_gl_fbo)
{
}


texture_target_gl::~texture_target_gl()
{
}


void
texture_target_gl::attach(render_slot_t pSlot,
        const std::shared_ptr<texture>& pTex,
        uint32_t pMipmapLevel, uint32_t pDepthOffset)
{
    mChanges.record_change();
    if (pTex) {
        mSlots[pSlot] = std::unique_ptr<slot>(
            new texture_slot_gl(std::static_pointer_cast<texture_gl>(pTex),
                    pMipmapLevel, pDepthOffset)
        );
    }
    else {
        mSlots[pSlot] = std::unique_ptr<slot>();
    }
}

}
