// vector.h
// A non-stl header only implementation of std::vector which attempts to adhere to the
// standard interface as closely as possible.
// Note, the standard is used for some components such as std::allocator and exceptions

/*
 * Vectors are sequence containers representing arrays that can change in size
 */

#pragma once

// Includes
#include <algorithm>		// std::copy_n
#include <initializer_list> // std::initializer_list
#include <memory>			// std::allocator
#include <utility>			// std::forward

using size_type = size_t;

namespace non_stl
{
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
		// Coefficient for expanding the capacity of the underlying array
		const double beta = 1.5;

		// ---------------
		// BEGIN INTERFACE
		// ---------------
	public:
		// ---------------
		// CONSTRUCTORS
		// ---------------

		// Default constructor
		vector() = default;

		// Fill constructor
		// Constructs a vector with size elements
		// Each element is a copy of val if provided
		explicit vector(size_type size);
		vector(size_type size, const T& val);

		// Range constructor

		// Copy constructor
		vector(const vector& rhs);

		// Move constructor
		vector(vector&& rhs);

		// Initializer list constructor
		//vector(std::initializer_list<T> init);

		// ---------------
		// OPERATOR=
		// ---------------
		//vector& operator=(const vector& rhs);
		//vector& operator=(vector&& rhs);
		//vector& operator=(std::initializer_list<T> init);

		// ---------------
		// DESTRUCTOR
		// ---------------
		~vector();

		// ---------------
		// ITERATORS
		// ---------------

		// ---------------
		// CAPACITY
		// ---------------

		// Returns the number of elements in the vector
		size_type size() const;

		// Return the maximum number of elements the vector can hold
		constexpr size_type max_size() const;

		// Resizes the container so that it contains n elements
		// May reduce or increase the size of vector
		void resize(size_type n);
		void resize(size_type n, const T& val);

		// Returns the size of the storage space currently allocated for the vector,
		// expressed in terms of elements
		size_type capacity() const;

		// Returns whether the vector is empty 
		//(i.e. whether its size is 0)
		bool empty() const;

		// Requests that the vector capacity be at least enough to contain n elements
		void reserve(size_type n);

		// Requests the container to reduce its capacity to fit its size
		void shrink_to_fit();

		// ---------------
		// ELEMENT ACCESS
		// ---------------

		// Returns a reference to the element at position n in the vector
		// with no range check
		T& operator[](size_type n);
		const T& operator[](size_type n) const;

		// Returns a reference to the element at position n in the vector
		// Function throws an out of range exception if the input is not within range
		// Use the operator[] overload to access without range checking
		T& at(size_type n);
		const T& at(size_type n) const;

		// Returns a reference to the first element in the vector
		T& front();
		const T& front() const;

		// Returns a reference to the last element in the vector
		T& back();
		const T& back() const;

		// Returns a direct pointer to the memory array used internally by the vector
		// to store its owned elements
		T* data();
		const T* data() const;

		// ---------------
		// MODIFIERS
		// ---------------

		// Assign new contents to the vector, replacing its current contents
		// and modifying its size if necessary

		// Range version

		// Fill version
		//void assign(size_type n, const T& val);

		// Initializer list version
		//void assign(std::initializer_list<T> il);

		// Adds a new element at the end of the vector after its current last element
		void push_back(const T& val);
		void push_back(T&& val);

		void pop_back();

		// ---------------
		// ALLOCATOR
		// ---------------

		// ---------------
		// NON MEMBER FUNCTION
		// OVERLOADS
		// ---------------

	private:
		// Private functions

		// Reallocate _data to be of capacity cap
		// Copy all elements from _data over and assign _capacity = cap
		void reallocate(size_type cap);

		// Call pop_back n times
		void pop_back_n(size_type n);

