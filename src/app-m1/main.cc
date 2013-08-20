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
main(int argc, char* argv[])
{
    using namespace trillek;

    graphics_driver& graphics = get_graphics_driver();
    input_driver& input = get_input_driver();
    system_event_queue& evqueue = get_system_event_queue();

    graphics.init();
    input.init();
    evqueue.init();

    input.set_system_event_queue(&evqueue);

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

    graphics.pre_shutdown();
    input.pre_shutdown();
    graphics.shutdown();
    input.shutdown();
}


