#ifndef EASY_CLONABLE_H
#define EASY_CLONABLE_H

#include <concepts>
#include <memory>

/**
 * @brief Represents an easy implementation of cloning for objects that have a
 * simple copy constructor
 */
template <typename Self>
struct ICloneable
{
    std::unique_ptr<Self> clone()
    {
        return std::make_unique<Self>(*((Self*)this));
    }
};

#endif // EASY_CLONABLE_H