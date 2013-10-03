#include <render_subsystem.hh>
#include <graphics_driver.hh>
#include <trillek_gl.hh>
#include "player.hh"
#include <stdexcept>
#include <iostream>

namespace {

    void
    throw_gl_error(GLuint err) {
        switch (err) {
        case GL_INVALID_ENUM:
            throw std::invalid_argument("GL_INVALID_ENUM");

        case GL_INVALID_VALUE:
            throw std::invalid_argument("GL_INVALID_VALUE");

        case GL_INVALID_OPERATION:
            throw std::invalid_argument("GL_INVALID_OPERATION");

        case GL_STACK_OVERFLOW:
            throw std::logic_error("GL_STACK_OVERFLOW");

        case GL_STACK_UNDERFLOW:
            throw std::logic_error("GL_STACK_UNDERFLOW");

        case GL_OUT_OF_MEMORY:
            throw std::runtime_error("GL_OUT_OF_MEMORY");

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            throw std::logic_error("GL_INVALID_FRAMEBUFFER_OPERATION");

        case GL_TABLE_TOO_LARGE:
            throw std::invalid_argument("GL_TABLE_TOO_LARGE");

        default:
            throw std::logic_error("Unknown OpenGL error");
        }
    }

#if 0
    const char*
    sVertShader =
        "attribute vec3 position;\n"
        "attribute vec3 normal;\n"
        "attribute vec4 color;\n"
        "varying vec4 colorVarying;\n"

        "uniform mat4 mvptransform;\n"
        "uniform mat3 ntransform;\n"

        "void main(void) {\n"
        "// vec3 eyeNormal = normalize(ntransform * normal);\n"
        "colorVarying = color;\n"
        "gl_Position = mvptransform * vec4(position, 1.0);\n"
        "// gl_Normal = normalize(ntransform * normal);\n"
        "}";
#endif

    enum {
        UNIFORM_MODELVIEWPROJECTIONMATRIX,
        UNIFORM_NORMALMATRIX,
        NUM_UNIFORMS
    };

    enum {
        VARYING_VERTEX,
        VARYING_NORMAL,
        VARYING_COLOR,
        NUM_VARYINGS
    };

    const char*
    sVertShader =
        "attribute vec3 position;\n"
        "attribute vec3 normal;\n"
        "attribute vec4 color;\n"

        "varying vec4 colorVarying;\n"

        "uniform mat4 mvptransform;\n"
        "uniform mat3 ntransform;\n"

        "void main()\n"
        "{\n"
        "vec3 eyeNormal = normalize(ntransform * normal);\n"
        "vec3 lightPosition = vec3(0.0, 0.0, 1.0);\n"
    
        "float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));\n"
                 
        "colorVarying = color * nDotVP;\n"
    
        "gl_Position = mvptransform * vec4(position,1);\n"
        "}\n";

    const char*
    sFragShader =
        "varying vec4 colorVarying;\n"
        "void main(void) {\n"
        "gl_FragColor = colorVarying;\n"
        "}\n";

    void dump_matrix4(const trillek::matrix4_t& pX) {
        const float_t* m = &pX.mM[0][0];
        std::cerr << '{';
        for (unsigned i = 0; i < 15; ++i) {
            std::cerr << ' ' << m[i] << ',';
        }
        std::cerr << ' ' << m[15] << " }\n";
    }
}

namespace trillek { namespace detail {


class render_subsystem_gl;

struct glconfig_t {
    const char* mRendererString;
    const char* mVendorString;
    const char* mVersionString;
    const char* mExtensionsString;

    float mVersion;

    int32_t mMaxTextureSize;
    int32_t mMaxTextureUnits;
    int32_t mMaxTextureCoords;
    int32_t mMaxTextureImageUnits;
    float_t mMaxTextureAnisotropy;
    int32_t mDepthBits;
    int32_t mStencilBits;
};


class shader_gl {
public:
    virtual ~shader_gl() {
        glDeleteShader(mHandleGL);
    }

    GLuint id() const {
        return mHandleGL;
    }

