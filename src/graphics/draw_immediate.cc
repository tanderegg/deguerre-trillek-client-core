#include <draw_immediate.hh>
#include <primitive.hh>

namespace trillek {

namespace {

    inline std::shared_ptr<vertex_buffer>
    volatile_vertex_buffer(graphics_device& pDevice, unsigned pVertices) {
        return pDevice.make_vertex_buffer(
            pDevice.standard_vertex_format(STD_VTX_FMT_PC),
            pVertices,
            BUFFER_VOLATILE
        );
    }

}


draw_immediate::draw_immediate(graphics_device& pDevice)
    : mDevice(pDevice)
{
}


draw_immediate::~draw_immediate()
{
}


void
draw_immediate::draw_rect(const point2_t& pUL, const point2_t& pLR,
                    const rgba_t& pColor)
{
    auto vb = volatile_vertex_buffer(mDevice, 10);
    {
        vertex_buffer_builder<std_vtx_fmt_pc_t> b(*vb);
        float_t off = 0.5f;
        float_t hw = 0.5f;
        b[0].mPosition.set(pUL.x + hw + off, pUL.y + off + hw, 0.0f);
        b[0].mColor = pColor;
        b[1].mPosition.set(pUL.x + hw + off, pUL.y + off - hw, 0.0f);
        b[1].mColor = pColor;
        b[2].mPosition.set(pLR.x + hw, pUL.y + off + hw, 0.0f);
        b[2].mColor = pColor;
        b[3].mPosition.set(pLR.x - hw, pUL.y + off - hw, 0.0f);
        b[3].mColor = pColor;
        b[4].mPosition.set(pLR.x - hw, pLR.y - hw, 0.0f);
        b[4].mColor = pColor;
        b[5].mPosition.set(pLR.x + hw, pLR.y + hw, 0.0f);
        b[5].mColor = pColor;
        b[6].mPosition.set(pUL.x - hw + off, pLR.y - hw, 0.0f);
        b[6].mColor = pColor;
        b[7].mPosition.set(pUL.x + hw + off, pLR.y + hw, 0.0f);
        b[7].mColor = pColor;
        b[8].mPosition.set(pUL.x + hw + off, pUL.y + off + hw, 0.0f);
        b[8].mColor = pColor;
        b[9].mPosition.set(pUL.x - hw + off, pUL.y + off - hw, 0.0f);
        b[9].mColor = pColor;
    }
    mDevice.set_vertex_buffer(vb);
    mDevice.draw_primitive(PRIM_TRIANGLE_STRIP, 0, 8);
}


void
draw_immediate::fill_rect(const point2_t& pUL, const point2_t& pLR,
                    const rgba_t& pColor)
{
    auto vb = volatile_vertex_buffer(mDevice, 4);
    {
        vertex_buffer_builder<std_vtx_fmt_pc_t> b(*vb);
        float_t off = 0.5f;
        float_t hw = 0.5f;
        b[0].mPosition.set(pUL.x + hw + off, pUL.y + off + hw, 0.0f);
        b[0].mColor = pColor;
        b[1].mPosition.set(pLR.x + hw, pUL.y + off + hw, 0.0f);
        b[1].mColor = pColor;
        b[2].mPosition.set(pUL.x - hw + off, pLR.y + off - hw, 0.0f);
        b[2].mColor = pColor;
        b[3].mPosition.set(pLR.x - hw, pLR.y - hw, 0.0f);
        b[3].mColor = pColor;
        std::cerr << "fill_rect:\n";
        std::cerr << "  (" << b[0].mPosition.x << ',' << b[0].mPosition.y
                << ',' << b[0].mPosition.z << ")\n";
        std::cerr << "  (" << b[1].mPosition.x << ',' << b[1].mPosition.y
                << ',' << b[1].mPosition.z << ")\n";
        std::cerr << "  (" << b[2].mPosition.x << ',' << b[2].mPosition.y
                << ',' << b[2].mPosition.z << ")\n";
        std::cerr << "  (" << b[3].mPosition.x << ',' << b[3].mPosition.y
                << ',' << b[3].mPosition.z << ")\n";
    }
    mDevice.set_vertex_buffer(vb);
    mDevice.draw_primitive(PRIM_TRIANGLE_STRIP, 0, 2);
}


void
draw_immediate::draw_line(const point2_t& pStart, const point2_t& pEnd,
                    const rgba_t& pColor)
{
    auto vb = volatile_vertex_buffer(mDevice, 2);
    {
        vertex_buffer_builder<std_vtx_fmt_pc_t> b(*vb);
        b[0].mPosition.set(pStart.x, pStart.y, 0.0f);
        b[0].mColor = pColor;
        b[1].mPosition.set(pEnd.x, pEnd.y, 0.0f);
        b[1].mColor = pColor;
    }
    mDevice.set_vertex_buffer(vb);
    mDevice.draw_primitive(PRIM_LINES, 0, 1);
}

}

