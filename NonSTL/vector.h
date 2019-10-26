// vector.h
// A non-stl header only implementation of std::vector which attempts to adhere to the
// standard interface as closely as possible.
// Note, the standard is used for some components such as std::allocator and exceptions

/*
 * Vectors are sequence containers representing arrays that can change in size
 */

#pragma once

#include <memory> // std::allocator
#include <initializer_list> // std::initializer_list

namespace non_stl
{
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
		// Coefficient for expanding the capacity of the underlying array
		const int beta = 2;

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
		explicit vector(size_t size);
		vector(size_t size, const T& val);

		// Range constructor

		// Copy constructor
		//vector(const vector& rhs);

		// Move constructor
		//vector(vector&& rhs);

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
		// ELEMENT ACCESS
		// ---------------

		// Returns a reference to the element at position n in the vector
		// with no range check
		T& operator[](int n);
		const T& operator[](int n) const;

		// Returns a reference to the element at position n in the vector
		// Function throws an out of range exception if the input is not within range
		// Use the operator[] overload to access without range checking
		T& at(int n);
		const T& at(int n) const;

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

	private:
		// Member variables
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

	template <class T, class Alloc>
	vector<T, Alloc>::vector(size_t size, const T& val) :
		_capacity(beta * size),
		_size(size),
		_data(_alloc.allocate(size))
	{
		for (size_t i = 0; i < size; ++i)
		{
			T cp(val);
			_data[i] = cp;
		}
	}

	template <class T, class Alloc>
	vector<T, Alloc>::~vector()
	{
		_alloc.deallocate(_data, _capacity);
	}

	 template <class T, class Alloc>
	 inline T& vector<T, Alloc>::operator[](int n)
	 {
		 return _data[n];
	 }

	 template <class T, class Alloc>
	 inline const T& vector<T, Alloc>::operator[](int n) const
	 {
		 return _data[n];
	 }

	 template <class T, class Alloc>
	 inline T& vector<T, Alloc>::at(int n)
	 {
		 if (n < 0 || n >= _size)
		 {
			 // throw out of range exception
		 }
		 return _data[n];
	 }

	 template <class T, class Alloc>
	 inline const T& vector<T, Alloc>::at(int n) const
	 {
		 if (n < 0 || n >= _size)
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
}
