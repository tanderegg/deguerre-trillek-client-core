#ifndef UTILS_HH_INCLUDED
#define UTILS_HH_INCLUDED

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <exception>
#include <bitset>
#include <vector>
#include <array>
#include <boost/utility.hpp>

namespace trillek {

    template<typename Container>
    void ensure_capacity(Container& pContainer, std::size_t pExtra = 1) {
        std::size_t oldSize = pContainer.size();
        std::size_t requiredSize = oldSize + pExtra;
        if (requiredSize >= pContainer.capacity()) {
            std::size_t newSize = std::max(requiredSize, (oldSize*5) >> 2);
            pContainer.reserve(newSize);
        }
    }

    class change_tracker {
    private:
        uint32_t mCurrentChange;
        uint32_t mAppliedChange;

    public:
        void record_change() {
            ++mCurrentChange;
        }

        void changes_committed() {
            mAppliedChange = mCurrentChange;
        }

        bool changes_pending() const {
            return mCurrentChange != mAppliedChange;
        }

        change_tracker()
            : mCurrentChange(0), mAppliedChange(0)
        {
        }
    };

}

#endif // UTILS_HH_INCLUDED