    shader_gl(const char* pText, GLuint pType) {
        mHandleGL = glCreateShader(pType);
        glShaderSource(mHandleGL, 1, (const GLchar**)&pText, 0);
        glCompileShader(mHandleGL);

        GLint status;
        glGetShaderiv(mHandleGL, GL_COMPILE_STATUS, &status);
        if (status == 0) {
            GLint logLength;
            glGetShaderiv(mHandleGL, GL_INFO_LOG_LENGTH, &logLength);
            std::unique_ptr<GLchar[]> log(new GLchar[logLength]);
            glGetShaderInfoLog(mHandleGL, logLength, &logLength, &log[0]);
            std::cerr << "Could not compile shader:\n" << log.get() << '\n';
            throw std::invalid_argument("shader_gl::shader_gl");
        }
    }

    GLuint mHandleGL;
};


class vertex_shader_gl : public shader_gl {
public:
    vertex_shader_gl(const char* pText)
            : shader_gl(pText, GL_VERTEX_SHADER)
    {
    }

    ~vertex_shader_gl() {
    }
};


class fragment_shader_gl : public shader_gl {
public:
    fragment_shader_gl(const char* pText)
            : shader_gl(pText, GL_FRAGMENT_SHADER)
    {
    }

    ~fragment_shader_gl() {
    }
};


class shader_program_gl {
public:
    shader_program_gl() {
        mProgram = glCreateProgram();
        std::cerr << "shader_program_gl constructor\n";
        check_gl_error();
        mShaders.reserve(2);
    }

    ~shader_program_gl() {
        glDeleteProgram(mProgram);
    }

    void link() {
        glBindAttribLocation(mProgram, VARYING_VERTEX, "position");
        glBindAttribLocation(mProgram, VARYING_NORMAL, "normal");
        glBindAttribLocation(mProgram, VARYING_COLOR, "color");
        glLinkProgram(mProgram);
        std::cerr << "shader_program_gl::link glLinkProgram\n";
        check_gl_error();
        mUniform[UNIFORM_MODELVIEWPROJECTIONMATRIX] = glGetUniformLocation(mProgram, "mvptransform");
        std::cerr << "shader_program_gl::link glGetUniformLocation\n";
        check_gl_error();
        mUniform[UNIFORM_NORMALMATRIX] = glGetUniformLocation(mProgram, "ntransform");
        std::cerr << "shader_program_gl::link glGetUniformLocation\n";
        check_gl_error();
    }

    void add_shader(std::shared_ptr<shader_gl> pShader) {
        glAttachShader(mProgram, pShader->mHandleGL);
        mShaders.push_back(std::move(pShader));
    }

    void select(const matrix4_t& pTransformMVP, const matrix3_t& pTransformN) {
        glUseProgram(mProgram);

        glUniformMatrix4fv(mUniform[UNIFORM_MODELVIEWPROJECTIONMATRIX], 1, GL_TRUE, &pTransformMVP.mM[0][0]);
        glUniformMatrix3fv(mUniform[UNIFORM_NORMALMATRIX], 1, GL_TRUE, &pTransformN.mM[0][0]);
    }

    void deselect() {
        glUseProgram(0);
    }

private:
    GLuint mProgram;
    std::vector< std::shared_ptr<shader_gl> > mShaders;
    GLuint mUniform[NUM_UNIFORMS];
};


struct vertex_format_gl : public vertex_format {
    vertex_format_gl(std::string pDescription)
        : vertex_format(pDescription)
    {
    }

