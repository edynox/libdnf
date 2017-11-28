#ifndef LIBDNF_SWDBHANDLETEST_HPP
#define LIBDNF_SWDBHANDLETEST_HPP

#include "handle.hpp"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class HandleTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE (HandleTest);
    CPPUNIT_TEST (testName);
    CPPUNIT_TEST (testStream);
    CPPUNIT_TEST (testVersion);
    CPPUNIT_TEST_SUITE_END ();

  public:
    void setUp () override;
    void tearDown () override;

    void testName ();
    void testStream ();
    void testVersion ();

  private:
    HandlePackage *handle;
};

#endif // LIBDNF_PACKAGETEST_HPP
