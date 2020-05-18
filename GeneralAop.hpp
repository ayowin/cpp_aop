
#ifndef __GENERALAOP_H__
#define __GENERALAOP_H__

#include "CppAop.hpp"

class GeneralAop
{
public:
    GeneralAop() = delete;
    ~GeneralAop() = delete;

    template <typename... AP, typename... Args, typename Function>
    static void invoke(Function &&f, Args &&... args)
    {
        WzAop::CppAop<Function, Args...> asp(std::forward<Function>(f));
        asp.invoke(std::forward<Args>(args)..., WzAop::identity_t<AP>()...);
    }
};

#endif