    void update_device() {
    }
};


GLuint
translate_primitive_type_gl(primitive_type_t pType) {
    switch (pType) {
    case PRIM_POINTS:
        return GL_POINTS;

    case PRIM_LINES:
        return GL_LINES;

    case PRIM_LINE_STRIP:
        return GL_LINE_STRIP;

    case PRIM_LINE_LOOP:
        return GL_LINE_LOOP;

    case PRIM_TRIANGLES:
        return GL_TRIANGLES;

    case PRIM_TRIANGLE_STRIP:
        return GL_TRIANGLE_STRIP;

    case PRIM_TRIANGLE_FAN:
        return GL_TRIANGLE_FAN;

    case PRIM_POLYGON:
        return GL_POLYGON;

    case PRIM_QUADS:
        return GL_QUADS;

    case PRIM_QUAD_STRIP:
        return GL_QUAD_STRIP;

    default:
        throw std::logic_error("translate_primitive_type_gl");
    }
}


GLuint
translate_vertex_count_gl(primitive_type_t pType, uint32_t pPrimitiveCount) {
    switch (pType) {
    case PRIM_POINTS:
        return pPrimitiveCount;

    case PRIM_LINES:
        return pPrimitiveCount * 2;

    case PRIM_LINE_STRIP:
        return pPrimitiveCount + 1;

    case PRIM_LINE_LOOP:
        return pPrimitiveCount;

    case PRIM_TRIANGLES:
        return pPrimitiveCount * 3;

    case PRIM_TRIANGLE_STRIP:
        return pPrimitiveCount + 2;

    case PRIM_TRIANGLE_FAN:
        return pPrimitiveCount + 2;

    case PRIM_QUADS:
        return pPrimitiveCount * 4;

    case PRIM_QUAD_STRIP:
        return (pPrimitiveCount + 1) * 2;

    case PRIM_POLYGON:
        return pPrimitiveCount;

    default:
        throw std::logic_error("translate_vertex_count_gl");
    }
}

GLuint
translate_index_count_gl(primitive_type_t pType, uint32_t pPrimitiveCount) {
    switch (pType) {
    case PRIM_POINTS:
        return pPrimitiveCount;

    case PRIM_LINES:
        return pPrimitiveCount * 2;

    case PRIM_LINE_STRIP:
        return pPrimitiveCount + 1;

    case PRIM_LINE_LOOP:
        return pPrimitiveCount;

    case PRIM_TRIANGLES:
        return pPrimitiveCount * 3;

    case PRIM_TRIANGLE_STRIP:
        return pPrimitiveCount + 2;

    case PRIM_TRIANGLE_FAN:
        return pPrimitiveCount + 2;

    case PRIM_QUADS:
        return pPrimitiveCount * 4;

    case PRIM_QUAD_STRIP:
        return (pPrimitiveCount + 1) * 2;

    case PRIM_POLYGON:
        return pPrimitiveCount;

    default:
        throw std::logic_error("translate_index_count_gl");
    }
}


GLuint
translate_buffer_lifetime_gl(buffer_lifetime_t pLifetime) {
    switch (pLifetime) {
    case BUFFER_STATIC:
        return GL_STATIC_DRAW;

    case BUFFER_DYNAMIC:
        return GL_DYNAMIC_DRAW;

    case BUFFER_VOLATILE:
        return GL_STREAM_DRAW;

    default:
        throw std::logic_error("translate_buffer_lifetime_gl");
    }
}


GLuint
translate_vertdata_bytesize_gl(vertdata_type_t pType) {
    switch (pType) {
    case VERTDATA_TYPE_FLOAT:
        return 4;

    case VERTDATA_TYPE_FLOAT2:
        return 8;

    case VERTDATA_TYPE_FLOAT3:
        return 12;

    case VERTDATA_TYPE_FLOAT4:
        return 16;

    case VERTDATA_TYPE_BYTE4:
        return 4;

    default:
        throw std::logic_error("translate_vertdata_bytesize_gl");
    }
}


GLuint
translate_vertdata_type_gl(vertdata_type_t pType) {
    switch (pType) {
    case VERTDATA_TYPE_FLOAT:
        return 1;

    case VERTDATA_TYPE_FLOAT2:
        return 2;

    case VERTDATA_TYPE_FLOAT3:
        return 3;

    case VERTDATA_TYPE_FLOAT4:
        return 4;

    case VERTDATA_TYPE_BYTE4:
        return 4;

    default:
        throw std::logic_error("translate_vertdata_type_gl");
    }
}


struct index_buffer_gl : public index_buffer {
    unsigned mIndexCount;
    GLuint mHandleGL;
    GLuint mLifetimeGL;

    index_buffer_gl(buffer_lifetime_t pLifetime, unsigned pIndexCount)
            : mIndexCount(pIndexCount) {
        preserve_index_buffer_gl idxbuf;

        mLifetimeGL = translate_buffer_lifetime_gl(pLifetime);
        glGenBuffers(1, &mHandleGL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandleGL);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndexCount * sizeof(uint16_t),
            NULL, mLifetimeGL);
        check_gl_error();
    }

