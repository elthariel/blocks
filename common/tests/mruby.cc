#include "mruby.hh"
#include "mruby_value.hh"
#include "mruby_containers.hh"

using namespace blocks::ruby;

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE mruby
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( basic )
{
  mruby mrb(boost::filesystem::current_path().native());

  auto res = mrb.eval("true");
  BOOST_CHECK((bool)res);
  BOOST_CHECK(!res.is_nil());

  res = mrb.eval("'test'");
  BOOST_CHECK(res.is_string());
  BOOST_CHECK(res.as_string() == "test");

  res = mrb.eval("[]");
  BOOST_CHECK(res.is_array());

  res = mrb.eval("{}");
  BOOST_CHECK(res.is_hash());
}

BOOST_AUTO_TEST_CASE( exception )
{
  mruby mrb(boost::filesystem::current_path().native());

  mrb.eval("raise 'test'");

  auto ex = mrb.exception();
  BOOST_CHECK(ex.is_exception());
  BOOST_CHECK(ex.classname() == "RuntimeError");
}

BOOST_AUTO_TEST_CASE( send )
{
  mruby mrb(boost::filesystem::current_path().native());

  mrb.eval("def test(x); \"test#{x}\"; end");

  arguments<1> argv = {{mrb.make(42)}};
  auto res = mrb.root().send<1>("test", argv);

  BOOST_CHECK(res.is_string());
  BOOST_CHECK(res.as_string() == "test42");
}
