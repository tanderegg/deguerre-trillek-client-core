#ifndef VERTEX_FORMAT_GL_HH_INCLUDED
#define VERTEX_FORMAT_GL_HH_INCLUDED

#include <primitive.hh>

namespace trillek {

struct vertex_format_gl : public vertex_format {
    vertex_format_gl(std::string pDescription)
        : vertex_format(pDescription)
    {
    }

    void update_device() {
    }
};

}


#endif // VERTEX_FORMAT_GL_HH_INCLUDED
