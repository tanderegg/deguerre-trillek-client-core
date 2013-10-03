#ifndef DUAL_HH_INCLUDED
#define DUAL_HH_INCLUDED

namespace trillek {

typedef <typename T>
class dual {
private:
    T mReal;
    T mUnreal;

public:
    explicit dual(const T& pReal = T(), const T& pUnreal = T())
        : mReal(pReal), mUnreal(pUnreal)
    {
    }

    const T& real() const {
        return mReal;
    }

    const T& unreal() const {
        return mUnreal;
    }

    dual<T>& operator+=(const dual<T>& pRhs) {
        mReal += pRhs.mReal;
        mUnreal += pRhs.mUnreal;
        return *this;
    } 

    dual<T>& operator-=(const dual<T>& pRhs) {
        mReal -= pRhs.mReal;
        mUnreal -= pRhs.mUnreal;
        return *this;
    } 

    dual<T>& operator*=(const T& pRhs) {
        mReal *= pRhs;
        mUnreal *= pRhs;
        return *this;
    } 

    dual<T>& operator/=(const T& pRhs) {
        mReal /= pRhs;
        mUnreal /= pRhs;
        return *this;
    } 

    dual<T>& operator*=(const dual<T>& pRhs) {
        mUnreal = mReal * pRhs.mUnreal + mUnreal * pRhs.mReal;
        mReal *= pRhs.mReal;
        return *this;
    } 

    dual<T>& operator/=(const dual<T>& pRhs) {
        mReal /= pRhs.mReal;
        mUnreal -= mReal * pRhs.mUnreal;
        mUnreal /= pRhs.mReal;
        return *this;
    } 
};

template<> class dual<float>;
template<> class dual<double>;
template<> class dual<long double>;


template<typename T>
dual<T> sin(const dual<T>& pTheta) {
    return dual<T>(
        std::sin(pTheta.mReal),
        pTheta.mUnreal * std::cos(pTheta.mReal)
    );
}

template<typename T>
dual<T> cos(const dual<T>& pTheta) {
    return dual<T>(
        std::cos(pTheta.mReal),
        - pTheta.mUnreal * std::sin(pTheta.mReal)
    );
}

template<typename T>
dual<T> exp(const dual<T>& pX) {
    return dual<T>(
        std::exp(pX.mReal),
        pX.mUnreal * std::exp(pX.mReal)
    );
}

}


#endif // DUAL_HH_INCLUDED
