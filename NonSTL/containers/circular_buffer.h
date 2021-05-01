// circular_buffer.h
// A non-stl header only implementation of a circular buffer data structure which attempts to adhere to
// standard practices.
// Note, the standard is used for some components such as std::array and std::move

/*
 * A circular buffer is a container of fixed size which uses a block of memory allocated at construction.
 * When the buffer fills new data is written over the oldest data in the buffer.
 * This container throws no exceptions.
 */

#pragma once

// Includes
#include <array>			// std::array
#include <iterator>			// std::random_access_iterator_tag
#include <type_traits>		// std::conditional_t
#include <utility>			// std::forward, std::move

using size_type = size_t;

namespace non_stl
{
	// Template parameter T is the generic object being stored within the container
	// size_type N is the size of the container which cannot be changed
	template <class T, size_type N>
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

	public:
		// ---------------
		// CONSTRUCTORS
		// ---------------

		// Default constructor
		circular_buffer() noexcept;
		
		// Copy constructor
		circular_buffer(const circular_buffer& other) noexcept;

		// Move constructor
		circular_buffer(circular_buffer&& other) noexcept;
		
		// ---------------
		// OPERATOR=
		// ---------------
		circular_buffer& operator=(const circular_buffer& rhs) noexcept;
		circular_buffer& operator=(circular_buffer&& rhs) noexcept;

		// ---------------
		// DESTRUCTOR
		// ---------------
		~circular_buffer() = default;
		
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
		
		// Returns a reference to the first element in the buffer
		T& front() noexcept;
		const T& front() const noexcept;

		// Returns a reference to the last element in the buffer
		T& back() noexcept;
		const T& back() const noexcept;
		
		// ---------------
		// ITERATORS
		// ---------------

		template <bool isConst> struct myIterator;
		using iterator = myIterator<false>;
		using const_iterator = myIterator<true>;

		// Returns an iterator to the first element of the container
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;

		// Returns a reverse iterator to the first element of the reversed container
		// Equivalent to the last element (not end) of the non-reversed container
		iterator rbegin() noexcept;
		const_iterator rbegin() const noexcept;

		// Returns an iterator to the element following the last element of the container
		iterator end() noexcept;
		const_iterator end() const noexcept;
		const_iterator cend() const noexcept;

		// Returns a reverse iterator to the element following the last element of the reversed container
		// It corresponds to the element preceding the first element of the non-reversed container
		iterator rend() noexcept;
		const_iterator rend() const noexcept;

		template <bool isconst = false>
		struct myIterator
		{
			using iterator_category = std::random_access_iterator_tag;
			using reference = typename std::conditional_t< isconst, T const&, T& >;
			using pointer = typename std::conditional_t< isconst, T const*, T* >;
			using vec_pointer = typename std::conditional_t<isconst, std::array<T,N> const*, std::array<T,N>*>;
		private:
			vec_pointer ptrToBuffer;
			size_type	offset;
			size_type	index;
			bool		reverse;

			bool comparable(const myIterator& other) {
				return (reverse == other.reverse);
			}

		public:
			myIterator() : ptrToBuffer(nullptr), offset(0), index(0), reverse(false) {}  //
			myIterator(const circular_buffer<T, N>::myIterator<false>& i) :
				ptrToBuffer(i.ptrToBuffer),
				offset(i.offset),
				index(i.index),
				reverse(i.reverse) {}
			reference operator*() {
				if (reverse)
					return (*ptrToBuffer)[(ptrToBuffer->size() + offset - index) % (ptrToBuffer->size())];
				return (*ptrToBuffer)[(offset + index) % (ptrToBuffer->size())];
			}
			reference operator[](size_type index) {
				myIterator iter = *this;
				iter.index += index;
				return *iter;
			}
			pointer operator->() { return &(operator *()); }

