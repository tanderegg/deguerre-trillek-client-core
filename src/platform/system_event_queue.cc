#include "system_event.hh"

namespace {
    static constexpr unsigned QUEUE_SIZE_BITS = 8;
    static constexpr unsigned QUEUE_SIZE = 1 << QUEUE_SIZE_BITS;
    static constexpr unsigned QUEUE_MASK = QUEUE_SIZE - 1;
}


namespace trillek { namespace detail {

class system_event_queue_impl : public system_event_queue
{
public:

    bool mActive;
    system_event_t mQueue[QUEUE_SIZE];
    unsigned mHead;
    unsigned mTail;

    system_event_queue_impl() {
        mActive = false;
        mHead = mTail = 0;
    }

    interface_key_t implements() const {
        return system_event_queue::s_interface;
    }

    void pre_init() {
    }

    void init(const subsystem_manager& pMgr) {
    }

    void post_init() {
        mActive = true;
    }

    void pre_shutdown() {
        mActive = false;
        clear();
    }

    void shutdown() {
    }

    void clear() {
        while (more_events()) {
            get();
        }
    }

    bool full() const {
        return ((mTail + 1) & QUEUE_MASK) == mHead;
    }

    virtual void push(system_event_t pEvent)  {
        if (full()) {
            get();
        }
        mTail = (mTail + 1) & QUEUE_MASK;
        mQueue[mTail] = std::move(pEvent);
    }

    virtual bool more_events() const {
        return mHead != mTail;
    }

    virtual system_event_t get()  {
        if (mHead == mTail) {
            return system_event_t(EV_NONE);
        }
        auto ev = std::move(mQueue[mHead]);
        mHead = (mHead + 1) & QUEUE_MASK;
        return ev;
    }
};


static system_event_queue_impl
sSysEventQueue;

} }


namespace trillek {
    system_event_queue& get_system_event_queue()
    {
        return detail::sSysEventQueue;
    }
}



