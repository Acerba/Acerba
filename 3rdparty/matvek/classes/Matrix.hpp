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

#ifndef MV_MATRIX_HPP
#define MV_MATRIX_HPP

#include "../utilities/Utilities.hpp"

namespace mv
{
    
    template <UInt8 Rows, UInt8 Columns = Rows, typename Type = MV_TYPE>
    class Mat
    {
        static_assert(Rows != 0u && Columns != 0u, "Cannot create matrix of dimension 0");
        
        //Matrix data
        union
        {
            std::array<Type, Rows * Columns> m_data;
            std::array<std::array<Type, Columns>, Rows> m_dataRows;
        };
        
        
    public:
        
        //Ctors
        
        //Default ctor (zero matrix)
        MV_API Mat() : m_data()
        {
            for (UInt16 i = 0u; i < Rows * Columns; ++i)
            {
                m_data[i] = static_cast<Type>(0);
            }
        }
        //Array ctor
        MV_API Mat(const std::array<Type, Rows * Columns>& arr) : m_data(arr)
        {
            
        }
        //Single argument ctor
        template <typename T,
        typename std::enable_if<detail::AcceptedType<Type, T>::value>::type* = nullptr
        >
        MV_API Mat(T arg) : m_data{{ std::forward<T>(arg) }}
        {
            
        }
        //Variadic ctor
        template <typename T, typename ... Args,
        typename std::enable_if<detail::AcceptedType<Type, Args...>::value>::type* = nullptr,
        typename std::enable_if<detail::AcceptedType<Type, T>::value>::type* = nullptr
        >
        MV_API Mat(T arg, Args&& ... args) : m_data{{ std::forward<T>(arg), std::forward<Args>(args)... }}
        {
            static_assert((sizeof...(Args) + 1u) == (Rows * Columns), "Invalid matrix constructor argument count");
        }
        //////////////////////////////////////////////////////////
        
        
        //Dtor
        ~Mat() = default;
        //////////////////////////////////////////////////////////
        
        MV_API Mat(Mat&&) = default;
        MV_API Mat(const Mat&) = default;
        
        #if 0
        
        //Move ctor
        MV_API Mat(Mat&& other) : m_data(std::move(other.m_data))
        {
            //m_data = std::move(other.m_data);
        }
        //////////////////////////////////////////////////////////
        
        
        //Copy-ctor
        MV_API Mat(const Mat& other) : m_data(other.m_data)
        {
            
        }
        //////////////////////////////////////////////////////////
        
        #endif // if 0
        
        //Assignment
        MV_API Mat& operator=(Mat&& other)
        {
            m_data = other.m_data;
            // swap(*this, other);
            return *this;
        }
        //////////////////////////////////////////////////////////
        
        
        MV_API Mat& operator=(const Mat& other)
        {
            m_data = other.m_data;
            return *this;
        }
        //////////////////////////////////////////////////////////
        
        
        //Type conversion
        template <typename T, typename std::enable_if<detail::MV_IsAllowed<T, Type>::value>::type* = nullptr>
        MV_API operator Mat<Rows, Columns, T>()
        {
            std::array<T, Rows * Columns> arr{};
            for (UInt16 i = 0u; i < Rows * Columns; ++i)
            {
                arr[i] = static_cast<T>(m_data[i]);
            }
            return Mat<Rows, Columns, T>(arr);
        }
        //////////////////////////////////////////////////////////
        