			myIterator& operator++ ()
			{
				++index;
				return *this;
			};
			myIterator operator ++(int)
			{
				myIterator iter = *this;
				++index;
				return iter;
			}
			myIterator& operator --()
			{
				--index;
				return *this;
			}
			myIterator operator --(int) {
				myIterator iter = *this;
				--index;
				return iter;
			}
			friend myIterator operator+(myIterator lhs, int rhs) {
				lhs.index += rhs;
				return lhs;
			}
			friend myIterator operator+(int lhs, myIterator rhs) {
				rhs.index += lhs;
				return rhs;
			}
			myIterator& operator+=(int n) {
				index += n;
				return *this;
			}
			friend myIterator operator-(myIterator lhs, int rhs) {
				lhs.index -= rhs;
				return lhs;
			}
			friend long long operator-(const myIterator& lhs, const myIterator& rhs) {
				lhs.index -= rhs;
				return lhs.index - rhs.index;
			}
			myIterator& operator-=(int n) {
				index -= n;
				return *this;
			}
			bool operator==(const myIterator& other)
			{
				if (comparable(other))
					return (index + offset == other.index + other.offset);
				return false;
			}
			bool operator!=(const myIterator& other)
			{
				if (comparable(other)) return !this->operator==(other);
				return true;
			}
			bool operator<(const myIterator& other)
			{
				if (comparable(other))
					return (index + offset < other.index + other.offset);
				return false;
			}
			bool operator<=(const myIterator& other)
			{
				if (comparable(other))
					return (index + offset <= other.index + other.offset);
				return false;
			}
			bool operator >(const myIterator& other)
			{
				if (comparable(other)) return !this->operator<=(other);
				return false;
			}
			bool operator>=(const myIterator& other)
			{
				if (comparable(other)) return !this->operator<(other);
				return false;
			}
			friend class circular_buffer<T, N>;
		};

		// ---------------
		// CAPACITY
		// ---------------
		// Returns the number of elements in the buffer
		size_type size() const noexcept;

		// Returns whether the buffer is empty 
		// (i.e. whether its size is 0)
		bool empty() const noexcept;

		// Return the maximum number of elements the buffer can hold
		constexpr size_type max_size() const noexcept;

		// Returns the size of the storage space currently allocated for the buffer,
		// expressed in terms of elements
		// Equivalent to max_size()
		constexpr size_type capacity() const noexcept;

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

