#include "common/block.hh"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Block
#include <boost/test/unit_test.hpp>

#include <iostream>

using namespace blocks;
using namespace std;

BOOST_AUTO_TEST_CASE( Block_accessors )
{
  Block b(0, 12);

  b.id(123);
  BOOST_CHECK( b.id() == 123 );

  BOOST_CHECK( b.variant() == 12 );

  BOOST_CHECK( b.air() == false );
  b.air(true);
  BOOST_CHECK( b.air() == true );

  BOOST_CHECK( b.transparent() == false );
  b.transparent(true);
  BOOST_CHECK( b.transparent() == true );
  BOOST_CHECK( b.air() == true );

  b.transparent(false);
  BOOST_CHECK( b.transparent() == false );
  BOOST_CHECK( b.air() == true );

  b.id(234);
  BOOST_CHECK( b.id() == 234 );
}
