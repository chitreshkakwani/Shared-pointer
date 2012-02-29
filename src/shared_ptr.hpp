/**
  * Ref counted shared pointer
  * 
  * Author : Chitresh Kakwani <chitreshkakwani@gmail.com>
  *
  */

namespace custom
{

enum EPointerType
{
	EPointerTypePtr,
	EPointerTypeArray
};

class RefCount
{
public:

	RefCount():m_refCount(0)
	{
	}

	void AddRef()
	{
		m_refCount++;
	}

	int Release()
	{
		return --m_refCount;
	}

	int count()
	{
		return m_refCount;
	}

private:

	int m_refCount;
};

/**
  * Templated class shared_ptr inherits from non-templated base class shared_ptr_base.
  * shared_ptr_base declares comparison operators as private to disallow mixed type comparisons.
  * Templated derived classes override the comparison operators which makes comparisons between 
  * same shared_ptr types possible.
  * 
  * 	shared_ptr<A> a1;
  *	shared_ptr<A> a2;
  *     shared_ptr<B> b;
  *	...
  *	
  *	if( a1 == a2 )  // Works fine
  *	...
  *	
  *	if( a1 == b )  // Error! operator == is private
  */
class shared_ptr_base
{
private:

	void operator==(const shared_ptr_base&) const;

	void operator!=(const shared_ptr_base&) const;
};


template<typename T, EPointerType ptrType = EPointerTypePtr>
class shared_ptr: private shared_ptr_base
{

public:

	// FIXME: Should this be explicit ?
	shared_ptr() : m_pointer(0), m_pRefCount(0)
	{
		m_pRefCount = new RefCount();
		m_pRefCount->AddRef();
	}

	shared_ptr(T* ptr) : m_pointer(ptr), m_pRefCount(0)
	{
		m_pRefCount = new RefCount();
		m_pRefCount->AddRef();
	}

	shared_ptr(const shared_ptr<T>& ptr) : m_pointer(ptr.m_pointer), m_pRefCount(ptr.m_pRefCount)
	{
		m_pRefCount->AddRef();
	}

	~shared_ptr()
	{
		if(m_pRefCount->Release() == 0)
		{
			if(m_pointer)
			{
				if(ptrType == EPointerTypePtr)
					delete m_pointer;
				else
					delete[] m_pointer;
			}
			delete m_pRefCount;
		}
	}

	bool isNull() const
	{
		return m_pointer == 0;
	}

	bool operator!() const
	{
		return m_pointer == 0;
	}

	void swap(shared_ptr<T>& rhs)
	{
		T* pData = rhs.m_pointer;
		RefCount* pRefCount = rhs.m_pRefCount;
		rhs.m_pointer = this->m_pointer;
		rhs.m_pRefCount = this->m_pRefCount;
		this->m_pointer = pData;
		this->m_pRefCount = pRefCount;
	}

	shared_ptr<T>& operator=(const shared_ptr<T>& rhs)
	{
		/*
		 * Copy-and-swap idiom
		 * Make a copy of rhs using the copy ctor, swap the values of the copy
		 * with this. The destruction of the swapped copy is then taken care of
		 * by the destructor
                 */
		shared_ptr<T> c(rhs);
		c.swap(*this);
		return *this;
	}

	T& operator*()
	{
		return *m_pointer;
	}

	T* operator->()
	{
		return m_pointer;
	}

	int ref_count()
	{
		return m_pRefCount->count();
	}

	template<class X>
	friend bool operator==(const shared_ptr<X>&, const shared_ptr<X>&);

	template<class X>
	friend bool operator!=(const shared_ptr<X>&, const shared_ptr<X>&);

private:
	
	T* m_pointer;
	RefCount* m_pRefCount;

};


/**
  * To avoid conversion to dumb pointers in situations like this:
  * 
  * 	if(shared == dumb)    // if( shared == static_cast< shared_ptr<T> >(dumb))
  *
  * comparison operators are declared as non-members
  */
template<class T>
inline bool operator==(const shared_ptr<T>& p1, const shared_ptr<T>& p2)
{
	return p1.m_pointer == p2.m_pointer;
}

template<class T>
inline bool operator!=(const shared_ptr<T>& p1, const shared_ptr<T>& p2)
{
	return p1.m_pointer != p2.m_pointer;
}


}


