#pragma once

#include <memory>

// std::make_unique not in c++11 standard
namespace std
{
    template <typename T, typename ... Args>
    std::unique_ptr<T> make_unique(Args&& ... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
