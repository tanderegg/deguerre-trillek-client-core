#include <subsystem.hh>
#include <platform_subsystem.hh>
#include <system_event.hh>
#include <graphics_subsystem.hh>
#include <graphics_device.hh>
#include <graphics_state.hh>
#include <draw_immediate.hh>
#include <keycodes.hh>
#include "player.hh"
#include <render_target.hh>
#include <primitive.hh>
#include <boost/random.hpp>
#include <iostream>
#include <vector3.hh>
#include <transform.hh>

#include <window_manager.hh>
#include <window.hh>

namespace trillek {
    extern platform_subsystem& get_platform_subsystem();
    extern system_event_queue& get_system_event_queue();
}


void dump_matrix4(const trillek::matrix4_t& pX) {
    const float_t* m = &pX.mM[0][0];
    std::cerr << '{';
    for (unsigned i = 0; i < 15; ++i) {
        std::cerr << ' ' << m[i] << ',';
    }
    std::cerr << ' ' << m[15] << " }\n";
}


void
load_subsystems(trillek::subsystem_manager& pMgr)
{
    using namespace trillek;
    pMgr.load(platform_subsystem::s_interface, get_platform_subsystem());
    pMgr.load(system_event_queue::s_interface, get_system_event_queue());
    pMgr.load(graphics_subsystem::s_interface, graphics_subsystem::get_graphics_subsystem());
}



struct milestone1 {
    trillek::subsystem_manager& mMgr;
    trillek::system_event_queue& mEvQueue;
    trillek::graphics_subsystem& mGraphics;
    trillek::platform_subsystem& mPlatform;

    std::shared_ptr<trillek::window> mMainWindow;
    std::shared_ptr<trillek::graphics_device> mDevice;
    std::shared_ptr<trillek::window_target> mTarget;

    std::shared_ptr<trillek::graphics_state> mBeautyPassState;
    std::shared_ptr<trillek::graphics_state> mHudState;

    bool mQuitEventPosted;

    std::shared_ptr<trillek::vertex_format> mVFormat;
    std::vector<std::unique_ptr<trillek::mesh>> mMeshes;

    trillek::float_t mRotation;

    milestone1(trillek::subsystem_manager& pMgr)
        : mMgr(pMgr),
          mEvQueue(mMgr.lookup<trillek::system_event_queue>()),
          mPlatform(mMgr.lookup<trillek::platform_subsystem>()),
          mGraphics(mMgr.lookup<trillek::graphics_subsystem>())
    {
        mQuitEventPosted = false;
        mRotation = 0;
    }

    void load_meshes();

    std::unique_ptr<trillek::mesh> build_mesh(float pGreyscale, uint32_t pBegin, uint32_t pEnd);

    void init() {
        mMainWindow = mPlatform.get_window_manager().get_main_window();
        mDevice = mGraphics.create_device();
        mTarget = mDevice->make_window_target(mMainWindow);
      
        mBeautyPassState = std::make_shared<trillek::graphics_state>();

        // Turn off depth test and depth write for HUD elements.
        mHudState = std::make_shared<trillek::graphics_state>();
        mHudState->mDepth.mFlags[trillek::D_ENABLE] = false;
        mHudState->mDepth.mFlags[trillek::D_WENABLE] = false;

        mDevice->set_graphics_state(mBeautyPassState);

        load_meshes();
    }

    bool quit_event_posted() const {
        return mQuitEventPosted;
    }

    void process_event(const trillek::system_event_t& pEvent);

    void process_events();

    void update() {
        using namespace trillek;

        uint32_t width, height;
        mMainWindow->get_dimensions(width, height);
        mDevice->set_viewport(recti_t{0, 0, width, height});

        camera c;
        c.update_projection_transform(recti_t(0,0,width,height),
               0.1f,1000.0f, 65.0f);
        c.update_camera_transform();
        // dump_matrix4(c.mProjectionXform);
        mDevice->projection_transform() = c.mProjectionXform;

        matrix4_t cam(c.mCameraXform);
        cam.translate(vector3_t(50,0,0));
        cam.rotate(mRotation, vector3_t(0,1,0));
        // dump_matrix4(cam);
        mDevice->camera_transform() = cam;

        matrix4_t model;
        // dump_matrix4(model);
        mDevice->model_transform() = model;

        matrix4_t mvp = mDevice->projection_transform();
        mvp *= mDevice->camera_transform();
        mvp *= mDevice->model_transform();
        // dump_matrix4(mvp);

        mRotation += 0.5;

        mDevice->update_state();
    }

    void hud_camera_begin() {
        mDevice->push_graphics_state();
        mDevice->set_graphics_state(mHudState);

        uint32_t width, height;
        mMainWindow->get_dimensions(width, height);

        mDevice->projection_transform() = trillek::ortho(
            0, width, 0, height, 0, 1
        );
        mDevice->camera_transform() = trillek::matrix4_t();
        mDevice->model_transform() = trillek::matrix4_t();
    }

    void hud_camera_end() {
        mDevice->pop_graphics_state();
    }

    void pre_shutdown() {
    }

    void frame();

    void draw_frame();
};


void
milestone1::process_events() {
    using namespace trillek;

    while (mEvQueue.more_events()) {
        system_event_t ev = mEvQueue.get();
        process_event(ev);
        if (ev.mType == EV_KEY && ev.mSubtype == EV_K_DOWN
                && ev.mData == K_ESCAPE) {
            mQuitEventPosted = true;
        }
    }
}

