#ifndef __SHARED_PTR_TEST__
#define __SHARED_PTR_TEST__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class SharedPtrTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SharedPtrTest );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
	}

	void tearDown()
	{
	}
	
	void Test();

};

class Dummy
{
public:
	Dummy():m_pData(0), m_data(0)
	{
	}
	
	Dummy(const Dummy& t):m_pData(t.m_pData), m_data(t.m_data)
	{
	}
	
	~Dummy()
	{
		if(m_pData)
			delete m_pData;
	}

	int* m_pData;
	int m_data;
};
	
#endif

