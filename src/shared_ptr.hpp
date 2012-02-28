namespace custom
{

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


template<typename T>
class shared_ptr: private shared_ptr_base
{
public:

	// FIXME: Should this be explicit ?
	shared_ptr() : m_pData(0), m_pRefCount(0)
	{
		m_pRefCount = new RefCount();
		m_pRefCount->AddRef();
	}

	shared_ptr(T* ptr) : m_pData(ptr), m_pRefCount(0)
	{
		m_pRefCount = new RefCount();
		m_pRefCount->AddRef();
	}

	shared_ptr(const shared_ptr<T>& ptr) : m_pData(ptr.m_pData), m_pRefCount(ptr.m_pRefCount)
	{
		m_pRefCount->AddRef();
	}

	~shared_ptr()
	{
		if(m_pRefCount->Release() == 0)
		{
			if(m_pData)
				delete m_pData;
			delete m_pRefCount;
		}
	}

	void swap(shared_ptr<T>& rhs)
	{
		T* pData = rhs.m_pData;
		RefCount* pRefCount = rhs.m_pRefCount;
		rhs.m_pData = this->m_pData;
		rhs.m_pRefCount = this->m_pRefCount;
		this->m_pData = pData;
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
		return *m_pData;
	}

	T* operator->()
	{
		return m_pData;
	}


private:

	T* m_pData;
	RefCount* m_pRefCount;

	template<class X>
	friend bool operator==(const shared_ptr<X>&, const shared_ptr<X>&);

	template<class X>
	friend bool operator!=(const shared_ptr<X>&, const shared_ptr<X>&);
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
	return p1.m_pData == p2.m_pData;
}

template<class T>
inline bool operator!=(const shared_ptr<T>& p1, const shared_ptr<T>& p2)
{
	return p1.m_pData != p2.m_pData;
}


}

