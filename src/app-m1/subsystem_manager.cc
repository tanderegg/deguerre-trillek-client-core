#include <subsystem.hh>
#include <unordered_map>
#include <vector>

namespace trillek { namespace detail {

class subsystem_manager_impl : public subsystem_manager {
private:
    std::vector< subsystem* > mSubsystems;
    std::unordered_map<interface_key_t,subsystem*> mIfMap;

public:
    interface_key_t implements() const {
        return subsystem_manager::s_interface;
    }

    void init(const subsystem_manager& pMgr) {
        for (auto s: mSubsystems) {
            s->init(pMgr);
        }
    }

    void pre_shutdown() {
        for (auto s: mSubsystems) {
            s->pre_shutdown();
        }
    }

    void shutdown() {
        for (auto s: mSubsystems) {
            s->shutdown();
        }
    }

    void load(interface_key_t pKey, subsystem& pSubsystem) {
        mIfMap.insert(std::pair<interface_key_t,subsystem*>(
            pKey, &pSubsystem
        ));
        mSubsystems.push_back(&pSubsystem);
    }

    subsystem*
    lookup(interface_key_t pKey) const {
        auto it = mIfMap.find(pKey);
        if (it != mIfMap.end()) {
            return it->second;
        }
        else {
            return nullptr;
        }
    }
};

} }

namespace trillek {

    subsystem_manager&
    standard_subsystem_manager() {
        static detail::subsystem_manager_impl sManager;
        return sManager;
    }

}

