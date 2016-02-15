#include "position.hh"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE pos
#include <boost/test/unit_test.hpp>

#include <iostream>

using namespace blocks;
using namespace std;

BOOST_AUTO_TEST_CASE( accessors )
{
  cpos pos1, pos2(1, 2, 3);

  BOOST_CHECK( pos1[0] == 0 );
  BOOST_CHECK( pos1[1] == 0 );
  BOOST_CHECK( pos1[2] == 0 );

  BOOST_CHECK( pos2.x() == 1 );
  BOOST_CHECK( pos2.y() == 2 );
  BOOST_CHECK( pos2.z() == 3 );
}

BOOST_AUTO_TEST_CASE( to_idx )
{
  cpos p1(10, 10, 10);
  auto idx = p1.to_idx();
  cpos p2(idx);

  // cout << string(p1) << endl;
  // cout << string(p2) << endl;

  BOOST_CHECK(p1 == p2);

  cpos p3(0, 0, 1);
  cpos p4(0, 1, 0);
  cpos p5(1, 0, 0);

  BOOST_CHECK(p3.to_idx() == 1);
  BOOST_CHECK(p4.to_idx() == 16);
  BOOST_CHECK(p5.to_idx() == 16 * 16);
}

BOOST_AUTO_TEST_CASE( valid )
{
  cpos p1(-1, 0, 0);
  cpos p2(1000, 0, 0);
  cpos p3;

  BOOST_CHECK(!p1.valid());
  BOOST_CHECK(!p2.valid());
  BOOST_CHECK(p3.valid()) ;
}

BOOST_AUTO_TEST_CASE( addition )
{
  cpos p1(1,1,1), p2(2, 2, 2);
  cpos p3 = p1 + p2;

  for (auto i=0; i < 3; i++)
    BOOST_CHECK(p3[i] == 3);
}
