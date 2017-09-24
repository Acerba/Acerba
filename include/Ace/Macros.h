#pragma once
#include <Ace/IntTypes.h>

///Enable bitwise operators for an enum
#define ACE_ENUM_OPERATORS(enumName) \
inline enumName operator|(enumName lhs, enumName rhs) \
{ \
    return static_cast<enumName>(static_cast<ace::UInt64>(lhs) | static_cast<ace::UInt64>(rhs)); \
} \
inline enumName operator|=(enumName lhs, enumName rhs) \
{ \
    return lhs = lhs | rhs; \
} \
inline enumName operator&(enumName lhs, enumName rhs) \
{ \
    return static_cast<enumName>(static_cast<ace::UInt64>(lhs) & static_cast<ace::UInt64>(rhs)); \
} \
inline enumName operator&=(enumName lhs, enumName rhs) \
{ \
    return lhs = lhs & rhs; \
} \
inline enumName operator^(enumName lhs, enumName rhs) \
{ \
    return static_cast<enumName>(static_cast<ace::UInt64>(lhs) ^ static_cast<ace::UInt64>(rhs)); \
} \
inline enumName operator^=(enumName lhs, enumName rhs) \
{ \
    return lhs = lhs ^ rhs; \
} \
inline enumName operator~(enumName rhs) \
{ \
    return static_cast<enumName>( ~ static_cast<ace::UInt64>(rhs)); \
}


///Disable copy-ctors for a class
#define ACE_DISABLE_COPY(className) \
    private: \
    className(className&&)=delete; \
    className(const className&)=delete; \
    className& operator=(className&&)=delete; \
    className& operator=(const className&)=delete;