void
milestone1::process_event(const trillek::system_event_t& pEvent) {
    using namespace trillek;
    using namespace std;

    switch (pEvent.mType) {
        case EV_MOUSE: {
            cerr << "Mouse ";
            switch (pEvent.mSubtype) {
                case EV_M_DX: {
                    cerr << "dx";
                    break;
                }
                case EV_M_DY: {
                    cerr << "dy";
                    break;
                }
                case EV_M_DZ: {
                    cerr << "dz";
                    break;
                }
                default: {
                    cerr << "(unknown)";
                    break;
                }
            }
            cerr << ' ' << pEvent.mData << '\n';
            break;
        }

        case EV_KEY: {
            cerr << "Key ";
            switch (pEvent.mSubtype) {
                case EV_K_DOWN: {
                    cerr << "down";
                    break;
                }
                case EV_K_UP: {
                    cerr << "up";
                    break;
                }
                default: {
                    cerr << "(unknown)";
                    break;
                }
            }
            cerr << ' ' << pEvent.mData << '\n';
            break;
        }

        default: {
            cerr << "Unknown system event\n";
            break;
        }
    }
}


void
milestone1::frame() {
    mPlatform.frame();
    process_events();
    draw_frame();
}


void
milestone1::draw_frame() {
    using namespace trillek;
    mDevice->begin_frame();
    mDevice->set_render_target(mTarget);
    update();
    mDevice->clear(CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL,
        rgba_t(0.5f,0.2f,0.2f,1.0f), 1.0f, 0xffu);

    for (auto& m: mMeshes) {
        m->draw();
    }

#if 0
    hud_camera_begin();
    draw_immediate& imm = mDevice->get_draw_immediate();
    imm.fill_rect(point2_t(10,10), point2_t(50,50), rgba_t(1,1,1,0.5));
    hud_camera_end();
#endif

    mDevice->end_frame();
    mTarget->swap_buffers();
}