		// Member variables
		Alloc _alloc;
		size_type _capacity;
		size_type _size;
		T* _data;
	};

	// ---------------
	// CONSTRUCTORS
	// ---------------
	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_type size) :
		_capacity(size),
		_size(0),
		_data(_alloc.allocate(size))
	{

	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_type size, const T& val) :
		_capacity((size_type)(beta * size)),
		_size(size),
		_data(_alloc.allocate(size))
	{
		for (size_type i = 0; i < size; ++i)
		{
			// TODO EMPLACE CONSTRUCTION HERE
			T cp(val);
			_data[i] = cp;
		}
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(const vector<T, Alloc>& rhs) :
		_alloc(rhs._alloc),
		_capacity(rhs._capacity),
		_size(rhs._size),
		_data(_alloc.allocate(_size))
	{
		std::copy_n(rhs._data, _size, _data);
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(vector<T, Alloc>&& rhs) :
		_alloc(rhs._alloc),
		_capacity(rhs._capacity),
		_size(rhs._size),
		_data(rhs._data)
	{
		rhs._capacity = 0;
		rhs._size = 0;
		rhs._data = nullptr;
	}

	// ---------------
	// OPERATOR=
	// ---------------

	// ---------------
	// DESTRUCTOR
	// ---------------
	template <class T, class Alloc>
	vector<T, Alloc>::~vector()
	{
		if (_data)
		{
			_alloc.deallocate(_data, _capacity);
		}
	}

	// ---------------
	// ITERATORS
	// ---------------

	// ---------------
	// CAPACITY
	// ---------------
	template <class T, class Alloc>
	inline size_type vector<T, Alloc>::size() const
	{
		return _size;
	}

	template <class T, class Alloc>
	constexpr size_type vector<T, Alloc>::max_size() const
	{
		// TODO probably not a completely accurate measure
		// due to hitting memory constraints long before having this many elements
		return std::numeric_limits<size_type>::max();
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::resize(size_type n)
	{
		// Reduce content to first n elements
		// Discarding any others
		if (n < _size)
		{
			// Pop back on the vector (_size - n) times
			pop_back_n(_size - n);
		}
		else if (n <= _capacity)
		{
			// Insert at the end as many elements as needed to reach size n
			// No element specified to assign, so leave uninitialized memory
			_size = n;
		}
		else
		{
			// Perform reallocation of the array
			reallocate(n);
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::resize(size_type n, const T& val)
	{
		size_type old_size = _size;
		resize(n);

		// Need to assign the new values of the resized vector to val
		if (n >= old_size)
		{
			for (size_type i = old_size - 1; i < _size; ++i)
			{
				// TODO EMPLACE CONSTRUCTION HERE
				T cp(val);
				_data[i] = cp;
			}
		}
	}

	template <class T, class Alloc>
	inline size_type vector<T, Alloc>::capacity() const
	{
		return _capacity;
	}

	template <class T, class Alloc>
	inline bool vector<T, Alloc>::empty() const
	{
		return size() == 0;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n)
	{
		if (n <= _size)
		{
			return;
		}
		
		reallocate(n);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::shrink_to_fit()
	{
		// If capacity is bigger than size then reallocate to _size
		// calling reallocate is safe since we are allocating the new
		// array to be of size _size so we won't go into memory we
		// aren't supposed to
		if (_capacity > _size)
		{
			reallocate(_size);
		}
	}

	// ---------------
	// ELEMENT ACCESS
	// ---------------
	template <class T, class Alloc>
	inline T& vector<T, Alloc>::operator[](size_type n)
	{
		return _data[n];
	}

	template <class T, class Alloc>
	inline const T& vector<T, Alloc>::operator[](size_type n) const
	{
		return _data[n];
	}

	template <class T, class Alloc>
	inline T& vector<T, Alloc>::at(size_type n)
	{
		// n < 0 not allowed due to unsigned typing
		if (n >= _size)
		{
			// throw out of range exception
		}
		return _data[n];
	}

	template <class T, class Alloc>
	inline const T& vector<T, Alloc>::at(size_type n) const
	{
		// n < 0 not allowed due to unsigned typing
		if (n >= _size)
		{
			// throw out of range exception
		}
		return _data[n];
	}

	template <class T, class Alloc>
	inline T& vector<T, Alloc>::front()
	{
		return _data[0];
	}

	template <class T, class Alloc>
	inline const T& vector<T, Alloc>::front() const
	{
		return _data[0];
	}

	template <class T, class Alloc>
	inline T& vector<T, Alloc>::back()
	{
		return _data[_size - 1];
	}

	template <class T, class Alloc>
	inline const T& vector<T, Alloc>::back() const
	{
		return _data[_size - 1];
	}

	template <class T, class Alloc>
	inline T* vector<T, Alloc>::data()
	{
		return _data;
	}

	template <class T, class Alloc>
	inline const T* vector<T, Alloc>::data() const
	{
		return _data;
	}

	// ---------------
	// MODIFIERS
	// ---------------
	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& val)
	{
		if (_size == _capacity)
		{
			reallocate((size_type)beta * _capacity);
		}

		T cp(val);
		_data[_size++] = cp;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(T&& val)
	{
		if (_size == _capacity)
		{
			reallocate((size_type)beta * _capacity);
		}

		T cp(std::forward<T>(val+1));
		_data[_size++] = cp;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back()
	{
		// Call destructor on the last element in the vector
		// and decrement size so we can write over it later
		_data[_size - 1].~T();
		--_size;
	}

	// ---------------
	// ALLOCATOR
	// ---------------

	// ---------------
	// NON MEMBER FUNCTION
	// OVERLOADS
	// ---------------

	// ---------------
	// PRIVATE
	// ---------------
	template <class T, class Alloc>
	void vector<T, Alloc>::reallocate(size_type cap)
	{
		// Allocate new array and copy over data
		T* cp = _alloc.allocate(cap);
		std::copy_n(_data, _size, cp);

		// Deallocate old array and reassign member variables
		_alloc.deallocate(_data, _capacity);
		_data = cp;
		_capacity = cap;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back_n(size_type n)
	{
		for (size_type i = 0; i < n; ++i)
		{
			pop_back();
		}
	}
}
