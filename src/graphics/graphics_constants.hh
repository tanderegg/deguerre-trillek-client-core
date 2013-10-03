#ifndef GRAPHICS_CONSTANTS_HH_INCLUDED
#define GRAPHICS_CONSTANTS_HH_INCLUDED

#include <utils.hh>
#include <maths.hh>
#include <color.hh>
#include <vector3.hh>

namespace trillek {
    enum device_statistics_t {
        STAT_POLY_COUNT = 0,
        STAT_DRAW_CALLS,
        STAT_TARGET_CHANGES,
        STAT_LAST
    };

    static constexpr unsigned MAX_MODEL_TRANSFORM_STACK = 16;

    static constexpr unsigned MAX_TEXTURE_UNITS = 16;

    enum standard_vertex_format_t {
        STD_VTX_FMT_PC = 0,
        STD_VTX_FMT_COUNT
    };

    struct std_vtx_fmt_pc_t {
        point3_t mPosition;
        rgba_t mColor;
    };

    enum buffer_lifetime_t {
        BUFFER_STATIC = 0,
        BUFFER_DYNAMIC,
        BUFFER_VOLATILE,
        BUFFER_LAST
    };

    enum primitive_type_t {
        PRIM_POINTS,
        PRIM_LINES,
        PRIM_LINE_STRIP,
        PRIM_LINE_LOOP,
        PRIM_TRIANGLES,
        PRIM_TRIANGLE_STRIP,
        PRIM_TRIANGLE_FAN,
        PRIM_POLYGON,
        PRIM_QUADS,
        PRIM_QUAD_STRIP,
        PRIM_LAST
    };

    enum vertdata_meaning_t {
        VERTDATA_POSITION = 0,
        VERTDATA_NORMAL,
        VERTDATA_COLOR,
        VERTDATA_TEXCOORD,
        VERTDATA_LAST
    };

    enum vertdata_type_t {
        VERTDATA_TYPE_FLOAT = 0,
        VERTDATA_TYPE_FLOAT2,
        VERTDATA_TYPE_FLOAT3,
        VERTDATA_TYPE_FLOAT4,
        VERTDATA_TYPE_BYTE4,
        VERTDATA_TYPE_LAST
    };

    enum format_t {
        FORMAT_FIRST = 0,

        FORMAT_A8 = 0,
        FORMAT_L8,

        FORMAT_A8L8,

        FORMAT_R8G8B8,

        FORMAT_R8G8B8A8,

        FORMAT_R16G16B16A16,
        FORMAT_R16G16B16A16F,

        FORMAT_R32G32B32A32F,

        FORMAT_DXT1,
        FORMAT_DXT2,
        FORMAT_DXT3,
        FORMAT_DXT4,
        FORMAT_DXT5,

        FORMAT_LAST,

        // First in each group
        FORMAT_8BIT = FORMAT_A8,
        FORMAT_16BIT = FORMAT_A8L8,
        FORMAT_24BIT = FORMAT_R8G8B8,
        FORMAT_32BIT = FORMAT_R8G8B8A8,
        FORMAT_64BIT = FORMAT_R16G16B16A16,
        FORMAT_128BIT = FORMAT_R32G32B32A32F,
        FORMAT_VARIABLE = FORMAT_DXT1
    };

    typedef unsigned clear_flags_t;
    static constexpr clear_flags_t CLEAR_COLOR = 1u << 0;
    static constexpr clear_flags_t CLEAR_DEPTH = 1u << 1;
    static constexpr clear_flags_t CLEAR_STENCIL = 1u << 2;

    enum cmp_func_t {
        CMP_NEVER = 0,
        CMP_LT,
        CMP_EQ,
        CMP_LE,
        CMP_GT,
        CMP_NE,
        CMP_GE,
        CMP_ALWAYS,
        CMP_LAST
    };
}

#endif // GRAPHICS_CONSTANTS_HH_INCLUDED
