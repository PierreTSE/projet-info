#ifndef COLLECTION_HPP
#define COLLECTION_HPP

template <typename T>
class Collection
{
public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;



	virtual reference front() = 0;
	virtual const_reference front() const = 0;
	virtual reference back() = 0;
	virtual const_reference back() const = 0;

};

#endif // !COLLECTION_HPP