    ~index_buffer_gl() {
        glDeleteBuffers(1, &mHandleGL);
    }

    void* lock(uint32_t pIndexStart, uint32_t pIndexCount) {
        preserve_index_buffer_gl idxbuf;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandleGL);
        std::cerr << "index_buffer_gl::lock glBindBuffer\n";
        check_gl_error();
        uint16_t* buffer
            = (uint16_t*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        std::cerr << "index_buffer_gl::lock glMap\n";
        check_gl_error();
        return (void*)(buffer + pIndexStart);
    }

    void unlock() {
        preserve_index_buffer_gl idxbuf;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandleGL);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }

    void select() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandleGL);
    }

    void deselect() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

};



struct mesh_gl : public mesh {
    render_subsystem_gl& mSubsystem;
    uint32_t mPrimitiveCount;

    ~mesh_gl() {
    }

    void select();

    void deselect();

    void draw();

    mesh_gl(render_subsystem_gl& pSubsystem, buffer_lifetime_t pLifetime,
             primitive_type_t pType, std::shared_ptr<vertex_format> pFormat,
             uint32_t pPrimitiveCount);

};



class render_subsystem_gl : public render_subsystem
{
public:
    graphics_driver* mGraphicsDriver;
    graphics_config_t mGraphicsConfig;
    uint32_t mWidth, mHeight;

    std::shared_ptr<vertex_buffer_gl> mVertexBuffer;
    std::shared_ptr<index_buffer_gl> mIndexBuffer;
    std::shared_ptr<shader_program_gl> mShaderProgram;

    bool mTransformDirty;
    matrix4_t mTransformMV;
    matrix4_t mTransformMVP;
    matrix3_t mTransformN;

    render_subsystem_gl() {
        mGraphicsDriver = nullptr;
    }

    interface_key_t implements() const {
        return render_subsystem::s_interface;
    }

    void init(const subsystem_manager& pMgr) {
        base_init();

        graphics_driver& graphicsDriver = pMgr.lookup<graphics_driver>();
        mGraphicsDriver = &graphicsDriver;

        auto vs = std::make_shared<vertex_shader_gl>(sVertShader);
        auto fs = std::make_shared<fragment_shader_gl>(sFragShader);
        auto program = std::make_shared<shader_program_gl>();
        program->add_shader(vs);
        program->add_shader(fs);
        program->link();
        mShaderProgram = program;
    }

    void post_init() {
        mGraphicsDriver->get_dimensions(mWidth, mHeight);
        mGraphicsDriver->get_config(mGraphicsConfig);
    }

    void pre_shutdown() {
    }

    void shutdown() {
    }

    void load_model_transform(const matrix4_t& pXform) {
        mTransformDirty = true;
    }

    void load_camera_transform(const matrix4_t& pXform) {
        mTransformDirty = true;
    }

    void load_projection_transform(const matrix4_t& pXform) {
        mTransformDirty = true;
    }

    void recalculate_transforms() {
        if (mTransformDirty) {
            std::cerr << "Recalculating transforms\n";
            mTransformMV = camera_transform() * model_transform();
            mTransformMVP = projection_transform() * mTransformMV;
            dump_matrix4(mTransformMV);
            dump_matrix4(mTransformMVP);
            mTransformN = dual_space(mTransformMVP);

            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(&mTransformMV.mM[0][0]);
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(&projection_transform().mM[0][0]);
            glMatrixMode(GL_MODELVIEW);

            mTransformDirty = false;
        }
    }

