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

	/* Reference count
         * ref_count is exposed for testing only
	 */
	CPPUNIT_ASSERT(sp2.ref_count() == 2);
	
	// Null pointer
	shared_ptr<Dummy> sp3;
	
	CPPUNIT_ASSERT(sp3.isNull() == true);
	CPPUNIT_ASSERT(!sp3 == true);

	// Comparison operators
	CPPUNIT_ASSERT(sp1 == sp2);
	CPPUNIT_ASSERT(!(sp1 != sp2));

	// Derived classes
	shared_ptr<Dummy> derived1(new DummyDerived());
	
	shared_ptr<Dummy> derived2(new Dummy());

	CPPUNIT_ASSERT(derived1 != derived2);

	derived1->m_data = 1;
	derived2 = derived1;
	
	CPPUNIT_ASSERT(derived1->GetData() == 2);
	CPPUNIT_ASSERT(derived2->GetData() == 2);
}