	private:
		void increment_head() noexcept;
		void increment_tail() noexcept;
	};

	// IMPL

	// ---------------
	// CONSTRUCTORS
	// ---------------
	template <class T, size_type N>
	circular_buffer<T, N>::circular_buffer() noexcept 
		: d_container()
		, d_head(0)
		, d_tail(BUFFER_SIZE - 1)
		, d_size(0)
	{

	}

	template <class T, size_type N>
	circular_buffer<T, N>::circular_buffer(const circular_buffer& other) noexcept
		: d_container(other.d_container)
		, d_head(other.d_head)
		, d_tail(other.d_tail)
		, d_size(other.d_size)
	{

	}

	template <class T, size_type N>
	circular_buffer<T, N>::circular_buffer(circular_buffer&& other) noexcept
		: d_container(std::move(other.d_container))
		, d_head(other.d_head)
		, d_tail(other.d_tail)
		, d_size(other.d_size)
	{

	}

	// ---------------
	// OPERATOR=
	// ---------------

	template <class T, size_type N>
	circular_buffer<T, N>& circular_buffer<T, N>::operator=(const circular_buffer<T, N>& rhs) noexcept
	{
		// Assign member variables
		d_container = rhs.d_container;
		d_head = rhs.d_head;
		d_tail = rhs.d_tail;
		d_size = rhs.d_size;

		return *this;
	}

	template <class T, size_type N>
	circular_buffer<T, N>& circular_buffer<T, N>::operator=(circular_buffer<T, N>&& rhs) noexcept
	{
		// Assign member variables
		d_container = std::move(rhs.d_container);
		d_head = rhs.d_head;
		d_tail = rhs.d_tail;
		d_size = rhs.d_size;

		return *this;
	}

	// ---------------
	// ELEMENT ACCESS
	// ---------------

	template <class T, size_type N>
	T& circular_buffer<T, N>::operator[](size_type n) noexcept
	{
		n += d_head;
		n %= BUFFER_SIZE;
		return d_container[n];
	}

	template <class T, size_type N>
	const T& circular_buffer<T, N>::operator[](size_type n) const noexcept
	{
		n += d_head;
		n %= BUFFER_SIZE;
		return d_container[n];
	}

	template <class T, size_type N>
	T& circular_buffer<T, N>::at(size_type n) noexcept
	{
		return this->operator[](n);
	}

	template <class T, size_type N>
	const T& circular_buffer<T, N>::at(size_type n) const noexcept
	{
		return this->operator[](n);
	}

	template <class T, size_type N>
	T& circular_buffer<T, N>::front() noexcept
	{
		return d_container[d_head];
	}

	template <class T, size_type N>
	const T& circular_buffer<T, N>::front() const noexcept
	{
		return d_container[d_head];
	}

	template <class T, size_type N>
	T& circular_buffer<T, N>::back() noexcept
	{
		return d_container[d_tail];
	}

	template <class T, size_type N>
	const T& circular_buffer<T, N>::back() const noexcept
	{
		return d_container[d_tail];
	}

	// ---------------
	// ITERATORS
	// ---------------

	template <class T, size_type N>
	typename circular_buffer<T, N>::iterator circular_buffer<T, N>::begin() noexcept
	{
		iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_head;
		iter.index = 0;
		iter.reverse = false;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::const_iterator circular_buffer<T, N>::begin() const noexcept
	{
		const_iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_head;
		iter.index = 0;
		iter.reverse = false;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::const_iterator circular_buffer<T, N>::cbegin() const noexcept
	{
		const_iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_head;
		iter.index = 0;
		iter.reverse = false;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::iterator circular_buffer<T, N>::rbegin() noexcept
	{
		iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_tail;
		iter.index = 0;
		iter.reverse = true;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::const_iterator circular_buffer<T, N>::rbegin() const noexcept
	{
		const_iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_tail;
		iter.index = 0;
		iter.reverse = true;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::iterator circular_buffer<T, N>::end() noexcept
	{
		iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_head;
		iter.index = d_size;
		iter.reverse = false;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::const_iterator circular_buffer<T, N>::end() const noexcept
	{
		const_iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_head;
		iter.index = d_size;
		iter.reverse = false;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::const_iterator circular_buffer<T, N>::cend() const noexcept
	{
		const_iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_head;
		iter.index = d_size;
		iter.reverse = false;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::iterator circular_buffer<T, N>::rend() noexcept
	{
		iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_tail;
		iter.index = d_size;
		iter.reverse = true;
		return iter;
	}

	template <class T, size_type N>
	typename circular_buffer<T, N>::const_iterator circular_buffer<T, N>::rend() const noexcept
	{
		const_iterator iter;
		iter.ptrToBuffer = &d_container;
		iter.offset = d_tail;
		iter.index = d_size;
		iter.reverse = true;
		return iter;
	}

	// ---------------
	// CAPACITY
	// ---------------
	
	template <class T, size_type N>
	size_type circular_buffer<T, N>::size() const noexcept
	{
		return d_size;
	}

	template <class T, size_type N>
	bool circular_buffer<T, N>::empty() const noexcept
	{
		return d_size == 0;
	}

	template <class T, size_type N>
	constexpr size_type circular_buffer<T, N>::max_size() const noexcept
	{
		return BUFFER_SIZE;
	}

	template <class T, size_type N>
	constexpr size_type circular_buffer<T, N>::capacity() const noexcept
	{
		return BUFFER_SIZE;
	}

	// ---------------
	// MODIFIERS
	// ---------------
	template <class T, size_type N>
	void circular_buffer<T, N>::push_back(const T& val) noexcept 
	{
		increment_tail();
		if (d_size > BUFFER_SIZE) {
			increment_head();
		}
		d_container[d_tail] = val;
	}

	template <class T, size_type N>
	void circular_buffer<T, N>::push_back(T&& val) noexcept
	{
		increment_tail();
		if (d_size > BUFFER_SIZE) {
			increment_head();
		}
		d_container[d_tail] = std::forward<T>(val);
	}

	template <class T, size_type N>
	template <class... Args>
	void circular_buffer<T, N>::emplace_back(Args&& ... args) noexcept
	{
		increment_tail();
		if (d_size > BUFFER_SIZE) {
			increment_head();
		}
		d_container[d_tail] = T(std::forward<Args>(args)...);
	}

	template <class T, size_type N>
	void circular_buffer<T, N>::pop_front() noexcept
	{
		increment_head();
	}

	// ---------------
	// PRIVATE
	// ---------------
	template <class T, size_type N>
	void circular_buffer<T, N>::increment_head() noexcept
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

	template <class T, size_type N>
	void circular_buffer<T, N>::increment_tail() noexcept
	{
		++d_tail;
		++d_size;
		if (d_tail == BUFFER_SIZE) {
			d_tail = 0;
		}
	}
}