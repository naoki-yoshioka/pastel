#define BOOST_TEST_MODULE geometry test
#include <boost/test/included/unit_test.hpp>

#include <cmath>

#include <pastel/geometry/vector.hpp>
#include <pastel/geometry/vector_io.hpp>
#include <pastel/geometry/point.hpp>
#include <pastel/geometry/point_io.hpp>
#include <pastel/geometry/get.hpp>
#include <pastel/geometry/dot_product.hpp>
#include <pastel/geometry/cross_product.hpp>
#include <pastel/geometry/squared_norm.hpp>
#include <pastel/geometry/norm.hpp>
#include <pastel/geometry/meta/dimension_of.hpp>


BOOST_AUTO_TEST_CASE(geometry_1dim_test, * boost::unit_test::tolerance(0.000001))
{
  using vector_type = pastel::geometry::vector<1u, double>;
  using point_type = pastel::geometry::point<1u, double>;
  BOOST_TEST(pastel::geometry::meta::dimension_of<vector_type>::value == 1u);
  BOOST_TEST(pastel::geometry::meta::dimension_of<point_type>::value == 1u);

  auto const vec1 = vector_type{1.0};
  auto const vec2 = vector_type{2.0};
  BOOST_TEST(pastel::geometry::get<0>(vec1) == 1.0);
  BOOST_TEST(vec1 + vec1 == vec2);
  BOOST_TEST(vec1 - vec2 == -vec1);
  BOOST_TEST(2.0 * vec1 == vec2);
  BOOST_TEST(vec1 * 2.0 == vec2);
  BOOST_TEST(vec2 / 2.0 == vec1);
  BOOST_TEST(pastel::geometry::dot_product(vec1, vec2) == 2.0);
  BOOST_TEST(pastel::geometry::squared_norm(vec1) == 1.0);
  using std::sqrt;
  BOOST_TEST(pastel::geometry::norm(vec1) == sqrt(1.0));
  BOOST_TEST(pastel::geometry::cross_product(vec1, vec2) == 0.0);

  auto const pnt1 = point_type{1.0};
  auto const pnt2 = point_type{3.0};
  BOOST_TEST(pastel::geometry::get<0>(pnt1) == 1.0);
  BOOST_TEST(pnt1 + vec2 == pnt2);
  BOOST_TEST(vec2 + pnt1 == pnt2);
  BOOST_TEST(pnt2 - vec2 == pnt1);
  BOOST_TEST(pnt2 - pnt1 == vec2);

  auto vec3 = vector_type{0.0};
  vec3.fill(1.0);
  BOOST_TEST((vec3 == vector_type{1.0}));
  auto vec4 = vec2;
  using std::swap;
  swap(vec3, vec4);
  BOOST_TEST(vec3 == vec2);

  for (auto& component: vec3)
    component = 1.0;
  BOOST_TEST((vec3 == vector_type{1.0}));
  for (auto iter = vec3.rbegin(), last = vec3.rend(); iter != last; ++iter)
    *iter = 2.0;
  BOOST_TEST((vec3 == vector_type{2.0}));
  BOOST_TEST(vec1.size() == pastel::geometry::meta::dimension_of<vector_type>::value);
  BOOST_TEST(vec1.max_size() == pastel::geometry::meta::dimension_of<vector_type>::value);
  BOOST_TEST(!vec1.empty());
  BOOST_TEST(vec1.at(0u) == vec1.front());
  BOOST_TEST(vec1.at(vec1.size()-1u) == vec1.back());

  auto pnt3 = point_type{0.0};
  pnt3.fill(1.0);
  BOOST_TEST((pnt3 == point_type{1.0}));
  auto pnt4 = pnt2;
  using std::swap;
  swap(pnt3, pnt4);
  BOOST_TEST(pnt3 == pnt2);

  for (auto& component: pnt3)
    component = 1.0;
  BOOST_TEST((pnt3 == point_type{1.0}));
  for (auto iter = pnt3.rbegin(), last = pnt3.rend(); iter != last; ++iter)
    *iter = 2.0;
  BOOST_TEST((pnt3 == point_type{2.0}));
  BOOST_TEST(pnt1.size() == pastel::geometry::meta::dimension_of<point_type>::value);
  BOOST_TEST(pnt1.max_size() == pastel::geometry::meta::dimension_of<point_type>::value);
  BOOST_TEST(!pnt1.empty());
  BOOST_TEST(pnt1.at(0u) == pnt1.front());
  BOOST_TEST(pnt1.at(pnt1.size()-1u) == pnt1.back());
}

