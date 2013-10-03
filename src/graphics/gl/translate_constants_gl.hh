#ifndef TRANSLATE_CONSTANTS_GL_HH_INCLUDED
#define TRANSLATE_CONSTANTS_GL_HH_INCLUDED

#include <graphics_constants.hh>
#include <graphics_gl.hh>

namespace trillek {


inline GLuint
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


inline GLuint
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

inline GLuint
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


inline GLuint
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


inline GLuint
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


inline GLuint
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

inline GLuint
translate_clear_flags_gl(clear_flags_t pFlags) {
    GLuint flags = 0;
    if (pFlags & CLEAR_COLOR) {
        flags |= GL_COLOR_BUFFER_BIT;
    }
    if (pFlags & CLEAR_DEPTH) {
        flags |= GL_DEPTH_BUFFER_BIT;
    }
    if (pFlags & CLEAR_STENCIL) {
        flags |= GL_STENCIL_BUFFER_BIT;
    }
    return flags;
}


inline GLenum
translate_cmp_func_gl(cmp_func_t pFunc) {
    switch (pFunc) {
    case CMP_NEVER:
        return GL_NEVER;
    case CMP_LT:
        return GL_LESS;
    case CMP_EQ:
        return GL_EQUAL;
    case CMP_LE:
        return GL_LEQUAL;
    case CMP_GT:
        return GL_GREATER;
    case CMP_NE:
        return GL_NOTEQUAL;
    case CMP_GE:
        return GL_GEQUAL;
    case CMP_ALWAYS:
        return GL_ALWAYS;
    default:
        throw std::logic_error("translate_cmp_func_gl");
    }
}

}

#endif
