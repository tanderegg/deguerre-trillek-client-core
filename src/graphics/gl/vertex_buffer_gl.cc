#include <vertex_buffer_gl.hh>
#include <translate_constants_gl.hh>
#include <translate_constants_gl.hh>

namespace trillek {


vertex_buffer_gl::vertex_buffer_gl(buffer_lifetime_t pLifetime,
            std::shared_ptr<vertex_format> pFormat,
            uint32_t pVertexCount)
    : vertex_buffer(pLifetime, std::move(pFormat), pVertexCount),
      mLifetimeGL(translate_buffer_lifetime_gl(pLifetime))
{
    gl::preserve_vertex_buffer vtxbuf;

    mVertexSize = mFormat->size();
    glGenBuffers(1, &mHandleGL);
    glBindBuffer(GL_ARRAY_BUFFER, mHandleGL);
    glBufferData(GL_ARRAY_BUFFER, pVertexCount * mVertexSize, NULL,
            mLifetimeGL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl::check_gl_error();
}


vertex_buffer_gl::~vertex_buffer_gl() {
    glDeleteBuffers(1, &mHandleGL);
}


void*
vertex_buffer_gl::lock(uint32_t pVertexStart, uint32_t pVertexCount) {
    gl::preserve_vertex_buffer vtxbuf;

    glBindBuffer(GL_ARRAY_BUFFER, mHandleGL);
    gl::check_gl_error();
    uint8_t* buffer = (uint8_t*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    gl::check_gl_error();
    return (void*)(buffer + pVertexStart * mVertexSize);
}



void
vertex_buffer_gl::unlock() {
    gl::preserve_vertex_buffer vtxbuf;

    glBindBuffer(GL_ARRAY_BUFFER, mHandleGL);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}


void
vertex_buffer_gl::select() {
    glBindBuffer(GL_ARRAY_BUFFER, mHandleGL);

    vertex_format& format = *mFormat;
    uint32_t vertsize = format.size();
    uint32_t nelements = format.elements();
    uint32_t texture = 0;
    uint8_t* bufferStart = nullptr;

    for (unsigned i = 0; i < nelements; ++i) {
        const vertex_element_t& element = format[i];
        GLuint elsize = translate_vertdata_type_gl(element.mType);
        switch (element.mMeaning) {
        case VERTDATA_POSITION:
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(elsize, GL_FLOAT, vertsize, bufferStart);
            break;

        case VERTDATA_NORMAL:
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, vertsize, bufferStart);
            break;

        case VERTDATA_COLOR:
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(elsize, GL_UNSIGNED_BYTE, vertsize, bufferStart);
            break;

        case VERTDATA_TEXCOORD:
            glClientActiveTexture(GL_TEXTURE0 + texture);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(elsize, GL_FLOAT, vertsize, bufferStart);
            ++texture;
            break;

        default:
            throw std::logic_error("vertex_buffer_gl::select");
        }
        bufferStart += translate_vertdata_bytesize_gl(element.mType);
    }
    gl::check_gl_error();
}

void
vertex_buffer_gl::deselect() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    vertex_format& format = *mFormat;
    uint32_t nelements = format.elements();
    uint32_t texture = 0;

    for (unsigned i = 0; i < nelements; ++i) {
        switch (format[i].mMeaning) {
        case VERTDATA_POSITION:
            glDisableClientState(GL_VERTEX_ARRAY);
            break;

        case VERTDATA_NORMAL:
            glDisableClientState(GL_NORMAL_ARRAY);
            break;

        case VERTDATA_COLOR:
            glDisableClientState(GL_COLOR_ARRAY);
            break;

        case VERTDATA_TEXCOORD:
            glClientActiveTexture(GL_TEXTURE0 + texture);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            ++texture;
            break;

        default:
            throw std::logic_error("vertex_buffer_gl::deselect");
        }
    }
    gl::check_gl_error();
}

}