BOOST_AUTO_TEST_CASE(geometry_2dim_test, * boost::unit_test::tolerance(0.000001))
{
  using vector_type = pastel::geometry::vector<2u, double>;
  using point_type = pastel::geometry::point<2u, double>;
  BOOST_TEST(pastel::geometry::meta::dimension_of<vector_type>::value == 2u);
  BOOST_TEST(pastel::geometry::meta::dimension_of<point_type>::value == 2u);

  auto const vec1 = vector_type{1.0, 2.0};
  auto const vec2 = vector_type{2.0, 4.0};
  BOOST_TEST(pastel::geometry::get<0>(vec1) == 1.0);
  BOOST_TEST(vec1 + vec1 == vec2);
  BOOST_TEST(vec1 - vec2 == -vec1);
  BOOST_TEST(2.0 * vec1 == vec2);
  BOOST_TEST(vec1 * 2.0 == vec2);
  BOOST_TEST(vec2 / 2.0 == vec1);
  BOOST_TEST(pastel::geometry::dot_product(vec1, vec2) == 10.0);
  BOOST_TEST(pastel::geometry::squared_norm(vec1) == 5.0);
  using std::sqrt;
  BOOST_TEST(pastel::geometry::norm(vec1) == sqrt(5.0));
  BOOST_TEST(pastel::geometry::cross_product(vec1, vec2) == 0.0);

  auto const pnt1 = point_type{1.0, 2.0};
  auto const pnt2 = point_type{3.0, 6.0};
  BOOST_TEST(pastel::geometry::get<0>(pnt1) == 1.0);
  BOOST_TEST(pnt1 + vec2 == pnt2);
  BOOST_TEST(vec2 + pnt1 == pnt2);
  BOOST_TEST(pnt2 - vec2 == pnt1);
  BOOST_TEST(pnt2 - pnt1 == vec2);

  auto vec3 = vector_type{0.0, 0.0};
  vec3.fill(1.0);
  BOOST_TEST((vec3 == vector_type{1.0, 1.0}));
  auto vec4 = vec2;
  using std::swap;
  swap(vec3, vec4);
  BOOST_TEST(vec3 == vec2);

  for (auto& component: vec3)
    component = 1.0;
  BOOST_TEST((vec3 == vector_type{1.0, 1.0}));
  for (auto iter = vec3.rbegin(), last = vec3.rend(); iter != last; ++iter)
    *iter = 2.0;
  BOOST_TEST((vec3 == vector_type{2.0, 2.0}));
  BOOST_TEST(vec1.size() == pastel::geometry::meta::dimension_of<vector_type>::value);
  BOOST_TEST(vec1.max_size() == pastel::geometry::meta::dimension_of<vector_type>::value);
  BOOST_TEST(!vec1.empty());
  BOOST_TEST(vec1.at(vec1.size()-1u) == vec1.back());

  auto pnt3 = point_type{0.0, 0.0};
  pnt3.fill(1.0);
  BOOST_TEST((pnt3 == point_type{1.0, 1.0}));
  auto pnt4 = pnt2;
  using std::swap;
  swap(pnt3, pnt4);
  BOOST_TEST(pnt3 == pnt2);

  for (auto& component: pnt3)
    component = 1.0;
  BOOST_TEST((pnt3 == point_type{1.0, 1.0}));
  for (auto iter = pnt3.rbegin(), last = pnt3.rend(); iter != last; ++iter)
    *iter = 2.0;
  BOOST_TEST((pnt3 == point_type{2.0, 2.0}));
  BOOST_TEST(pnt1.size() == pastel::geometry::meta::dimension_of<point_type>::value);
  BOOST_TEST(pnt1.max_size() == pastel::geometry::meta::dimension_of<point_type>::value);
  BOOST_TEST(!pnt1.empty());
  BOOST_TEST(pnt1.at(0u) == pnt1.front());
  BOOST_TEST(pnt1.at(pnt1.size()-1u) == pnt1.back());
}

