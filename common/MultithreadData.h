#pragma once
#include <condition_variable>
#include <mutex>
#include <functional>
#include "notcopiable.h"

template<typename Data>
using readData = std::function<void(const Data&)>;

template<typename Data>
using writeData = std::function<void(Data&)>;

template<typename SharedData>
class MultithreadData
{
private:
    mutable std::mutex m_accessProtect;
    SharedData m_sharedData;
    
public:
	NOT_COPIABLE(MultithreadData)
	NOT_MOVABLE(MultithreadData)
    MultithreadData() = default;
    ~MultithreadData() = default;
    void process(writeData<SharedData> a_fun)
    {
        std::scoped_lock lock(m_accessProtect);
        a_fun(m_sharedData);
    }
    void process(readData<SharedData> a_fun)const
    {
        std::scoped_lock lock(m_accessProtect);
        a_fun(m_sharedData);
    }
};