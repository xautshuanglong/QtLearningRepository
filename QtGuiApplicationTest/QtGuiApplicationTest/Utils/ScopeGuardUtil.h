#ifndef SCOPE_GUARD_UTIL_H
#define SCOPE_GUARD_UTIL_H

namespace Shuanglong::Utils
{
    template <class Func>
    class ScopeGuardUtil final
    {
    public:
        ScopeGuardUtil() = delete;
        ScopeGuardUtil(Func destroyFunc): m_bActive(true), m_destroyFunc(destroyFunc) {}
        ~ScopeGuardUtil()
        {
            if (m_bActive)
            {
                m_destroyFunc();
            }
        }

        ScopeGuardUtil(ScopeGuardUtil&& other) : m_bActive(other.m_bActive), m_destroyFunc(other.m_destroyFunc)
        {
            other.Dismiss();
        }

        void Dismiss() { m_bActive = false; }

    private:
        bool m_bActive;
        Func m_destroyFunc;
    };

    enum class ScopeGuardHelper {};
    
    template<class Func>
    ScopeGuardUtil<Func> operator+(ScopeGuardHelper, Func&& destoryFunc)
    {
        return ScopeGuardUtil(destoryFunc);
    }

#define SCOPEGUARD_CONCAT(str1, str2) str1##str2
#define SCOPEGUARD_INSTANCE(XXX) SCOPEGUARD_CONCAT(ScopeGuard_Instance_, XXX)
#define SCOPEGUARD_DESTROY(XXX) auto SCOPEGUARD_INSTANCE(XXX) = Shuanglong::Utils::ScopeGuardHelper() + [&]()
}

#endif // SCOPE_GUARD_UTIL_H
