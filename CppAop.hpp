
#ifndef __CPPAOP_H__
#define __CPPAOP_H__

#include <iostream>

namespace WzAop
{

#define HAS_MEMBER(member)                                                                                       \
    template <typename T, typename... Args>                                                                      \
    struct HasMember_##member                                                                                    \
    {                                                                                                            \
    private:                                                                                                     \
        template <typename U>                                                                                    \
        static auto check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
                                                                                                                 \
        template <typename U>                                                                                    \
        static std::false_type check(...);                                                                       \
                                                                                                                 \
    public:                                                                                                      \
        enum                                                                                                     \
        {                                                                                                        \
            value = std::is_same<decltype(check<T>(0)), std::true_type>::value                                   \
        };                                                                                                       \
    };

    HAS_MEMBER(before)
    HAS_MEMBER(after)

    template <typename Function, typename... Args>
    class CppAop
    {
    public:
        CppAop() = default;
        CppAop(const CppAop &) = delete;
        CppAop &operator=(const CppAop &) = delete;

        CppAop(Function &&f) : m_func(std::forward<Function>(f))
        {
        }

        template <typename T>
        typename std::enable_if<HasMember_before<T, Args...>::value && HasMember_after<T, Args...>::value>::type
        invoke(Args &&... args, T &&aspect)
        {
            aspect.before(std::forward<Args>(args)...);
            m_func(std::forward<Args>(args)...);
            aspect.after(std::forward<Args>(args)...);
        }

        template <typename T>
        typename std::enable_if<HasMember_before<T, Args...>::value && !HasMember_after<T, Args...>::value>::type
        invoke(Args &&... args, T &&aspect)
        {
            aspect.before(std::forward<Args>(args)...);
            m_func(std::forward<Args>(args)...);
        }

        template <typename T>
        typename std::enable_if<!HasMember_before<T, Args...>::value && HasMember_after<T, Args...>::value>::type
        invoke(Args &&... args, T &&aspect)
        {
            m_func(std::forward<Args>(args)...);
            aspect.after(std::forward<Args>(args)...);
        }

        template <typename Head, typename... Tail>
        void invoke(Args &&... args, Head &&headAspect, Tail &&... tailAspect)
        {
            headAspect.before(std::forward<Args>(args)...);
            invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
            headAspect.after(std::forward<Args>(args)...);
        }

    private:
        Function m_func;
    };

    /* Cross platform deal: compatibility for visual studio & g++ */
    template <typename T>
    using identity_t = T;

} /* namespace WzAop */

#endif