#if 1
static trillek::point3_t sVertices[] = {
    trillek::point3_t{-10, 20, -7.5511},
    trillek::point3_t{-10, 17.5511, -10},
    trillek::point3_t{10, 17.5511, -10},
    trillek::point3_t{10, 20, -7.5511},
    trillek::point3_t{10, 8.06932e-007, -10},
    trillek::point3_t{-10, 8.06932e-007, -10},
    trillek::point3_t{-10, 17.5511, 10},
    trillek::point3_t{-10, 20, 7.5511},
    trillek::point3_t{10, 20, 7.5511},
    trillek::point3_t{10, 17.5511, 10},
    trillek::point3_t{10, 8.06932e-007, 10},
    trillek::point3_t{-10, 8.06932e-007, 10},
    trillek::point3_t{-3.31472, 17.5511, 10},
    trillek::point3_t{-3.31472, 20, 7.5511},
    trillek::point3_t{-3.31472, 20, -7.5511},
    trillek::point3_t{-3.31472, 17.5511, -10},
    trillek::point3_t{-3.31472, 8.06932e-007, -10},
    trillek::point3_t{-3.31472, 8.06932e-007, 10},
    trillek::point3_t{-3.31472, 17.5511, 10},
    trillek::point3_t{-1.00407, 17.5511, 10},
    trillek::point3_t{-1.00407, 8.06932e-007, 10},
    trillek::point3_t{-1.00407, 8.06932e-007, -10},
    trillek::point3_t{-1.00407, 17.5511, -10},
    trillek::point3_t{-1.00407, 20, -7.5511},
    trillek::point3_t{-1.00407, 20, 7.5511},
    trillek::point3_t{-1.00407, 17.5511, 10},
    trillek::point3_t{1.00811, 17.5511, 10},
    trillek::point3_t{1.00811, 8.06932e-007, 10},
    trillek::point3_t{1.00811, 8.06932e-007, -10},
    trillek::point3_t{1.00811, 17.5511, -10},
    trillek::point3_t{1.00811, 20, -7.5511},
    trillek::point3_t{1.00811, 20, 7.5511},
    trillek::point3_t{1.00811, 17.5511, 10},
    trillek::point3_t{2.78463, 17.5511, 10},
    trillek::point3_t{2.78463, 8.06932e-007, 10},
    trillek::point3_t{2.78463, 8.06932e-007, -10},
    trillek::point3_t{2.78463, 17.5511, -10},
    trillek::point3_t{2.78463, 20, -7.5511},
    trillek::point3_t{2.78463, 20, 7.5511},
    trillek::point3_t{2.78463, 17.5511, 10},
    trillek::point3_t{4.1786, 17.5511, 10},
    trillek::point3_t{4.1786, 20, 7.5511},
    trillek::point3_t{4.1786, 20, -7.5511},
    trillek::point3_t{4.1786, 17.5511, -10},
    trillek::point3_t{4.1786, 8.06932e-007, -10},
    trillek::point3_t{4.1786, 8.06932e-007, 10},
    trillek::point3_t{-4.5412, 17.5511, 10},
    trillek::point3_t{-4.5412, 20, 7.5511},
    trillek::point3_t{-4.5412, 20, -7.5511},
    trillek::point3_t{-4.5412, 17.5511, -10},
    trillek::point3_t{-4.5412, 8.06932e-007, -10},
    trillek::point3_t{-4.5412, 8.06932e-007, 10},
    trillek::point3_t{-4.5412, 17.5511, 10},
    trillek::point3_t{-2.10063, 17.5511, 10},
    trillek::point3_t{-2.10063, 8.06932e-007, 10},
    trillek::point3_t{-2.10063, 8.06932e-007, -10},
    trillek::point3_t{-2.10063, 17.5511, -10},
    trillek::point3_t{-2.10063, 20, -7.5511},
    trillek::point3_t{-2.10063, 20, 7.5511},
    trillek::point3_t{-2.10063, 17.5511, 10},
    trillek::point3_t{-5.52869, 17.5511, 10},
    trillek::point3_t{-5.52869, 8.06932e-007, 10},
    trillek::point3_t{-5.52869, 8.06932e-007, -10},
    trillek::point3_t{-5.52869, 17.5511, -10},
    trillek::point3_t{-5.52869, 20, -7.5511},
    trillek::point3_t{-5.52869, 20, 7.5511},
    trillek::point3_t{-5.52869, 17.5511, 10},
    trillek::point3_t{-6.80106, 17.5511, 10},
    trillek::point3_t{-6.80105, 20, 7.5511},
    trillek::point3_t{-6.80105, 20, -7.5511},
    trillek::point3_t{-6.80105, 17.5511, -10},
    trillek::point3_t{-6.80105, 8.06932e-007, -10},
    trillek::point3_t{-6.80105, 8.06932e-007, 10},
    trillek::point3_t{-6.80105, 17.5511, 10},
    trillek::point3_t{-8.00141, 17.5511, 10},
    trillek::point3_t{-8.00141, 8.06932e-007, 10},
    trillek::point3_t{-8.00141, 8.06932e-007, -10},
    trillek::point3_t{-8.00141, 17.5511, -10},
    trillek::point3_t{-8.00141, 20, -7.5511},
    trillek::point3_t{-8.00141, 20, 7.5511},
    trillek::point3_t{-8.00141, 17.5511, 10},
    trillek::point3_t{5.73518, 17.5511, 10},
    trillek::point3_t{5.73518, 20, 7.5511},
    trillek::point3_t{5.73518, 20, -7.5511},
    trillek::point3_t{5.73518, 17.5511, -10},
    trillek::point3_t{5.73518, 8.06932e-007, -10},
    trillek::point3_t{5.73518, 8.06932e-007, 10},
    trillek::point3_t{7.24944, 17.5511, 10},
    trillek::point3_t{7.24944, 20, 7.5511},
    trillek::point3_t{7.24944, 20, -7.5511},
    trillek::point3_t{7.24944, 17.5511, -10},
    trillek::point3_t{7.24944, 8.06932e-007, -10},
    trillek::point3_t{7.24944, 8.06932e-007, 10},
    trillek::point3_t{8.425, 17.5511, 10},
    trillek::point3_t{8.425, 20, 7.5511},
    trillek::point3_t{8.425, 20, -7.5511},
    trillek::point3_t{8.425, 17.5511, -10},
    trillek::point3_t{8.425, 8.06932e-007, -10},
    trillek::point3_t{8.425, 8.06932e-007, 10},
    trillek::point3_t{-1.00407, 20, 3.38186},
    trillek::point3_t{1.00811, 20, 3.38186},
    trillek::point3_t{2.78463, 20, 3.38186},
    trillek::point3_t{4.1786, 20, 3.38186},
    trillek::point3_t{5.73518, 20, 3.38186},
    trillek::point3_t{7.24944, 20, 3.38186},
    trillek::point3_t{8.425, 20, 3.38186},
    trillek::point3_t{10, 20, 3.38186},
    trillek::point3_t{10, 2.6753e-006, 2.95273},
    trillek::point3_t{8.425, 2.6753e-006, 2.95273},
    trillek::point3_t{7.24944, 2.6753e-006, 2.95273},
    trillek::point3_t{5.73518, 2.6753e-006, 2.95273},
    trillek::point3_t{4.1786, 2.6753e-006, 2.95273},
    trillek::point3_t{2.78463, 2.6753e-006, 2.95273},
    trillek::point3_t{1.00811, 2.6753e-006, 2.95273},
    trillek::point3_t{-1.00407, 2.6753e-006, 2.95273},
    trillek::point3_t{-2.10063, 2.6753e-006, 2.95273},
    trillek::point3_t{-3.31472, 2.6753e-006, 2.95273},
    trillek::point3_t{-4.5412, 2.6753e-006, 2.95273},
    trillek::point3_t{-5.52869, 2.6753e-006, 2.95273},
    trillek::point3_t{-6.80105, 2.6753e-006, 2.95273},
    trillek::point3_t{-8.00141, 2.6753e-006, 2.95273},
    trillek::point3_t{-10, 2.6753e-006, 2.95273},
    trillek::point3_t{-10, 20, 3.38186},
    trillek::point3_t{-8.00141, 20, 3.38186},
    trillek::point3_t{-6.80105, 20, 3.38186},
    trillek::point3_t{-5.52869, 20, 3.38186},
    trillek::point3_t{-4.5412, 20, 3.38186},
    trillek::point3_t{-3.31472, 20, 3.38186},
    trillek::point3_t{-2.10063, 20, 3.38186},
    trillek::point3_t{-1.00407, 20, -1.0445},
    trillek::point3_t{1.00811, 20, -1.0445},
    trillek::point3_t{2.78463, 20, -1.0445},
    trillek::point3_t{4.1786, 20, -1.0445},
    trillek::point3_t{5.73518, 20, -1.0445},
    trillek::point3_t{7.24944, 20, -1.0445},
    trillek::point3_t{8.425, 20, -1.0445},
    trillek::point3_t{10, 20, -1.0445},
    trillek::point3_t{10, 8.06932e-007, -1.38325},
    trillek::point3_t{8.425, 8.06932e-007, -1.38325},
    trillek::point3_t{7.24944, 8.06932e-007, -1.38325},
    trillek::point3_t{5.73518, 8.06932e-007, -1.38325},
    trillek::point3_t{4.1786, 8.06932e-007, -1.38325},
    trillek::point3_t{2.78463, 8.06932e-007, -1.38325},
    trillek::point3_t{1.00811, 8.06932e-007, -1.38325},
    trillek::point3_t{-1.00407, 8.06932e-007, -1.38325},
    trillek::point3_t{-2.10063, 8.06932e-007, -1.38325},
    trillek::point3_t{-3.31472, 8.06932e-007, -1.38325},
    trillek::point3_t{-4.5412, 8.06932e-007, -1.38325},
    trillek::point3_t{-5.52869, 8.06932e-007, -1.38325},
    trillek::point3_t{-6.80105, 8.06932e-007, -1.38325},
    trillek::point3_t{-8.00141, 8.06932e-007, -1.38325},
    trillek::point3_t{-10, 8.06932e-007, -1.38325},
    trillek::point3_t{-10, 20, -1.0445},
    trillek::point3_t{-8.00141, 20, -1.0445},
    trillek::point3_t{-6.80105, 20, -1.0445},
    trillek::point3_t{-5.52869, 20, -1.0445},
    trillek::point3_t{-4.5412, 20, -1.0445},
    trillek::point3_t{-3.31472, 20, -1.0445},
    trillek::point3_t{-2.10063, 20, -1.0445},
    trillek::point3_t{-10, 13.7777, 10},
    trillek::point3_t{-8.00141, 13.7777, 10},
    trillek::point3_t{-6.80105, 13.7777, 10},
    trillek::point3_t{-5.52869, 13.7777, 10},
    trillek::point3_t{-4.5412, 13.7777, 10},
    trillek::point3_t{-3.31472, 13.7777, 10},
    trillek::point3_t{-2.10063, 13.7777, 10},
    trillek::point3_t{-1.00407, 13.7777, 10},
    trillek::point3_t{1.00811, 13.7777, 10},
    trillek::point3_t{2.78463, 13.7777, 10},
    trillek::point3_t{4.1786, 13.7777, 10},
    trillek::point3_t{5.73518, 13.7777, 10},
    trillek::point3_t{7.24944, 13.7777, 10},
    trillek::point3_t{8.425, 13.7777, 10},
    trillek::point3_t{10, 13.7777, 10},
    trillek::point3_t{10, 13.7595, 3.2896},
    trillek::point3_t{10, 13.7595, -1.11733},
    trillek::point3_t{10, 13.7777, -10},
    trillek::point3_t{8.425, 13.7777, -10},
    trillek::point3_t{7.24944, 13.7777, -10},
    trillek::point3_t{5.73518, 13.7777, -10},
    trillek::point3_t{4.1786, 13.7777, -10},
    trillek::point3_t{2.78463, 13.7777, -10},
    trillek::point3_t{1.00811, 13.7777, -10},
    trillek::point3_t{-1.00407, 13.7777, -10},
    trillek::point3_t{-2.10063, 13.7777, -10},
    trillek::point3_t{-3.31472, 13.7777, -10},
    trillek::point3_t{-4.5412, 13.7777, -10},
    trillek::point3_t{-5.52869, 13.7777, -10},
    trillek::point3_t{-6.80105, 13.7777, -10},
    trillek::point3_t{-8.00141, 13.7777, -10},
    trillek::point3_t{-10, 13.7777, -10},
    trillek::point3_t{-10, 13.8204, -1.11733},
    trillek::point3_t{-10, 13.8204, 3.2896},
    trillek::point3_t{-10, 5.57799, 10},
    trillek::point3_t{-8.00141, 5.57799, 10},
    trillek::point3_t{-6.80105, 5.57799, 10},
    trillek::point3_t{-5.52869, 5.57799, 10},
    trillek::point3_t{-4.5412, 5.57799, 10},
    trillek::point3_t{-3.31472, 5.57799, 10},
    trillek::point3_t{-2.10063, 5.57799, 10},
    trillek::point3_t{-1.00407, 5.57799, 10},
    trillek::point3_t{1.00811, 5.57799, 10},
    trillek::point3_t{2.78463, 5.57799, 10},
    trillek::point3_t{4.1786, 5.57799, 10},
    trillek::point3_t{5.73518, 5.57799, 10},
    trillek::point3_t{7.24944, 5.57799, 10},
    trillek::point3_t{8.425, 5.57799, 10},
    trillek::point3_t{10, 5.57799, 10},
    trillek::point3_t{10, 5.59624, 3.08911},
    trillek::point3_t{10, 5.59624, -1.27559},
    trillek::point3_t{10, 5.57799, -10},
    trillek::point3_t{8.425, 5.57799, -10},
    trillek::point3_t{7.24944, 5.57799, -10},
    trillek::point3_t{5.73518, 5.57799, -10},
    trillek::point3_t{4.1786, 5.57799, -10},
    trillek::point3_t{2.78463, 5.57799, -10},
    trillek::point3_t{1.00811, 5.57799, -10},
    trillek::point3_t{-1.00407, 5.57799, -10},
    trillek::point3_t{-2.10063, 5.57799, -10},
    trillek::point3_t{-3.31472, 5.57799, -10},
    trillek::point3_t{-4.5412, 5.57799, -10},
    trillek::point3_t{-5.52869, 5.57799, -10},
    trillek::point3_t{-6.80105, 5.57799, -10},
    trillek::point3_t{-8.00141, 5.57799, -10},
    trillek::point3_t{-10, 5.57799, -10},
    trillek::point3_t{-10, 5.59624, -1.27559},
    trillek::point3_t{-10, 5.59624, 3.08911},
    trillek::point3_t{10, 10.6648, 10},
    trillek::point3_t{8.425, 10.6648, 10},
    trillek::point3_t{7.24944, 10.6648, 10},
    trillek::point3_t{5.73518, 10.6648, 10},
    trillek::point3_t{4.1786, 10.6648, 10},
    trillek::point3_t{2.78463, 10.6648, 10},
    trillek::point3_t{1.00811, 10.6648, 10},
    trillek::point3_t{-1.00407, 10.6648, 10},
    trillek::point3_t{-2.10063, 10.6648, 10},
    trillek::point3_t{-3.31472, 10.6648, 10},
    trillek::point3_t{-4.5412, 10.6648, 10},
    trillek::point3_t{-5.52869, 10.6648, 10},
    trillek::point3_t{-6.80105, 10.6648, 10},
    trillek::point3_t{-8.00141, 10.6648, 10},
    trillek::point3_t{-10, 10.6648, 10},
    trillek::point3_t{-10, 10.6349, 3.21349},
    trillek::point3_t{-10, 10.6349, -1.17741},
    trillek::point3_t{-10, 10.6648, -10},
    trillek::point3_t{-8.00141, 10.6648, -10},
    trillek::point3_t{-6.80105, 10.6648, -10},
    trillek::point3_t{-5.52869, 10.6648, -10},
    trillek::point3_t{-4.5412, 10.6648, -10},
    trillek::point3_t{-3.31472, 10.6648, -10},
    trillek::point3_t{-2.10063, 10.6648, -10},
    trillek::point3_t{-1.00407, 10.6648, -10},
    trillek::point3_t{1.00811, 10.6648, -10},
    trillek::point3_t{2.78463, 10.6648, -10},
    trillek::point3_t{4.1786, 10.6648, -10},
    trillek::point3_t{5.73518, 10.6648, -10},
    trillek::point3_t{7.24944, 10.6648, -10},
    trillek::point3_t{8.425, 10.6648, -10},
    trillek::point3_t{10, 10.6648, -10},
    trillek::point3_t{10, 10.6349, -1.17741},
    trillek::point3_t{10, 10.6349, 3.21349},
    trillek::point3_t{1.00539, 19.5842, 3.37795},
    trillek::point3_t{-1.00324, 19.5842, 3.37795},
    trillek::point3_t{-2.09786, 19.5842, 3.37795},
    trillek::point3_t{2.77876, 19.5842, 3.37795},
    trillek::point3_t{4.17027, 19.5842, 3.37795},
    trillek::point3_t{5.7241, 19.5842, 3.37795},
    trillek::point3_t{5.7241, 19.5842, -1.04059},
    trillek::point3_t{-5.51985, 19.5842, 3.37795},
    trillek::point3_t{-6.78997, 19.5842, 3.37795},
    trillek::point3_t{-6.78997, 19.5842, -1.04059},
    trillek::point3_t{-4.53411, 19.5842, 3.37795},
    trillek::point3_t{-3.3098, 19.5842, 3.37795},
    trillek::point3_t{-1.00324, 19.5842, -1.04059},
    trillek::point3_t{1.00539, 19.5842, -1.04059},
    trillek::point3_t{-2.09786, 19.5842, -1.04059},
    trillek::point3_t{2.77876, 19.5842, -1.04059},
    trillek::point3_t{4.17027, 19.5842, -1.04059},
    trillek::point3_t{-5.51985, 19.5842, -1.04059},
    trillek::point3_t{-4.53411, 19.5842, -1.04059},
    trillek::point3_t{-3.3098, 19.5842, -1.04059},
    trillek::point3_t{1.23018, 18.9954, 3.70078},
    trillek::point3_t{-1.07196, 18.9954, 3.70078},
    trillek::point3_t{3.26269, 18.9954, 3.70078},
    trillek::point3_t{-2.32654, 18.9954, 3.70078},
    trillek::point3_t{-3.71558, 18.9954, 3.70078},
    trillek::point3_t{4.85753, 18.9954, 3.70078},
    trillek::point3_t{6.63842, 18.9954, 3.70078},
    trillek::point3_t{6.63842, 18.9954, -1.36342},
    trillek::point3_t{4.85753, 18.9954, -1.36342},
    trillek::point3_t{-6.24858, 18.9954, 3.70078},
    trillek::point3_t{-7.7043, 18.9954, 3.70078},
    trillek::point3_t{-5.11879, 18.9954, 3.70078},
    trillek::point3_t{-7.7043, 18.9954, -1.36342},
    trillek::point3_t{-6.24858, 18.9954, -1.36342},
    trillek::point3_t{-1.07196, 18.9954, -1.36342},
    trillek::point3_t{1.23018, 18.9954, -1.36342},
    trillek::point3_t{-2.32654, 18.9954, -1.36342},
    trillek::point3_t{3.26269, 18.9954, -1.36342},
    trillek::point3_t{-3.71558, 18.9954, -1.36342},
    trillek::point3_t{-5.11879, 18.9954, -1.36342},
    trillek::point3_t{-2.33877, 19.4262, 3.71805},
    trillek::point3_t{-3.73729, 19.4262, 3.71805},
    trillek::point3_t{-5.15008, 19.4262, 3.71805},
    trillek::point3_t{-6.28757, 19.4262, 3.71805},
    trillek::point3_t{-7.75322, 19.4262, 3.71805},
    trillek::point3_t{-7.75322, 19.4262, -1.3807},
    trillek::point3_t{-6.28757, 19.4262, -1.3807},
    trillek::point3_t{-5.15008, 19.4262, -1.3807},
    trillek::point3_t{-3.73729, 19.4262, -1.3807},
    trillek::point3_t{-2.33877, 19.4262, -1.3807},
    trillek::point3_t{-1.07564, 19.4262, -1.3807},
    trillek::point3_t{1.24221, 19.4262, -1.3807},
    trillek::point3_t{3.28859, 19.4262, -1.3807},
    trillek::point3_t{4.89431, 19.4262, -1.3807},
    trillek::point3_t{6.68734, 19.4262, -1.3807},
    trillek::point3_t{6.68734, 19.4262, 3.71805},
    trillek::point3_t{4.89431, 19.4262, 3.71805},
    trillek::point3_t{3.28859, 19.4262, 3.71805},
    trillek::point3_t{1.24221, 19.4262, 3.71805},
    trillek::point3_t{-1.07564, 19.4262, 3.71805}
};


