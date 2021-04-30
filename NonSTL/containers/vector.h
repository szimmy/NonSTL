// vector.h
// A non-stl header only implementation of std::vector which attempts to adhere to the
// standard interface as closely as possible.
// Note, the standard is used for some components such as std::allocator and exceptions

/*
 * Vectors are sequence containers representing arrays that can change in size
 */

#pragma once

// Includes
#include <algorithm>		// std::copy_n, std::swap
#include <initializer_list>	// std::initializer_list
#include <memory>			// std::allocator
#include <utility>			// std::forward

using size_type = size_t;

namespace non_stl
{
	// Template parameter T is the generic object being stored within the container
	// Template parameter Alloc is the allocator which defines memory allocation for
	// objects of type T. If a custom allocator is not provided then the default 
	// std::allocator<T> will be used which will call the operators 
	// new, new[], delete, delete[]
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
		// Coefficient for expanding the capacity of the underlying array
		const double beta = 2.0;

		// ---------------
		// BEGIN INTERFACE
		// ---------------
	public:
		// ---------------
		// CONSTRUCTORS
		// ---------------

		// Default constructor
		vector();

		// Fill constructor
		// Constructs a vector with size elements
		// Each element is a copy of val if provided
		explicit vector(size_type size);
		vector(size_type size, const T& val);

		// Range constructor
		template <class InputIterator>
		vector(InputIterator first, InputIterator last);

		// Copy constructor
		vector(const vector& rhs);

		// Move constructor
		vector(vector&& rhs) noexcept;

		// Initializer list constructor
		vector(std::initializer_list<T> init);

		// ---------------
		// OPERATOR=
		// ---------------
		vector& operator=(const vector& rhs);
		vector& operator=(vector&& rhs);
		vector& operator=(std::initializer_list<T> init);

		// ---------------
		// DESTRUCTOR
		// ---------------
		~vector();

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
		// ITERATORS
		// ---------------

		// forward declare base iterator types
		class parent_iterator; // TODO better naming for these? parent -> base seems like a weird hierarchy
		class base_iterator;
		class base_const_iterator;
		class forward_iterator;
		class base_reverse_iterator;

		// forward declare returned iterator types
		class iterator;
		class const_iterator;
		class reverse_iterator;
		class const_reverse_iterator;

		// Returns an iterator to the first element of the container
		// If the container is empty, the returned iterator will be equal to end()
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;

		// Returns an iterator to the element following the last element of the container
		// Attempting to access or modify this element results in undefined behavior
		iterator end() noexcept;
		const_iterator end() const noexcept;
		const_iterator cend() const noexcept;

		// Returns a reverse iterator to the first element of the reversed container
		// Equivalent to the last element (not end) of the non-reversed container
		// If the container is empty, the returned iterator will be equal to rend()
		reverse_iterator rbegin() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator crbegin() const noexcept;

		// Returns a reverse iterator to the element following the last element of the reversed container
		// It corresponds to the element preceding the first element of the non-reversed container
		// Attempting to access or modify this element results in undefined behavior
		reverse_iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;
		const_reverse_iterator crend() const noexcept;

		// ---------------
		// CAPACITY
		// ---------------

		// Returns the number of elements in the vector
		size_type size() const noexcept;

		// Return the maximum number of elements the vector can hold
		constexpr size_type max_size() const;

		// Resizes the container so that it contains n elements
		// May reduce or increase the size of vector
		// If the container is expanded and val is provided
		// the new elements of the container will be set to val
		void resize(size_type n);
		void resize(size_type n, const T& val);

		// Returns the size of the storage space currently allocated for the vector,
		// expressed in terms of elements
		size_type capacity() const noexcept;

		// Returns whether the vector is empty 
		// (i.e. whether its size is 0)
		bool empty() const noexcept;

		// Requests that the vector capacity be at least enough to contain n elements
		void reserve(size_type n);

		// Requests the container to reduce its capacity to fit its size
		void shrink_to_fit();

		// ---------------
		// MODIFIERS
		// ---------------

		// Assign new contents to the vector, replacing its current contents
		// and modifying its size if necessary