BOOST_AUTO_TEST_CASE(geometry_3dim_test, * boost::unit_test::tolerance(0.000001))
{
  using vector_type = pastel::geometry::vector<3u, double>;
  using point_type = pastel::geometry::point<3u, double>;
  BOOST_TEST(pastel::geometry::meta::dimension_of<vector_type>::value == 3u);
  BOOST_TEST(pastel::geometry::meta::dimension_of<point_type>::value == 3u);

  auto const vec1 = vector_type{0.0, 1.0, 2.0};
  auto const vec2 = vector_type{0.0, 2.0, 4.0};
  BOOST_TEST(pastel::geometry::get<1>(vec1) == 1.0);
  BOOST_TEST(vec1 + vec1 == vec2);
  BOOST_TEST(vec1 - vec2 == -vec1);
  BOOST_TEST(2.0 * vec1 == vec2);
  BOOST_TEST(vec1 * 2.0 == vec2);
  BOOST_TEST(vec2 / 2.0 == vec1);
  BOOST_TEST(pastel::geometry::dot_product(vec1, vec2) == 10.0);
  BOOST_TEST(pastel::geometry::squared_norm(vec1) == 5.0);
  using std::sqrt;
  BOOST_TEST(pastel::geometry::norm(vec1) == sqrt(5.0));
  auto const vec0 = vector_type{0.0, 0.0, 0.0};
  BOOST_TEST(pastel::geometry::cross_product(vec1, vec2) == vec0);

  auto const pnt1 = point_type{0.0, 1.0, 2.0};
  auto const pnt2 = point_type{0.0, 3.0, 6.0};
  BOOST_TEST(pastel::geometry::get<1>(pnt1) == 1.0);
  BOOST_TEST(pnt1 + vec2 == pnt2);
  BOOST_TEST(vec2 + pnt1 == pnt2);
  BOOST_TEST(pnt2 - vec2 == pnt1);
  BOOST_TEST(pnt2 - pnt1 == vec2);

  auto vec3 = vector_type{0.0, 0.0, 0.0};
  vec3.fill(1.0);
  BOOST_TEST((vec3 == vector_type{1.0, 1.0, 1.0}));
  auto vec4 = vec2;
  using std::swap;
  swap(vec3, vec4);
  BOOST_TEST(vec3 == vec2);

  for (auto& component: vec3)
    component = 1.0;
  BOOST_TEST((vec3 == vector_type{1.0, 1.0, 1.0}));
  for (auto iter = vec3.rbegin(), last = vec3.rend(); iter != last; ++iter)
    *iter = 2.0;
  BOOST_TEST((vec3 == vector_type{2.0, 2.0, 2.0}));
  BOOST_TEST(vec1.size() == pastel::geometry::meta::dimension_of<vector_type>::value);
  BOOST_TEST(vec1.max_size() == pastel::geometry::meta::dimension_of<vector_type>::value);
  BOOST_TEST(!vec1.empty());
  BOOST_TEST(vec1.at(vec1.size()-1u) == vec1.back());

  auto pnt3 = point_type{0.0, 0.0, 0.0};
  pnt3.fill(1.0);
  BOOST_TEST((pnt3 == point_type{1.0, 1.0, 1.0}));
  auto pnt4 = pnt2;
  using std::swap;
  swap(pnt3, pnt4);
  BOOST_TEST(pnt3 == pnt2);

  for (auto& component: pnt3)
    component = 1.0;
  BOOST_TEST((pnt3 == point_type{1.0, 1.0, 1.0}));
  for (auto iter = pnt3.rbegin(), last = pnt3.rend(); iter != last; ++iter)
    *iter = 2.0;
  BOOST_TEST((pnt3 == point_type{2.0, 2.0, 2.0}));
  BOOST_TEST(pnt1.size() == pastel::geometry::meta::dimension_of<point_type>::value);
  BOOST_TEST(pnt1.max_size() == pastel::geometry::meta::dimension_of<point_type>::value);
  BOOST_TEST(!pnt1.empty());
  BOOST_TEST(pnt1.at(0u) == pnt1.front());
  BOOST_TEST(pnt1.at(pnt1.size()-1u) == pnt1.back());
}