static unsigned sFaces[] = {
    24, 31, 30, 23, 0,
    26, 33, 32, 25, 0,
    31, 38, 37, 30, 0,
    33, 40, 39, 32, 0,
    38, 43, 44, 37, 0,
    40, 41, 42, 39, 0,
    49, 15, 16, 50, 0,
    47, 13, 14, 48, 0,
    15, 58, 57, 16, 0,
    58, 24, 23, 57, 0,
    13, 60, 59, 14, 0,
    60, 26, 25, 59, 0,
    65, 49, 50, 64, 0,
    67, 47, 48, 66, 0,
    70, 65, 64, 71, 0,
    68, 67, 66, 69, 0,
    1, 79, 78, 2, 0,
    79, 70, 71, 78, 0,
    7, 81, 80, 8, 0,
    81, 68, 69, 80, 0,
    43, 84, 85, 44, 0,
    41, 82, 83, 42, 0,
    84, 90, 91, 85, 0,
    82, 88, 89, 83, 0,
    90, 96, 97, 91, 0,
    96, 4, 3, 97, 0,
    88, 94, 95, 89, 0,
    94, 10, 9, 95, 0,
    25, 32, 101, 100, 0,
    28, 21, 115, 114, 0,
    32, 39, 102, 101, 0,
    35, 28, 114, 113, 0,
    39, 42, 103, 102, 0,
    46, 35, 113, 112, 0,
    48, 14, 128, 127, 0,
    18, 52, 118, 117, 0,
    14, 59, 129, 128, 0,
    59, 25, 100, 129, 0,
    21, 55, 116, 115, 0,
    55, 18, 117, 116, 0,
    66, 48, 127, 126, 0,
    52, 62, 119, 118, 0,
    69, 66, 126, 125, 0,
    62, 73, 120, 119, 0,
    123, 8, 80, 124, 0,
    80, 69, 125, 124, 0,
    73, 76, 121, 120, 0,
    76, 12, 122, 121, 0,
    42, 83, 104, 103, 0,
    87, 46, 112, 111, 0,
    83, 89, 105, 104, 0,
    93, 87, 111, 110, 0,
    89, 95, 106, 105, 0,
    95, 9, 107, 106, 0,
    99, 93, 110, 109, 0,
    282, 297, 296, 283, 0,
    131, 31, 24, 130, 0,
    115, 145, 144, 114, 0,
    145, 22, 29, 144, 0,
    284, 299, 297, 282, 0,
    132, 38, 31, 131, 0,
    114, 144, 143, 113, 0,
    144, 29, 36, 143, 0,
    287, 290, 299, 284, 0,
    133, 43, 38, 132, 0,
    113, 143, 142, 112, 0,
    143, 36, 45, 142, 0,
    286, 300, 301, 293, 0,
    158, 15, 49, 157, 0,
    118, 148, 147, 117, 0,
    148, 51, 17, 147, 0,
    285, 298, 300, 286, 0,
    159, 58, 15, 158, 0,
    283, 296, 298, 285, 0,
    130, 24, 58, 159, 0,
    116, 146, 145, 115, 0,
    146, 56, 22, 145, 0,
    117, 147, 146, 116, 0,
    147, 17, 56, 146, 0,
    293, 301, 295, 291, 0,
    157, 49, 65, 156, 0,
    119, 149, 148, 118, 0,
    149, 63, 51, 148, 0,
    291, 295, 294, 292, 0,
    156, 65, 70, 155, 0,
    120, 150, 149, 119, 0,
    150, 72, 63, 149, 0,
    1, 153, 154, 79, 0,
    153, 123, 124, 154, 0,
    125, 155, 154, 124, 0,
    155, 70, 79, 154, 0,
    121, 151, 150, 120, 0,
    151, 77, 72, 150, 0,
    122, 152, 151, 121, 0,
    152, 6, 77, 151, 0,
    288, 289, 290, 287, 0,
    134, 84, 43, 133, 0,
    112, 142, 141, 111, 0,
    142, 45, 86, 141, 0,
    105, 135, 134, 104, 0,
    135, 90, 84, 134, 0,
    111, 141, 140, 110, 0,
    141, 86, 92, 140, 0,
    106, 136, 135, 105, 0,
    136, 96, 90, 135, 0,
    137, 4, 96, 136, 0,
    138, 108, 109, 139, 0,
    110, 140, 139, 109, 0,
    108, 11, 99, 109, 0,
    92, 98, 139, 0,
    140, 92, 139, 0,
    107, 136, 106, 0,
    107, 137, 136, 0,
    5, 138, 139, 98, 0,
    23, 30, 183, 184, 0,
    27, 20, 167, 168, 0,
    30, 37, 182, 183, 0,
    34, 27, 168, 169, 0,
    37, 44, 181, 182, 0,
    41, 34, 169, 170, 0,
    50, 16, 186, 187, 0,
    19, 53, 164, 165, 0,
    16, 57, 185, 186, 0,
    57, 23, 184, 185, 0,
    20, 54, 166, 167, 0,
    54, 19, 165, 166, 0,
    64, 50, 187, 188, 0,
    53, 61, 163, 164, 0,
    71, 64, 188, 189, 0,
    61, 74, 162, 163, 0,
    191, 2, 78, 190, 0,
    78, 71, 189, 190, 0,
    74, 75, 161, 162, 0,
    75, 7, 160, 161, 0,
    44, 85, 180, 181, 0,
    82, 41, 170, 171, 0,
    85, 91, 179, 180, 0,
    88, 82, 171, 172, 0,
    91, 97, 178, 179, 0,
    97, 3, 177, 178, 0,
    174, 10, 94, 173, 0,
    94, 88, 172, 173, 0,
    107, 9, 10, 174, 175, 0,
    160, 7, 8, 123, 193, 0,
    3, 4, 137, 176, 177, 0,
    192, 153, 1, 2, 191, 0,
    193, 123, 153, 192, 0,
    137, 107, 175, 176, 0,
    217, 29, 22, 218, 0,
    201, 21, 28, 202, 0,
    216, 36, 29, 217, 0,
    202, 28, 35, 203, 0,
    215, 45, 36, 216, 0,
    203, 35, 46, 204, 0,
    220, 17, 51, 221, 0,
    198, 52, 18, 199, 0,
    219, 56, 17, 220, 0,
    218, 22, 56, 219, 0,
    200, 55, 21, 201, 0,
    199, 18, 55, 200, 0,
    221, 51, 63, 222, 0,
    197, 62, 52, 198, 0,
    222, 63, 72, 223, 0,
    196, 73, 62, 197, 0,
    6, 225, 224, 77, 0,
    223, 72, 77, 224, 0,
    195, 76, 73, 196, 0,
    194, 12, 76, 195, 0,
    214, 86, 45, 215, 0,
    204, 46, 87, 205, 0,
    213, 92, 86, 214, 0,
    205, 87, 93, 206, 0,
    212, 98, 92, 213, 0,
    211, 5, 98, 212, 0,
    11, 208, 207, 99, 0,
    206, 93, 99, 207, 0,
    208, 11, 108, 209, 0,
    122, 12, 194, 227, 0,
    210, 138, 5, 211, 0,
    6, 152, 226, 225, 0,
    152, 122, 227, 226, 0,
    209, 108, 138, 210, 0,
    183, 253, 252, 184, 0,
    253, 217, 218, 252, 0,
    167, 235, 234, 168, 0,
    235, 201, 202, 234, 0,
    182, 254, 253, 183, 0,
    254, 216, 217, 253, 0,
    168, 234, 233, 169, 0,
    234, 202, 203, 233, 0,
    181, 255, 254, 182, 0,
    255, 215, 216, 254, 0,
    169, 233, 232, 170, 0,
    233, 203, 204, 232, 0,
    186, 250, 249, 187, 0,
    250, 220, 221, 249, 0,
    164, 238, 237, 165, 0,
    238, 198, 199, 237, 0,
    185, 251, 250, 186, 0,
    251, 219, 220, 250, 0,
    184, 252, 251, 185, 0,
    252, 218, 219, 251, 0,
    166, 236, 235, 167, 0,
    236, 200, 201, 235, 0,
    165, 237, 236, 166, 0,
    237, 199, 200, 236, 0,
    187, 249, 248, 188, 0,
    249, 221, 222, 248, 0,
    163, 239, 238, 164, 0,
    239, 197, 198, 238, 0,
    188, 248, 247, 189, 0,
    248, 222, 223, 247, 0,
    162, 240, 239, 163, 0,
    240, 196, 197, 239, 0,
    225, 245, 246, 224, 0,
    245, 191, 190, 246, 0,
    189, 247, 246, 190, 0,
    247, 223, 224, 246, 0,
    161, 241, 240, 162, 0,
    241, 195, 196, 240, 0,
    160, 242, 241, 161, 0,
    242, 194, 195, 241, 0,
    180, 256, 255, 181, 0,
    256, 214, 215, 255, 0,
    170, 232, 231, 171, 0,
    232, 204, 205, 231, 0,
    179, 257, 256, 180, 0,
    257, 213, 214, 256, 0,
    171, 231, 230, 172, 0,
    231, 205, 206, 230, 0,
    178, 258, 257, 179, 0,
    258, 212, 213, 257, 0,
    177, 259, 258, 178, 0,
    259, 211, 212, 258, 0,
    208, 228, 229, 207, 0,
    228, 174, 173, 229, 0,
    172, 230, 229, 173, 0,
    230, 206, 207, 229, 0,
    174, 228, 261, 175, 0,
    228, 208, 209, 261, 0,
    194, 242, 243, 227, 0,
    242, 160, 193, 243, 0,
    176, 260, 259, 177, 0,
    260, 210, 211, 259, 0,
    226, 244, 245, 225, 0,
    244, 192, 191, 245, 0,
    227, 243, 244, 226, 0,
    243, 193, 192, 244, 0,
    175, 261, 260, 176, 0,
    261, 209, 210, 260, 0,
    129, 264, 273, 128, 0,
    128, 273, 272, 127, 0,
    127, 272, 269, 126, 0,
    126, 269, 270, 125, 0,
    125, 270, 271, 155, 0,
    155, 271, 279, 156, 0,
    156, 279, 280, 157, 0,
    157, 280, 281, 158, 0,
    158, 281, 276, 159, 0,
    159, 276, 274, 130, 0,
    130, 274, 275, 131, 0,
    131, 275, 277, 132, 0,
    132, 277, 278, 133, 0,
    133, 278, 268, 134, 0,
    134, 268, 267, 104, 0,
    104, 267, 266, 103, 0,
    103, 266, 265, 102, 0,
    102, 265, 262, 101, 0,
    101, 262, 263, 100, 0,
    100, 263, 264, 129, 0,
    264, 302, 303, 273, 0,
    273, 303, 304, 272, 0,
    272, 304, 305, 269, 0,
    269, 305, 306, 270, 0,
    270, 306, 307, 271, 0,
    271, 307, 308, 279, 0,
    279, 308, 309, 280, 0,
    280, 309, 310, 281, 0,
    281, 310, 311, 276, 0,
    276, 311, 312, 274, 0,
    274, 312, 313, 275, 0,
    275, 313, 314, 277, 0,
    277, 314, 315, 278, 0,
    278, 315, 316, 268, 0,
    268, 316, 317, 267, 0,
    267, 317, 318, 266, 0,
    266, 318, 319, 265, 0,
    265, 319, 320, 262, 0,
    262, 320, 321, 263, 0,
    263, 321, 302, 264, 0,
    302, 285, 286, 303, 0,
    303, 286, 293, 304, 0,
    304, 293, 291, 305, 0,
    305, 291, 292, 306, 0,
    306, 292, 294, 307, 0,
    307, 294, 295, 308, 0,
    308, 295, 301, 309, 0,
    309, 301, 300, 310, 0,
    310, 300, 298, 311, 0,
    311, 298, 296, 312, 0,
    312, 296, 297, 313, 0,
    313, 297, 299, 314, 0,
    314, 299, 290, 315, 0,
    315, 290, 289, 316, 0,
    316, 289, 288, 317, 0,
    317, 288, 287, 318, 0,
    318, 287, 284, 319, 0,
    319, 284, 282, 320, 0,
    320, 282, 283, 321, 0,
    321, 283, 285, 302, 0
};
#endif