    void clear() {
        glViewport(0, 0, mWidth, mHeight);
        // glScissor(0, 0, mWidth, mHeight);
        glDepthMask(GL_TRUE);
        glClearDepth(1.0f);
        glClearColor(0.2f, 0.2f, 0.0f, 1.0f);
        glStencilMask(0xFF);
        glClearStencil(1 << (mGraphicsConfig.mStencilBits - 1));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_STENCIL_TEST);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
    }

    void pre_draw_primitive() {
    }

    void post_draw_primitive(uint32_t pPrimitiveCount) {
        ++mStatistics.mDrawCalls;
        mStatistics.mPolyCount += pPrimitiveCount;
    }

    void draw_primitive(primitive_type_t pType,
                const vertex_buffer& pVertices, uint32_t pVertexStart,
                uint32_t pPrimitiveCount) {
        preserve_vertex_buffer_gl vtxbuf;

        glBindBuffer(GL_ARRAY_BUFFER,
            static_cast<const vertex_buffer_gl&>(pVertices).mHandleGL);

        pre_draw_primitive();

        glDrawArrays(translate_primitive_type_gl(pType), pVertexStart,
            translate_index_count_gl(pType, pPrimitiveCount));

        post_draw_primitive(pPrimitiveCount);
    }

    void draw_primitive(primitive_type_t pType, uint32_t pVertexStart,
            uint32_t pPrimitiveCount) {
        pre_draw_primitive();

        glDrawArrays(translate_primitive_type_gl(pType), pVertexStart,
            translate_index_count_gl(pType, pPrimitiveCount));

        post_draw_primitive(pPrimitiveCount);
    }

    void frame_begin() {
        mGraphicsDriver->begin_rendering();
        clear();
    }

    void frame_end() {
        mGraphicsDriver->end_rendering();
        mGraphicsDriver->swap_buffers();
    }

    std::unique_ptr<vertex_format>
    make_vertex_format(std::string pDescription) {
        return std::unique_ptr<vertex_format>(
            new vertex_format_gl(std::move(pDescription))
        );
    }

    std::unique_ptr<index_buffer>
    make_index_buffer(buffer_lifetime_t pLifetime, unsigned pIndexCount) {
        return std::unique_ptr<index_buffer>(
            new index_buffer_gl(pLifetime, pIndexCount)
        );
    }

    std::unique_ptr<vertex_buffer>
    make_vertex_buffer(buffer_lifetime_t pLifetime,
                std::shared_ptr<vertex_format> pFormat,
                uint32_t pVertexCount) {
        return std::unique_ptr<vertex_buffer>(
            new vertex_buffer_gl(pLifetime, std::move(pFormat), pVertexCount)
        );
    }

    std::unique_ptr<mesh>
    make_mesh(buffer_lifetime_t pLifetime, primitive_type_t pType,
                std::shared_ptr<vertex_format> pFormat,
                uint32_t pPrimitiveCount) {
        return std::unique_ptr<mesh>(
            new mesh_gl(*this, pLifetime, pType, std::move(pFormat),
                        pPrimitiveCount)
        );
    }
};


inline
mesh_gl::mesh_gl(render_subsystem_gl& pSubsystem,
         buffer_lifetime_t pLifetime, primitive_type_t pType,
         std::shared_ptr<vertex_format> pFormat, uint32_t pPrimitiveCount)
    : mSubsystem(pSubsystem)
{
    mType = pType;
    mVertexStart = 0;
    mVertexCount = translate_vertex_count_gl(pType, pPrimitiveCount);
    mIndexStart = 0;
    mIndexCount = translate_index_count_gl(pType, pPrimitiveCount);
    mMinIndex = 0;
    mIndexBuffer = mSubsystem.make_index_buffer(pLifetime, mIndexCount);
    mVertexBuffer = mSubsystem.make_vertex_buffer(pLifetime,
            std::move(pFormat), mVertexCount);
    mPrimitiveCount = pPrimitiveCount;
}



void
mesh_gl::draw() {
    select_guard<mesh_gl> sel(*this);
    mSubsystem.draw_primitive(mType, mVertexStart, mPrimitiveCount);
}


void
mesh_gl::select() {
    mIndexBuffer->select();
    mVertexBuffer->select();
    mSubsystem.recalculate_transforms();
    // mShaderProgram->select(mSubsystem.mTransformMVP, mSubsystem.mTransformN);
    glUseProgram(0);
}

void
mesh_gl::deselect() {
    mIndexBuffer->deselect();
    mVertexBuffer->deselect();
    // mShaderProgram->deselect();
}

static render_subsystem_gl
sRenderSubsystem;

} }




namespace trillek {
    render_subsystem& get_render_subsystem()
    {
        return detail::sRenderSubsystem;
    }
}



