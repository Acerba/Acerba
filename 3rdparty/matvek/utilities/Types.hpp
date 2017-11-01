/// MATVEK C++ Library
/// Copyright (c) 2017 Eetu Asikainen
/// 
/// This software is provided 'as-is', without any express or implied
/// warranty. In no event will the authors be held liable for any damages
/// arising from the use of this software.
/// 
/// Permission is granted to anyone to use this software for any purpose,
/// including commercial applications, and to alter it and redistribute it
/// freely, subject to the following restrictions:
/// 
/// 1. The origin of this software must not be misrepresented; you must not
///    claim that you wrote the original software. If you use this software
///    in a product, an acknowledgement in the product documentation would be
///    appreciated but is not required.
/// 2. Altered source versions must be plainly marked as such, and must not be
///    misrepresented as being the original software.
/// 3. This notice may not be removed or altered from any source distribution.

#ifndef MV_TYPES_HPP
#define MV_TYPES_HPP

namespace mv
{
    using Int8 = signed char;
    using UInt8 = unsigned char;
    
    using Int16 = signed short int;
    using UInt16 = unsigned short int;


    namespace detail
    {
        template <typename SizeType, SizeType, typename>
        class VektorImpl;
    }

    template <UInt16 Size, typename Type>
    using Vektor = detail::VektorImpl<UInt16, Size, Type>;

    template <UInt8, UInt8, typename>
    class Mat;

    template <typename>
    class Quat;

}

#endif // MV_TYPES_HPP