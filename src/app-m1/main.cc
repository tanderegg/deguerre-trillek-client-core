#include <subsystem.hh>
#include <graphics_driver.hh>
#include <input_driver.hh>
#include <system_event.hh>
#include <keycodes.hh>
#include <trillek_gl.hh>
#include <iostream>

namespace trillek {
    extern graphics_driver& get_graphics_driver();
    extern input_driver& get_input_driver();
    extern system_event_queue& get_system_event_queue();
}


void
load_subsystems(trillek::subsystem_manager& pMgr)
{
    using namespace trillek;
    std::cerr << "Graphics driver interface = " << graphics_driver::s_interface << '\n';
    std::cerr << "Input driver interface = " << input_driver::s_interface << '\n';
    std::cerr << "System event queue interface = " << system_event_queue::s_interface << '\n';
    pMgr.load(graphics_driver::s_interface, get_graphics_driver());
    pMgr.load(input_driver::s_interface, get_input_driver());
    pMgr.load(system_event_queue::s_interface, get_system_event_queue());
}


void
process_event(const trillek::system_event_t& pEvent) {
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

int
main(int argc, char* argv[]) {
    using namespace trillek;

    subsystem_manager& mgr = standard_subsystem_manager();

    load_subsystems(mgr);
    mgr.initialise();

    system_event_queue& evqueue = mgr.lookup<system_event_queue>();
    graphics_driver& graphics = mgr.lookup<graphics_driver>();

    // input.capture_mouse(false);

    bool done = false;
    while (!done) {
        while (evqueue.more_events()) {
            system_event_t ev = evqueue.get();
            process_event(ev);
            if (ev.mType == EV_KEY && ev.mSubtype == EV_K_DOWN
                    && ev.mData == K_ESCAPE) {
                done = true;
                break;
            }
        }
        graphics.begin_rendering();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        graphics.end_rendering();
        graphics.swap_buffers();
    }
    mgr.pre_shutdown();
    mgr.shutdown();
}


