// vector.h
// A non-stl header only implementation of std::vector which attempts to adhere to the
// standard interface as closely as possible.
// Note, the standard is used for some components such as std::allocator and exceptions

/*
 * Vectors are sequence containers representing arrays that can change in size
 */

#pragma once

#include <memory> // std::allocator

namespace non_stl
{
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
		// ---------------
		// BEGIN INTERFACE
		// ---------------
	public:
		// Default constructor
		vector() = default;

		explicit vector(size_t size);

	private:
		Alloc _alloc;
		size_t _capacity;
		size_t _size;
		T* _data;
	};

	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_t size) :
		_capacity(size),
		_size(0),
		_data(_alloc.allocate(size))
	{

	}
}
