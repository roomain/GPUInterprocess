#pragma once
/***********************************************
* @headerfile enumerates.h
* @date 25 / 08 / 2024
* @author Roomain
************************************************/
#include <vector>

// enumerate in opencl
template<typename ...Arg, typename Fun, typename Type>
void enumerate(const Fun& a_fun, std::vector<Type>& a_list, Arg... a_arg)
{
    uint32_t num = 0;
    a_fun(num, a_arg..., nullptr, &num);
    a_list.resize(num);
    a_fun(num, a_arg..., a_list.data(), &num);
}

