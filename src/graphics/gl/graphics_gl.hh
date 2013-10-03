#ifndef GRAPHICS_GL_HH_INCLUDED
#define GRAPHICS_GL_HH_INCLUDED

#include <utils.hh>

#ifdef __gl_h_
#error gl.h included before graphics_gl.hh
#endif

#ifdef __glext_h_
#error glext.h included before graphics_gl.hh
#endif

#ifdef __wglext_h_
#error wglext.h included before graphics_gl.hh
#endif

#ifdef __glxext_h_
#error glxext.h included before graphics_gl.hh
#endif

#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <GL/gl.h>
#elif defined(__APPLE__) || defined(__APPLE_CC__)
# include <OpenGL/gl.h>
#else // GLX
# define __glext_h_  /* prevent glext.h from being included  */
# define __glxext_h_ /* prevent glxext.h from being included */
# include <GL/gl.h>
# include <GL/glx.h>
#endif


namespace trillek { namespace gl {


extern void throw_gl_error(GLenum pErr);


inline void
check_gl_error() {
    GLuint err = glGetError();
    if (err != GL_NO_ERROR) {
        throw_gl_error(err);
    }
}


template<typename T>
struct select_guard {
    T& mObject;

    select_guard(T& pObject)
        : mObject(pObject)
    {
        mObject.select();
    }

    ~select_guard()
    {
        mObject.deselect();
    }
};


class preserve_flag
{
public: 
    ~preserve_flag()
    {
        if (mEnabled) {
            glEnable(mCap);
        } else {
            glDisable(mCap);
        }
    }

protected:
    preserve_flag(GLenum pCap)
        : mCap(pCap)
    {
        mEnabled = glIsEnabled(pCap);
    }

private:
    GLenum mCap;
    bool mEnabled;
};


template<typename T>
class preserve
{
public: 
    ~preserve()
    {
       mBinder(mBinding, mPreserved);
    }

protected:
    typedef void(*BindFn)(GLenum, T);

    preserve(GLenum pBinding, GLenum pGetBinding, BindFn pBinder)
        : mBinding(pBinding), mPreserved(0), mBinder(pBinder)
    {
        glGetIntegerv(pGetBinding, reinterpret_cast<GLint*>(&mPreserved));
    }

private:
    GLenum mBinding;
    T mPreserved;
    BindFn mBinder;
};


class preserve_index_buffer : public preserve<GLuint> {
public:
    preserve_index_buffer()
        : preserve(GL_ELEMENT_ARRAY_BUFFER,
                   GL_ELEMENT_ARRAY_BUFFER_BINDING,
                   glBindBuffer)
    {
    }
};


class preserve_vertex_buffer : public preserve<GLuint> {
public:
    preserve_vertex_buffer()
        : preserve(GL_ARRAY_BUFFER,
                      GL_ARRAY_BUFFER_BINDING,
                      glBindBuffer)
    {
    }
};


class preserve_2d_texture : public preserve<GLuint> {
public:
    preserve_2d_texture()
        : preserve(GL_TEXTURE_2D,
                      GL_TEXTURE_BINDING_2D,
                      glBindTexture)
    {
    }
};


class preserve_3d_texture : public preserve<GLuint> {
public:
    preserve_3d_texture()
        : preserve(GL_TEXTURE_3D,
                      GL_TEXTURE_BINDING_3D,
                      glBindTexture)
    {
    }
};


class preserve_read_framebuffer : public preserve<GLuint> {
public:
    preserve_read_framebuffer()
        : preserve(GL_READ_FRAMEBUFFER,
                      GL_READ_FRAMEBUFFER_BINDING,
                      glBindFramebuffer)
    {
    }
};


class preserve_draw_framebuffer : public preserve<GLuint> {
public:
    preserve_draw_framebuffer()
        : preserve(GL_DRAW_FRAMEBUFFER,
                      GL_DRAW_FRAMEBUFFER_BINDING,
                      glBindFramebuffer)
    {
    }
};


class preserve_framebuffer {
public:
    preserve_framebuffer()
    {
    }

    ~preserve_framebuffer()
    {
    }

private:
    preserve_read_framebuffer mRead;
    preserve_draw_framebuffer mWrite;
};


class preserve_cull_face : public preserve_flag {
public:
    preserve_cull_face()
        : preserve_flag(GL_CULL_FACE)
    {
    }
};


class preserve_depth_test : public preserve_flag {
public:
    preserve_depth_test()
        : preserve_flag(GL_DEPTH_TEST)
    {
    }
};

} }


#endif // GRAPHICS_GL_HH_INCLUDED
