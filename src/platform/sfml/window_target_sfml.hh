#ifndef WINDOW_TARGET_SFML_HH_INCLUDED
#define WINDOW_TARGET_SFML_HH_INCLUDED

#include <platform_sfml.hh>
#include <render_target.hh>

namespace trillek {

    class window_sfml;
    class graphics_device_gl;

    class window_target_sfml : public window_target {
    public:
        window_target_sfml(const std::shared_ptr<window_sfml>& pWin,
		const std::shared_ptr<graphics_device_gl>& pDevice);

        virtual ~window_target_sfml();

        virtual format_t get_format() const;

        virtual vector2i_t get_dimensions();

        virtual void select();

        virtual void deselect();

        virtual void copy_to_texture(texture& pTex);

        virtual bool swap_buffers();

    private:
	std::weak_ptr<graphics_device_gl> mDevice;
	std::shared_ptr<window_sfml> mWinSfml;
    };

};


#endif // WINDOW_TARGET_SFML_HH_INCLUDED
