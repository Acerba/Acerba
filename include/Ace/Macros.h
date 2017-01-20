#pragma once
#include <Ace/IntTypes.h>

///Macro for |, |=, &, &= operators for enums
#define ACE_ENUM_OPERATORS(enumName) \
enumName operator|(enumName lhs, enumName rhs) \
{ \
    return static_cast<enumName>(static_cast<ace::UInt16>(lhs) | static_cast<ace::UInt16>(rhs)); \
} \
enumName operator|=(enumName lhs, enumName rhs) \
{ \
    return lhs = lhs | rhs; \
} \
enumName operator&(enumName lhs, enumName rhs) \
{ \
    return static_cast<enumName>(static_cast<ace::UInt16>(lhs) & static_cast<ace::UInt16>(rhs)); \
} \
enumName operator&=(enumName lhs, enumName rhs) \
{ \
    return lhs = lhs & rhs; \
}


///Disable copy-ctors for a class
#define ACE_DISABLE_COPY(className) \
    private: \
    className(const className&)=delete; \
    className(const className&&)=delete; \
    className operator=(const className&)=delete;