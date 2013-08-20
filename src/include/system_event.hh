#ifndef SYSTEM_EVENT_HH_INCLUDED
#define SYSTEM_EVENT_HH_INCLUDED

#include "subsystem.hh"

#include <memory>

namespace trillek {
    // Event types
    enum system_event_type_t {
        EV_NONE = 0,
        EV_MOUSE,
        EV_KEY,

        EV_LAST // Must be last
    };

    // Mouse event subtypes
    enum {
        EV_M_DX = 0,
        EV_M_DY,
        EV_M_DZ,
        EV_M_LAST
    };

    // Key event subtypes
    enum {
        EV_K_DOWN = 0,
        EV_K_UP,
        EV_K_LAST
    };

    // Base class for extra data associated with a system event.
    struct system_event_data {
    };

    typedef std::unique_ptr<system_event_data> system_event_data_ptr;

    struct system_event_t {
        system_event_type_t mType;
        unsigned mSubtype;
        int mData;
        system_event_data_ptr mExtraData;

        template<typename T>
        const T& extra_data() const
        {
            return *std::static_pointer_cast<T>(mExtraData);
        }

        explicit system_event_t(system_event_type_t pType = EV_NONE,
                       unsigned pSubtype = 0, int pData = 0)
            : mType(pType), mSubtype(pSubtype), mData(pData)
        {
        }

        system_event_t(system_event_type_t pType, unsigned pSubtype, int pData,
                system_event_data_ptr pExtraData)
            : mType(pType), mSubtype(pSubtype), mData(pData),
              mExtraData(std::move(pExtraData))
        {
        }
    };

    class system_event_queue : public subsystem
    {
    public:
        virtual void clear() = 0;

        virtual void push(system_event_t pEvent) = 0;

        virtual bool more_events() const = 0;

        virtual system_event_t get() = 0;
    };

}

#endif // SYSTEM_EVENT_HH_INCLUDED
