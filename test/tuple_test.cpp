#define BOOST_TEST_MODULE tuple test
#include <boost/test/included/unit_test.hpp>

#include <tuple>
#include <vector>
#include <type_traits>

#include <pastel/utility/tuple/meta/all_of.hpp>
#include <pastel/utility/tuple/meta/any_of.hpp>
#include <pastel/utility/tuple/meta/none_of.hpp>

#include <pastel/utility/tuple/meta/adjacent_find.hpp>

#include <pastel/utility/tuple/meta/copy.hpp>
#include <pastel/utility/tuple/meta/copy_n.hpp>


BOOST_AUTO_TEST_CASE(all_of_test)
{
  using tuple_type = std::tuple<int, double, double, float, short>;
  BOOST_TEST((pastel::utility::tuple::meta::all_of<tuple_type, std::is_pod>::value));
}

BOOST_AUTO_TEST_CASE(any_of_test)
{
  using tuple_type = std::tuple<std::vector<int>, double, std::vector<double>>;
  BOOST_TEST((pastel::utility::tuple::meta::any_of<tuple_type, std::is_pod>::value));
}

BOOST_AUTO_TEST_CASE(none_of_test)
{
  using tuple_type = std::tuple<std::vector<int>, std::vector<float>, std::vector<double>>;
  BOOST_TEST((pastel::utility::tuple::meta::none_of<tuple_type, std::is_pod>::value));
}

BOOST_AUTO_TEST_CASE(adjacent_find_test)
{
  using tuple_type = std::tuple<int, double, double, float, short>;
  BOOST_TEST(pastel::utility::tuple::meta::adjacent_find<tuple_type>::value == 1u);
}

BOOST_AUTO_TEST_CASE(copy_test)
{
  using tuple1_type = std::tuple<int, double, double, float, short>;
  using tuple2_type = std::tuple<int, double, float>;
  BOOST_TEST((std::is_same<typename pastel::utility::tuple::meta::copy<tuple1_type, tuple2_type>::type, tuple1_type>::value));
  using tuple3_type = std::tuple<int, double, float, short, long double, long int>;
  using tuple4_type = std::tuple<int, double, double, float, short, long int>;
  BOOST_TEST((std::is_same<typename pastel::utility::tuple::meta::copy<tuple1_type, tuple3_type>::type, tuple4_type>::value));
}

BOOST_AUTO_TEST_CASE(copy_n_test)
{
  using tuple1_type = std::tuple<int, double, double, float, short>;
  using tuple2_type = std::tuple<double, int, float>;
  using tuple3_type = std::tuple<int, double, float>;
  BOOST_TEST((std::is_same<typename pastel::utility::tuple::meta::copy_n<tuple1_type, 2u, tuple2_type>::type, tuple3_type>::value));
  using tuple4_type = std::tuple<int, double, double, float>;
  BOOST_TEST((std::is_same<typename pastel::utility::tuple::meta::copy_n<tuple1_type, 4u, tuple3_type>::type, tuple4_type>::value));
}

