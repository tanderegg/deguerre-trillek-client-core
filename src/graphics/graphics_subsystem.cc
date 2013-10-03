#include <graphics_subsystem.hh>
#include <graphics_adapter.hh>
#include <graphics_device.hh>


namespace trillek {

struct graphics_subsystem::impl {
    impl(graphics_subsystem& pSubsys)
    {
    }

    ~impl()
    {
    }

    std::vector< std::shared_ptr<graphics_adapter> > mAdapters;
};

void
graphics_subsystem::pre_init() {
}


void
graphics_subsystem::init(const subsystem_manager& pMgr) {
    // During the init phase, the platform injects adapters.
}


void
graphics_subsystem::post_init() {
    // Adapters should be registered now.

    if (mPImpl->mAdapters.size() == 0) {
        throw std::logic_error("No graphics adapters registered");
    }
    if (mPImpl->mAdapters.size() > 1) {
        throw std::logic_error("More than one graphics adapter registered");
    }
}


void
graphics_subsystem::pre_shutdown() {
    mPImpl->mAdapters.clear();
}


void
graphics_subsystem::shutdown() {
}


void
graphics_subsystem::register_adapter(
        std::shared_ptr<graphics_adapter> pAdapter)
{
    mPImpl->mAdapters.push_back(std::move(pAdapter));
}


std::shared_ptr<graphics_device>
graphics_subsystem::create_device() const {
    return mPImpl->mAdapters[0]->mDeviceFactory();
}


graphics_subsystem::graphics_subsystem()
    : mPImpl(new graphics_subsystem::impl(*this))
{
}


graphics_subsystem::~graphics_subsystem()
{
}


graphics_subsystem&
graphics_subsystem::get_graphics_subsystem() {
    static graphics_subsystem sGraphicsSubsystem;
    return sGraphicsSubsystem;
}


}