        MV_API const Type* array() const
        {
            return m_data.data();
        }
        MV_API Type* array()
        {
            return m_data.data();
        }
        
        
        MV_API Type at(const UInt16 index) const
        {
            MV_ASSERT(index < (Rows * Columns), "Raw index out of range");
            return m_data[index];
        }
        MV_API Type& at(const UInt16 index)
        {
            MV_ASSERT(index < (Rows * Columns), "Raw index out of range");
            return m_data[index];
        }
        //////////////////////////////////////////////////////////
        
        
        MV_API Type at(const UInt8 row, const UInt8 column) const
        {
            MV_ASSERT(row < Rows, "Row index out of range");
            MV_ASSERT(column < Columns, "Column index out of range");
            return m_dataRows[row][column];
        }
        MV_API Type& at(const UInt8 row, const UInt8 column)
        {
            MV_ASSERT(row < Rows, "Row index out of range");
            MV_ASSERT(column < Columns, "Column index out of range");
            return m_dataRows[row][column];
        }
        //////////////////////////////////////////////////////////
        
        
        //Get const ref to data
        MV_API const std::array<Type, Rows * Columns>& data() const
        {
            return m_data;
        }
        //Get ref to data
        MV_API std::array<Type, Rows * Columns>& data()
        {
            return m_data;
        }
        //////////////////////////////////////////////////////////
        
        
        //Get const ref to data
        MV_API const std::array<Type, Columns>& dataRow(const UInt8 row) const
        {
            MV_ASSERT(row < Rows, "Index out of range");
            return m_dataRows[row];
        }
        //Get ref to data
        MV_API std::array<Type, Columns>& dataRow(const UInt8 row)
        {
            MV_ASSERT(row < Rows, "Index out of range");
            return m_dataRows[row];
        }
        //////////////////////////////////////////////////////////
        
        
        //Fill entire matrix with one data value
        MV_API void fill(Type data)
        {
            for (auto& itr : m_data)
            {
                itr = data;
            }
        }
        //////////////////////////////////////////////////////////
        
        
        template <UInt16 I>
        inline MV_API const Type& get() const
        {
            static_assert(I < (Rows * Columns), "Matrix: Get<I>: Invalid index");
            return std::get<I>(m_data);
        }
        template <UInt8 R, UInt8 C>
        inline MV_API const Type& get() const
        {
            static_assert(R < Rows && C < Columns, "Matrix: Get<R, C>: Invalid index");
            return std::get<C>(std::get<R>(m_dataRows));
        }
        //////////////////////////////////////////////////////////
        
        
        MV_API void scale(const Type scalar)
        {
            for (UInt16 i = 0u; i < Rows * Columns; ++i)
            {
                m_data[i] *= scalar;
            }
        }
        //////////////////////////////////////////////////////////
        
        
        //Return number of elements in matrix
        MV_API UInt16 size() const
        {
            return Rows * Columns;
        }
        //////////////////////////////////////////////////////////
        
        
        //Swap
        MV_API friend void swap(Mat& first, Mat& second)
        {
            using std::swap;
            swap(first.m_data, second.m_data);
        }
        //////////////////////////////////////////////////////////
        
        
        //Return data from raw index
        MV_API Type operator[](const UInt16 index) const
        {
            MV_ASSERT(index < (Rows * Columns), "Raw index out of range");
            return m_data[index];
        }
        MV_API Type& operator[](const UInt16 index)
        {
            MV_ASSERT(index < (Rows * Columns), "Raw index out of range");
            return m_data[index];
        }
        //////////////////////////////////////////////////////////
        
        
        //Return data from index
        MV_API Type operator[](const Index index) const
        {
            MV_ASSERT(index.Row < Rows, "Row index out of range");
            MV_ASSERT(index.Col < Columns, "Column index out of range");
            return m_data[index.Row][index.Col];
        }
        MV_API Type& operator[](const Index index)
        {
            MV_ASSERT(index.Row < Rows, "Row index out of range");
            MV_ASSERT(index.Col < Columns, "Column index out of range");
            return m_data[index.Row][index.Col];
        }
        //////////////////////////////////////////////////////////
        
        
        //Increment
        MV_API Mat& operator+=(const Mat& rhs)
        {
            for (UInt16 i = 0u; i < (Rows * Columns); ++i)
            {
                m_data[i] += rhs.m_data[i];
            }
            return *this;
        }
        MV_API friend Mat operator+(Mat lhs, const Mat& rhs)
        {
            lhs += rhs;
            return lhs;
        }
        //////////////////////////////////////////////////////////
        
        
        //Substract
        MV_API Mat& operator-=(const Mat& rhs)
        {
            for (UInt16 i = 0u; i < (Rows * Columns); ++i)
            {
                m_data[i] -= rhs.m_data[i];
            }
            return *this;
        }
        MV_API friend Mat operator-(Mat lhs, const Mat& rhs)
        {
            lhs -= rhs;
            return lhs;
        }
        //////////////////////////////////////////////////////////
        
        
        // Scale
        MV_API Mat& operator*=(const Type rhs)
        {
            scale(rhs);
            return *this;
        }
        MV_API friend Mat operator*(Mat lhs, const Type& rhs)
        {
            lhs *= rhs;
            return lhs;
        }
        //////////////////////////////////////////////////////////
        
    }; //Mat
    
    #include "../functions/MatrixFunctions.hpp"
    
    #ifdef MV_VEKTOR_HPP
    #include "../functions/MatrixVektorFunctions.hpp"
    #endif
    #ifdef MV_QUATERNION_HPP
    #include "../functions/MatrixQuaternionFunctions.hpp"
    #endif
    
} // mv

#endif // MV_MATRIX_HPP