std::unique_ptr<trillek::mesh>
milestone1::build_mesh(float pGreyscale, uint32_t pBegin, uint32_t pEnd) {
    using namespace trillek;
    using namespace std;

    unique_ptr<trillek::mesh> m
        = mDevice->make_mesh(PRIM_POLYGON, mVFormat, pEnd - pBegin, BUFFER_STATIC);

    // Normals not attached to this mesh, unfortunately.
    point3_t& p0 = sVertices[sFaces[pBegin + 0] - 1];
    point3_t& p1 = sVertices[sFaces[pBegin + 1] - 1];
    point3_t& p2 = sVertices[sFaces[pBegin + 2] - 1];
    vector3_t n = (p0 - p1) ^ (p2 - p1);
    n.normalize();

    {
        mesh_builder b(*m);
        for (uint32_t i = pBegin; i < pEnd; ++i) {
            uint32_t v = sFaces[i] - 1;

            b.position(sVertices[v]);
            b.normal(n);
            b.color(pGreyscale, pGreyscale, pGreyscale);
            b.advance();
        }
    }

    return m;
}


void
milestone1::load_meshes() {
    using namespace trillek;

    mVFormat = mDevice->make_vertex_format("PNC");
    mVFormat->add_element(VERTDATA_POSITION, VERTDATA_TYPE_FLOAT3);
    mVFormat->add_element(VERTDATA_NORMAL, VERTDATA_TYPE_FLOAT3);
    mVFormat->add_element(VERTDATA_COLOR, VERTDATA_TYPE_BYTE4);

    boost::mt19937 rng;
    boost::uniform_01<boost::mt19937> unif(rng);

    uint32_t b = 0;
    for (unsigned i = 0; i < sizeof(sFaces) / sizeof(sFaces[0]); ++i) {
        if (!sFaces[i]) {
            mMeshes.push_back(build_mesh(unif() * 0.5 + 0.5, b, i));
            b = i + 1;
        }
    }
}



int
main(int argc, char* argv[]) {
    using namespace trillek;

    subsystem_manager& mgr = standard_subsystem_manager();
    load_subsystems(mgr);
    mgr.initialise();

    milestone1 m1(mgr);

    m1.init();

    while (!m1.quit_event_posted()) {
        m1.frame();
    }
}




