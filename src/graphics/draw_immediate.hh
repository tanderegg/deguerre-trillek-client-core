#ifndef DRAW_IMMEDIATE_HH_INCLUDED
#define DRAW_IMMEDIATE_HH_INCLUDED

#include <graphics_device.hh>
#include <vector2.hh>

namespace trillek {

class draw_immediate {
public:
    draw_immediate(graphics_device& pDevice);
    ~draw_immediate();

    void draw_rect(const point2_t& pUL, const point2_t& pLR,
                    const rgba_t& pColor);

    void fill_rect(const point2_t& pUL, const point2_t& pLR,
                    const rgba_t& pColor);

    void draw_line(const point2_t& pStart, const point2_t& pEnd,
                    const rgba_t& pColor);

private:
    graphics_device& mDevice;
};

}

#endif // DRAW_IMMEDIATE_HH_INCLUDED
