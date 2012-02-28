#include "../src/shared_ptr.hpp"
#include "shared_ptr_test.h"

using namespace custom;

CPPUNIT_TEST_SUITE_REGISTRATION( SharedPtrTest );

void SharedPtrTest::Test()
{
	shared_ptr<Dummy> sp1(new Dummy());

	// operator ->
	CPPUNIT_ASSERT(sp1->m_data == 0);

	// operator *
	CPPUNIT_ASSERT((*sp1).m_data == 0);
	

	// Assignment
	shared_ptr<Dummy> sp2(new Dummy());
	sp1->m_data = 5;
	sp2 = sp1;

	CPPUNIT_ASSERT(sp2->m_data == 5);
}

