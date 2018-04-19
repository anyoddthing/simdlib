//
// Created by Daniel Doubleday on 04/07/15.
// Copyright (c) 2015 Daniel Doubleday. All rights reserved.
//

#pragma once

#include <array>
#include "Numerics.hpp"

namespace simd
{
    /** RAII container for 16-byte aligned memory
     * Only supports OSes where malloc returns aligned memory.
     * FIXME: Would need specialization for Win32.
     */
    struct AlignedMemory
    {
        typedef float                                 value_type;
        typedef value_type&                           reference;
        typedef const value_type&                     const_reference;
        typedef value_type*                           iterator;
        typedef const value_type*                     const_iterator;
        typedef value_type*                           pointer;
        typedef const value_type*                     const_pointer;
        typedef std::reverse_iterator<iterator>       reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        
        AlignedMemory() : _size(0) {}
        
        AlignedMemory(uint size) :
            _heap(new float[size]), _size(size)
        {}
        
        float& operator [](size_t index) { return _heap[index]; }

        iterator begin() noexcept { return _heap.get(); }
        const_iterator begin() const noexcept { return _heap.get(); }
        iterator end() noexcept { return _heap.get() + _size; }
        const_iterator end() const noexcept { return _heap.get() + _size; }

        reverse_iterator rbegin() noexcept {return reverse_iterator(end());}
        const_reverse_iterator rbegin() const noexcept {return const_reverse_iterator(end());}
        reverse_iterator rend() noexcept {return reverse_iterator(begin());}
        const_reverse_iterator rend() const noexcept {return const_reverse_iterator(begin());}

        const_iterator cbegin() const noexcept {return begin();}
        const_iterator cend() const noexcept {return end();}
        const_reverse_iterator crbegin() const noexcept {return rbegin();}
        const_reverse_iterator crend() const noexcept {return rend();}

        float* data() { return _heap.get(); }
        const float* data() const { return _heap.get(); }

        float& front() { return *_heap.get(); }

        float& back() { return *(_heap.get() + _size - 1); }

        template <typename FunctionT>
        void forEach(FunctionT f)
        {
            for (auto i = 0; i < size(); ++i)
            {
                f(i, _heap.get()[i]);
            }
        }
        
        uint size()
        {
            return _size;
        }
        
        void zero_mem()
        {
            memset(data(), 0, size() * sizeof(float));
        }
        
    private:
        std::unique_ptr<float[]> _heap;
        uint _size;
    };
    
    template <size_t Size>
    struct AlignedBuffer
    {
        typedef float                                 value_type;
        typedef value_type&                           reference;
        typedef const value_type&                     const_reference;
        typedef value_type*                           iterator;
        typedef const value_type*                     const_iterator;
        typedef value_type*                           pointer;
        typedef const value_type*                     const_pointer;
        typedef std::reverse_iterator<iterator>       reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef std::integral_constant<size_t, Size>  size_type;

        AlignedBuffer() = default;
        AlignedBuffer(const std::initializer_list<float>& coefs)
        {
            std::copy_n(coefs.begin(), std::min(Size, coefs.size()), _buffer.begin());
        }
        
        AlignedBuffer(AlignedBuffer&& other) : _buffer(std::move(other._buffer)){}
        AlignedBuffer& operator=(AlignedBuffer&& other)
        {
            _buffer = std::move(other._buffer);
            return *this;
        }

        AlignedBuffer(const AlignedBuffer&) = delete;
        AlignedBuffer& operator=(const AlignedBuffer&) = delete;

        constexpr uint size() { return Size; }

        float& operator [](size_t index) { return _buffer[index]; }
        const float& operator [](size_t index) const noexcept { return _buffer[index]; }

        iterator begin() noexcept { return _buffer.begin(); }
        const_iterator begin() const noexcept { return _buffer.begin(); }
        iterator end() noexcept { return _buffer.end(); }
        const_iterator end() const noexcept { return _buffer.end(); }

        reverse_iterator rbegin() noexcept {return reverse_iterator(end());}
        const_reverse_iterator rbegin() const noexcept {return const_reverse_iterator(end());}
        reverse_iterator rend() noexcept {return reverse_iterator(begin());}
        const_reverse_iterator rend() const noexcept {return const_reverse_iterator(begin());}

        const_iterator cbegin() const noexcept {return begin();}
        const_iterator cend() const noexcept {return end();}
        const_reverse_iterator crbegin() const noexcept {return rbegin();}
        const_reverse_iterator crend() const noexcept {return rend();}

        float* data() { return _buffer.data(); }
        const float* data() const { return _buffer.data(); }

        float& front() { return _buffer.front(); }

        float& back() { return _buffer.back(); }

        template <typename FunctionT>
        void forEach(FunctionT f)
        {
            uint i = 0;
            std::for_each(begin(), end(), [&](reference val) {
                f(i, val);
                i++;
            });
        }

        template <typename FunctionT>
        void forEach(FunctionT f) const
        {
            int i = 0;
            std::for_each(begin(), end(), [&](auto val) {
                f(i, val);
                i++;
            });
        }
        
        void set(size_t start, float v1, float v2, float v3, float v4)
        {
            _buffer[start + 0] = v1;
            _buffer[start + 1] = v2;
            _buffer[start + 2] = v3;
            _buffer[start + 3] = v4;
        }
        
        void zero_mem()
        {
            recipes<Size>::memset(data(), 0);
        }

        void fill(float value)
        {
            simd::recipes<Size>::memset(data(), value);
        }

        void copy(const float* src)
        {
            simd::recipes<Size>::memcopy(data(), src);
        }

        void add(const float* src)
        {
            simd::recipes<Size>::add(data(), data(), src);
        }
        
    private:
        alignas(16) std::array<float, Size> _buffer;
    };

    

}