		// Range version
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last);

		// Fill version
		void assign(size_type n, const T& val);

		// Initializer list version
		void assign(std::initializer_list<T> il);

		// Adds a new element at the end of the vector after its current last element
		void push_back(const T& val);
		void push_back(T&& val);

		// Appends a new element to the end of the container.
		// The arguments args... are forwarded to the constructor as std::forward<Args>(args).... 
		template <class... Args>
		void emplace_back(Args&& ... args);

		// Removes the last element in the vector
		void pop_back();

		// The vector is extended by inserting new elements before the element 
		// at the specified position, effectively increasing the container size
		// by the number of elements inserted

		// Single element
		iterator insert(iterator position, const T& val);

		// Range
		template <class InputIterator>
		iterator insert(iterator position, InputIterator first, InputIterator last);

		// Move 
		iterator insert(iterator position, T&& val);

		// Initializer list
		iterator insert(iterator position, std::initializer_list<T> il);

		// Exchanges the content of the container by the content of x
		// which is another vector object of the same type
		void swap(vector& x);

		// Removes all elements from the vector leaving the container with a size of 0
		void clear() noexcept;

		// ---------------
		// ALLOCATOR
		// ---------------

		// Returns a copy of the allocator object associated with this vector
		Alloc get_allocator() const noexcept;

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

		// Copy items from initializer list into _data
		// Assumed that _data is empty and properly sized
		void copy_from_initializer_list(std::initializer_list<T>& init);

		// Shift the underlying array for the vector by n indicies
		// from idx until the end of the array
		void shift_array(size_type n, size_type idx);

		// Returns an iterator that is n positions from begin
		iterator get_iterator(size_type n);

		template <class InputIterator>
		size_type get_iterator_diff(InputIterator first, InputIterator last);

		// Member variables

		// Allocator object, used to dynamically allocate memory for the arrays
		Alloc _alloc;

		// Maximum storage capacity of the vector. When _size == _capacity the vector
		// will be reallocated and _capacity increased by a factor of beta
		size_type _capacity;

		// The current amount of elements stored within the vector
		size_type _size;

		// Underlying array of template type T which the vector wraps around
		T* _data;
	};

	// BASE ITERATORS

	// Define equivalency operators and all data members
	// for every iterator to derive from
	template <class T, class Alloc>
	class vector<T, Alloc>::parent_iterator
	{
	public:
		parent_iterator(T* ptr, size_type size, size_type idx) :
			_curr(ptr),
			_size(size),
			_idx(idx)
		{
		}

		virtual inline bool operator==(const parent_iterator& rhs) const
		{
			return (isEnd() && rhs.isEnd()) || (*_curr == *rhs._curr);
		}

		virtual inline bool operator!=(const parent_iterator& rhs) const
		{
			return !(*this == rhs);
		}

	protected:
		inline bool isEnd() const
		{
			return _size == _idx;
		}

		T* _curr;
		size_type _size;
		size_type _idx;
	};

	// Define base_iterator which contains the standard access operators
	// for all non_const iterators to derive from
	template <class T, class Alloc>
	class vector<T, Alloc>::base_iterator : virtual public parent_iterator
	{
	public:
		base_iterator(T* ptr, size_type size, size_type idx) :
			parent_iterator(ptr, size, idx)
		{
		}

		virtual T& operator*()
		{
			return *_curr;
		}

		virtual T* operator->()
		{
			return _curr;
		}
	};

	// Define base_const_iterator which contains the constant access operators
	// for all const iterators to derive from
	template <class T, class Alloc>
	class vector<T, Alloc>::base_const_iterator : virtual public parent_iterator
	{
	public:
		base_const_iterator(T* ptr, size_type size, size_type idx) :
			parent_iterator(ptr, size, idx)
		{
		}

		virtual const T& operator*() const
		{
			return *_curr;
		}

		virtual const T* operator->() const
		{
			return _curr;
		}
	};

	// Define forward_iterator which defines the ability to iterate standardly through a container
	// the contrast to this is reverse_iterator
	template <class T, class Alloc>
	class vector<T, Alloc>::forward_iterator : virtual public parent_iterator
	{
	public:
		forward_iterator(T* ptr, size_type size, size_type idx) :
			parent_iterator(ptr, size, idx)
		{
		}

		// Prefix
		virtual forward_iterator operator++(int)
		{
			++_curr;
			++_idx;
			return *this;
		}

		// Postfix
		virtual forward_iterator& operator++()
		{
			_curr++;
			_idx++;
			return *this;
		}

		// Prefix
		virtual forward_iterator operator--(int)
		{
			--_curr;
			--_idx;
			return *this;
		}

		// Postfix
		virtual forward_iterator& operator--()
		{
			_curr--;
			_idx--;
			return *this;
		}
	};

	// Define base_reverse_iterator which defines the ability to iterate through a container the opposite of the
	// operator you are invoking. Increment operator takes you to the iterator before the current iterator.
	// The contrast to this is forward_iterator
	template <class T, class Alloc>
	class vector<T, Alloc>::base_reverse_iterator : virtual public parent_iterator
	{
	public:
		base_reverse_iterator(T* ptr, size_type size, size_type idx) :
			parent_iterator(ptr, size, idx)
		{
		}

		// Prefix
		virtual base_reverse_iterator operator++(int)
		{
			--_curr;
			--_idx;
			return *this;
		}

		// Postfix
		virtual base_reverse_iterator& operator++()
		{
			_curr--;
			_idx--;
			return *this;
		}

		// Prefix
		virtual base_reverse_iterator operator--(int)
		{
			++_curr;
			++_idx;
			return *this;
		}

		// Postfix
		virtual base_reverse_iterator& operator--()
		{
			_curr++;
			_idx++;
			return *this;
		}
	};

	// ITERATOR IMPLEMENTATIONS

	// Iterator definition and impl. Standard iterator is both a base_iterator and forward_iterator
	template <class T, class Alloc>
	class vector<T, Alloc>::iterator : public vector<T, Alloc>::base_iterator, 
									   public vector<T, Alloc>::forward_iterator
	{
	public:
		iterator(T* ptr, size_type size, size_type idx) :
			parent_iterator(ptr, size, idx),
			base_iterator(ptr, size, idx),
			forward_iterator(ptr, size, idx)
		{
		}
	};

	// Const Iterator definition and impl. Const iterator is a base_const_iterator and forward_iterator
	template <class T, class Alloc>
	class vector<T, Alloc>::const_iterator : public vector<T, Alloc>::base_const_iterator, 
											 public vector<T, Alloc>::forward_iterator
	{
	public:
		const_iterator(T* ptr, size_type size, size_type idx) :
			parent_iterator(ptr, size, idx),
			base_const_iterator(ptr, size, idx),
			forward_iterator(ptr, size, idx)
		{
		}
	};

	// Reverse Iterator definition and impl. Reverse iterator is both a base_iterator and base_reverse_iterator
	template <class T, class Alloc>
	class vector<T, Alloc>::reverse_iterator : public vector<T, Alloc>::base_iterator, 
											   public vector<T, Alloc>::base_reverse_iterator
	{
	public:
		reverse_iterator(T* ptr, size_type size, size_type idx) :
			parent_iterator(ptr, size, idx),
			base_iterator(ptr, size, idx),
			base_reverse_iterator(ptr, size, idx)
		{
		}
	};

	// Const Reverse Iterator definition and impl. Const reverse iterator is both a base_cont_iterator and base_reverse_iterator
	template <class T, class Alloc>
	class vector<T, Alloc>::const_reverse_iterator : public vector<T, Alloc>::base_const_iterator,
													 public vector<T, Alloc>::base_reverse_iterator
	{
	public:
		const_reverse_iterator(T* ptr, size_type size, size_type idx) :
			parent_iterator(ptr, size, idx),
			base_const_iterator(ptr, size, idx),
			base_reverse_iterator(ptr, size, idx)
		{
		}
	};

	// VECTOR IMPL

	// ---------------
	// CONSTRUCTORS
	// ---------------
	template <class T, class Alloc>
	vector<T, Alloc>::vector() :
		_capacity(10),
		_size(0),
		_data(_alloc.allocate(_capacity))
	{

	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_type size) :
		_capacity((size_type)(beta* size)),
		_size(size),
		_data(_alloc.allocate(size))
	{
		for (auto i = 0; i < size; ++i)
		{
			_alloc.construct((_data + i), T());
		}
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_type size, const T& val) :
		_capacity((size_type)(beta * size)),
		_size(size),
		_data(_alloc.allocate(_capacity))
	{
		for (auto i = 0; i < size; ++i)
		{
			_alloc.construct((_data + i), val);
		}
	}

	template <class T, class Alloc>
	template <class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) :
		_capacity(0),
		_size(0)
	{
		if constexpr (std::is_integral<InputIterator>::value) {
			_capacity = (size_type)(beta * first);
			_size = first;
			_data = _alloc.allocate(_capacity);

			for (auto i = 0; i < first; ++i)
			{
				_alloc.construct((_data + i), last);
			}
		}
		else {
			_size = get_iterator_diff(first, last);
			_capacity = (size_type)(beta * _size);
			_data = _alloc.allocate(_capacity);

			auto count = 0;
			for (auto it2 = first; it2 != last; ++it2)
			{
				_data[count++] = *it2;
			}
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
	vector<T, Alloc>::vector(vector<T, Alloc>&& rhs) noexcept :
		_alloc(rhs._alloc),
		_capacity(rhs._capacity),
		_size(rhs._size),
		_data(rhs._data)
	{
		// set the rvalue to an undefined state since we have moved from it
		rhs._capacity = 0;
		rhs._size = 0;
		rhs._data = nullptr;
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(std::initializer_list<T> init) :
		_capacity((size_type)(beta * init.size())),
		_size(init.size()),
		_data(_alloc.allocate(_capacity))
	{
		copy_from_initializer_list(init);
	}

	// ---------------
	// OPERATOR=
	// ---------------
	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& rhs)
	{
		// Deallocate currently allocated data
		~vector();

		// Assign member variables
		_alloc = rhs._alloc;
		_capacity = rhs._capacity;
		_size = rhs._size;

		// Initialize proper sized container and copy elements
		_data = _alloc.allocate(_capacity);
		std::copy_n(rhs_data, _size, _data);
	}

	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector<T, Alloc>&& rhs)
	{
		// Swap all variables with the rvalue
		// The destructor will be called on rvalue which will
		// clean up the memory allocated to the original vector
		swap(rhs);
	}

	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<T> init)
	{
		// Deallocate current allocated data
		~vector();

		// Assign member variables
		// Note alloc will remain as is already assigned
		_size = init.size();
		_capacity = (size_type)beta * _size;

		// Initialize proper sized container
		_data = _alloc.allocate(_capacity);

		// Copy data from initializer list into _data
		copy_from_initializer_list(init);
	}

	// ---------------
	// DESTRUCTOR
	// ---------------
	template <class T, class Alloc>
	vector<T, Alloc>::~vector()
	{
		if (_data)
		{
			// Destruct every element contained in the vector
			for (auto i = 0; i < _size; ++i)
			{
				_data[i].~T();
			}

			// Deallocate _data
			_alloc.deallocate(_data, _capacity);
			_data = nullptr;
			_capacity = 0;
			_size = 0;
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
	// ITERATORS
	// ---------------

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::iterator vector<T, Alloc>::begin() noexcept
	{
		return vector<T, Alloc>::iterator(&_data[0], _size, 0);
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::begin() const noexcept
	{
		return vector<T, Alloc>::const_iterator(&_data[0], _size, 0);
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cbegin() const noexcept
	{
		return begin();
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::iterator vector<T, Alloc>::end() noexcept
	{
		return vector<T, Alloc>::iterator(&_data[_size], _size, _size);
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::end() const noexcept
	{
		return vector<T, Alloc>::const_iterator(&_data[_size], _size, _size);
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cend() const noexcept
	{
		return end();
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rbegin() noexcept
	{
		// Size for a reverse iterator is effectively -1 (one past normal beginning)
		return vector<T, Alloc>::reverse_iterator(&_data[_size - 1], -1, _size-1);
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::rbegin() const noexcept
	{
		// Size for a reverse iterator is effectively -1 (one past normal beginning)
		return vector<T, Alloc>::const_reverse_iterator(&_data[_size - 1], -1, _size-1);
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crbegin() const noexcept
	{
		return rbegin();
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rend() noexcept
	{
		// Size for a reverse iterator is effectively -1 (one past normal beginning)
		return vector<T, Alloc>::reverse_iterator(&_data[-1], -1, -1);
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::rend() const noexcept
	{
		// Size for a reverse iterator is effectively -1 (one past normal beginning)
		return vector<T, Alloc>::const_reverse_iterator(&_data[-1], -1, -1);
	}

	template <class T, class Alloc>
	inline typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crend() const noexcept
	{
		return rend();
	}

	// ---------------
	// CAPACITY
	// ---------------
	template <class T, class Alloc>
	inline size_type vector<T, Alloc>::size() const noexcept
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
			_size = n;
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::resize(size_type n, const T& val)
	{
		const auto old_size = _size;
		resize(n);

		// Need to assign the new values of the resized vector to val
		if (n >= old_size)
		{
			for (auto i = old_size - 1; i < _size; ++i)
			{
				_alloc.construct((_data + i), val);
			}
		}
	}

	template <class T, class Alloc>
	inline size_type vector<T, Alloc>::capacity() const noexcept
	{
		return _capacity;
	}

	template <class T, class Alloc>
	inline bool vector<T, Alloc>::empty() const noexcept
	{
		return size() == 0;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n)
	{
		if (n <= _capacity)
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
	// MODIFIERS
	// ---------------
	template <class T, class Alloc>
	template <class InputIterator>
	void vector<T, Alloc>::assign(InputIterator first, InputIterator last)
	{
		// Need to clear out the elements currently assigned anyway so do it first
		clear();

		if constexpr (std::is_integral<InputIterator>::value) {
			if (first >= _capacity)
			{
				// Need to reallocate the inner array
				reallocate((size_type)(beta * first));
			}

			// Assign size of vector
			_size = first;

			// Assign each new element to val
			for (auto i = 0; i < first; ++i)
			{
				_alloc.construct((_data + i), last);
			}
		}
		else {
			// Need to obtain the size of how many elements are in the range of iterators
			auto n = get_iterator_diff(first, last);

			if (n >= _capacity)
			{
				// Need to reallocate the inner array
				reallocate((size_type)(beta * n));
			}

			// Assign size of vector
			_size = n;

			n = 0;

			// Populate data
			for (auto it2 = first; it2 != last; ++it2)
			{
				_data[n++] = *it2;
			}
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::assign(size_type n, const T& val)
	{
		// Need to clear out the elements currently assigned anyway so do it first
		clear();

		if (n >= _capacity)
		{
			// Need to reallocate the inner array
			reallocate((size_type)(beta * n));
		}

		// Assign size of vector
		_size = n;

		// Assign each new element to val
		for (auto i = 0; i < n; ++i)
		{
			_alloc.construct((_data + i), val);
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::assign(std::initializer_list<T> il)
	{
		// Need to clear out elements currently assigned anyway so do it first
		clear();

		auto n = il.size();

		if (n >= _capacity)
		{
			// Need to reallocate the inner array
			reallocate((size_type)(beta * n));
		}

		// Assign size of vector
		_size = n;

		// Assign each new element
		copy_from_initializer_list(il);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& val)
	{
		// Check for reallocation
		if (_size == _capacity)
		{
			reallocate((size_type)(beta * _capacity));
		}

		_alloc.construct((_data + _size++), val);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(T&& val)
	{
		// Check for reallocation
		if (_size == _capacity)
		{
			reallocate((size_type)(beta * _capacity));
		}

		_alloc.construct((_data + _size++), std::forward<T>(val));
	}

	template <class T, class Alloc>
	template <class... Args>
	void vector<T, Alloc>::emplace_back(Args&& ... args)
	{
		// Check for reallocation
		if (_size == _capacity)
		{
			reallocate((size_type)(beta * _capacity));
		}

		_alloc.construct((_data + _size++), std::forward<Args>(args)...);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back()
	{
		// Call destructor on the last element in the vector
		// and decrement size so we can write over it later
		_data[_size - 1].~T();
		--_size;
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const T& val)
	{
		const auto idx = &(*position) - _data;

		if (_size == _capacity)
		{
			// Need to reallocate
			reallocate((size_type)(beta * _capacity));
		}

		// Need to shift the array over by n indicies from the end until idx
		shift_array(1, idx);

		_alloc.construct((_data + idx), val);

		return get_iterator(idx);
	}

	template <class T, class Alloc>
	template <class InputIterator>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last)
	{
		const auto idx = &(*position) - _data;
		const auto n = get_iterator_diff(first, last);
		const auto old_size = _size;

		if (_size + n >= _capacity)
		{
			// Need to reallocate
			reallocate((size_type)(beta * (_size + n)));
		}

		// Need to shift the array over by n indicies
		shift_array(n, idx);

		auto it = first;
		for (auto i = idx; i < old_size; ++i)
		{
			_data[i] = *(it);
			++it;
		}

		return get_iterator(idx);
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, T&& val)
	{
		const auto idx = &(*position) - _data;

		if (_size == _capacity)
		{
			// Need to reallocate
			reallocate((size_type)(beta * _capacity));
		}

		// Need to shift the array over by n indicies from the end until idx
		shift_array(1, idx);

		_alloc.construct((_data + idx), val);

		return get_iterator(idx);
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, std::initializer_list<T> il)
	{
		const auto idx = &(*position) - _data;
		const auto n = il.size();

		if (_size + n >= _capacity)
		{
			// Need to reallocate
			reallocate((size_type)(beta * (_size + n)));
		}

		// Need to shift the array over by n indicies
		shift_array(n, idx);

		auto i = idx;
		for (auto& it : il)
		{
			_data[idx++] = it;
		}

		return get_iterator(idx);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::swap(vector < T, Alloc>& x)
	{
		std::swap(_alloc, x._alloc);
		std::swap(_capacity, x._capacity);
		std::swap(_size, x._size);
		std::swap(_data, x._data);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::clear() noexcept
	{
		// pop_back_n(_size) would be a cleaner
		// implementation in terms of reuse
		// but holds the overhead of performing
		// _size decremenets when we know we are
		// going all the way to 0 every time
		for (auto i = 0; i < _size; ++i)
		{
			_data[i].~T();
		}
		_size = 0;
	}

	// ---------------
	// ALLOCATOR
	// ---------------

	template <class T, class Alloc>
	Alloc vector<T, Alloc>::get_allocator() const noexcept
	{
		return _alloc;
	}

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
		auto cp = _alloc.allocate(cap);

		// Only copy_n if we have elements to copy
		if(_size > 0)
		{
			std::copy_n(_data, _size, cp);
		}

		// Deallocate old array and reassign member variables
		_alloc.deallocate(_data, _capacity);
		_data = cp;
		_capacity = cap;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back_n(size_type n)
	{
		for (auto i = 0; i < n; ++i)
		{
			pop_back();
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::copy_from_initializer_list(std::initializer_list<T>& init)
	{
		auto count = 0;
		for (auto& it : init)
		{
			_alloc.construct((_data + count++), it);
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::shift_array(size_type n, size_type idx)
	{
		auto copy_to = _size + n;
		auto copy_from = _size;
		auto complete = false;

		while(copy_from >= idx && !complete)
		{
			_data[copy_to--] = _data[copy_from];
			if (copy_from == 0)
			{
				complete = true;
			}
			--copy_from;
		}

		// Adjust the size counter for the vector
		_size += n;
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::get_iterator(size_type n)
	{
		// Get the difference from the index to the end of the vector
		auto diff = _size - n;

		vector<T, Alloc>::iterator it = diff < n ? end() : begin();

		// Decide whether to iterate from the front or back of the vector
		if (diff < n)
		{
			// Shorter to iterate from the back
			it = end();

			// Decrease the iterator diff times
			for (auto i = 0; i < diff; ++i)
			{
				--it;
			}
		}
		else
		{
			// Short to iterate from the front
			it = begin();

			// Increase the iterator n times
			for (auto i = 0; i < n; ++i)
			{
				++it;
			}
		}

		return it;
	}

	template <class T, class Alloc>
	template <class InputIterator>
	size_type vector<T, Alloc>::get_iterator_diff(InputIterator first, InputIterator last)
	{
		auto ret = 0;
		for (auto it = first; it != last; it++)
		{
			++ret;
		}
		return ret;
	}
}
