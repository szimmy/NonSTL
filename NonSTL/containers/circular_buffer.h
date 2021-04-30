// circular_buffer.h
// A non-stl header only implementation of a circular buffer data structure which attempts to adhere to
// standard practices.
// Note, the standard is used for some components such as std::allocator and exceptions

/*
 * A circular buffer is a container of fixed size which uses a block of memory allocated at construction.
 * When the buffer fills new data is written over the oldest data in the buffer.
 * This container throws no exceptions.
 */

#pragma once

// Includes
#include <algorithm>		// std::copy_n, std::swap
#include <array>			// std::array
#include <memory>			// std::allocator
#include <utility>			// std::forward

// TODO REMOVE AFTER TESTING
#include <iostream>

using size_type = size_t;

namespace non_stl
{
	// Template parameter T is the generic object being stored within the container
	// size_type N is the size of the container which cannot be changed
	// Template parameter Alloc is the allocator which defines memory allocation for
	// objects of type T. If a custom allocator is not provided then the default 
	// std::allocator<T> will be used
	template <class T, size_type N, class Alloc = std::allocator<T> >
	class circular_buffer
	{
	private:
		static constexpr size_type BUFFER_SIZE = N;
		using container = std::array<T, BUFFER_SIZE>;

		// Container which holds the buffer data
		container d_container;

		// Index which represents the oldest data to be overwritten
		size_type d_head;

		// Index which represents the newest data
		size_type d_tail;

		// Amount of elements written into the buffer
		size_type d_size;

		// Allocator objects
		Alloc	  d_alloc;

	public:
		// ---------------
		// CONSTRUCTORS
		// ---------------

		// Default constructor
		circular_buffer() noexcept;
		/*
		// Copy constructor
		circular_buffer(const circular_buffer& other) noexcept;

		// Move constructor
		circular_buffer(circular_buffer&& other) noexcept;

		// ---------------
		// OPERATOR=
		// ---------------
		circular_buffer(const circular_buffer& rhs) noexcept;
		circular_buffer(circular_buffer&& rhs) noexcept;

		// ---------------
		// DESTRUCTOR
		// ---------------
		~circular_buffer();

		// ---------------
		// ELEMENT ACCESS
		// ---------------

		// Returns a reference to the element at position n in the buffer
		// with no range check
		T& operator[](size_type n) noexcept;
		const T& operator[](size_type n) const noexcept;

		// Returns a reference to the element at position n in the buffer
		// Operationally same as operator[]
		T& at(size_type n) noexcept;
		const T& at(size_type n) const noexcept;
		*/
		// Returns a reference to the first element in the buffer
		T& front() noexcept;
		const T& front() const noexcept;

		// Returns a reference to the last element in the buffer
		T& back() noexcept;
		const T& back() const noexcept;
		/*
		// TODO ITERATORS
		// TODO ITERATORS
		// TODO ITERATORS
		// TODO ITERATORS
		// TODO ITERATORS
		// TODO ITERATORS

		// ---------------
		// CAPACITY
		// ---------------
		// Returns the number of elements in the buffer
		size_type size() const noexcept;

		// Return the maximum number of elements the buffer can hold
		constexpr size_type max_size() const noexcept;

		// Returns the size of the storage space currently allocated for the buffer,
		// expressed in terms of elements
		// Equivalent to max_size()
		constexpr size_type capacity() const noexcept;

		// Returns whether the buffer is empty 
		// (i.e. whether its size is 0)
		bool empty() const noexcept;*/

		// ---------------
		// MODIFIERS
		// ---------------
		// Adds a new element at the end of the buffer after its current last element
		void push_back(const T& val) noexcept;
		void push_back(T&& val) noexcept;
		
		// Appends a new element to the end of the buffer.
		// The arguments args... are forwarded to the constructor as std::forward<Args>(args).... 
		template <class... Args>
		void emplace_back(Args&& ... args) noexcept;
		
		// Removes the first element in the buffer
		void pop_front() noexcept;
		/*
		// ---------------
		// ALLOCATOR
		// ---------------

		// Returns a copy of the allocator object associated with this buffer
		Alloc get_allocator() const noexcept;*/

	private:
		void increment_head() noexcept;
		void increment_tail() noexcept;
	};


	// IMPL

	// ---------------
	// CONSTRUCTORS
	// ---------------
	template <class T, size_type N, class Alloc>
	circular_buffer<T, N, Alloc>::circular_buffer() noexcept 
		: d_container()
		, d_head(0)
		, d_tail(BUFFER_SIZE - 1)
		, d_size(0)
	{

	}

	// ---------------
	// ELEMENT ACCESS
	// ---------------
	template <class T, size_type N, class Alloc>
	T& circular_buffer<T, N, Alloc>::front() noexcept
	{
		return d_container[d_head];
	}

	template <class T, size_type N, class Alloc>
	const T& circular_buffer<T, N, Alloc>::front() const noexcept
	{
		return d_container[d_head];
	}

	template <class T, size_type N, class Alloc>
	T& circular_buffer<T, N, Alloc>::back() noexcept
	{
		return d_container[d_tail];
	}

	template <class T, size_type N, class Alloc>
	const T& circular_buffer<T, N, Alloc>::back() const noexcept
	{
		return d_container[d_tail];
	}

	// ---------------
	// MODIFIERS
	// ---------------
	template <class T, size_type N, class Alloc>
	void circular_buffer<T, N, Alloc>::push_back(const T& val) noexcept 
	{
		increment_tail();
		if (d_size > BUFFER_SIZE) {
			increment_head();
		}
		d_alloc.construct((d_container.data() + d_tail), val);
	}

	template <class T, size_type N, class Alloc>
	void circular_buffer<T, N, Alloc>::push_back(T&& val) noexcept
	{
		increment_tail();
		if (d_size > BUFFER_SIZE) {
			increment_head();
		}
		d_alloc.construct((d_container.data() + d_tail), std::forward<T>(val));
	}

	template <class T, size_type N, class Alloc>
	template <class... Args>
	void circular_buffer<T, N, Alloc>::emplace_back(Args&& ... args) noexcept
	{
		increment_tail();
		if (d_size > BUFFER_SIZE) {
			increment_head();
		}
		d_alloc.construct((d_container.data() + d_tail), std::forward<Args>(args)...);
	}

	template <class T, size_type N, class Alloc>
	void circular_buffer<T, N, Alloc>::pop_front() noexcept
	{
		increment_head();
	}

	// ---------------
	// PRIVATE
	// ---------------
	template <class T, size_type N, class Alloc>
	void circular_buffer<T, N, Alloc>::increment_head() noexcept
	{
		// Only do this on non-empty buffers
		if (d_size != 0) {
			++d_head;
			--d_size;
			if (d_head == BUFFER_SIZE) {
				d_head = 0;
			}
		}

	}

	template <class T, size_type N, class Alloc>
	void circular_buffer<T, N, Alloc>::increment_tail() noexcept
	{
		++d_tail;
		++d_size;
		if (d_tail == BUFFER_SIZE) {
			d_tail = 0;
		}
	}
}