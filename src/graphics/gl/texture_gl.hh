#ifndef TEXTURE_GL_HH_INCLUDE
#define TEXTURE_GL_HH_INCLUDE

#include <texture.hh>

namespace trillek {

    class texture_gl : public texture {
    public:
        texture_gl();
        virtual ~texture_gl();

        GLuint handle() const { return mHandle; }
        GLuint buffer() const { return mBuffer; }
        GLenum binding() const { return mBinding; }

    private:
        GLuint mHandle;
        GLuint mBuffer;
        GLenum mBinding;
        std::weak_ptr<graphics_device_gl> mDevice;
    };

}


#endif // TEXTURE_GL_HH_INCLUDE
