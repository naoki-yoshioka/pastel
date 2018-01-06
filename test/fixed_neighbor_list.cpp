#define BOOST_TEST_MODULE fixed neighbor list test
#include <boost/test/included/unit_test.hpp>

#include <cstddef>
#include <utility>
#include <vector>
#include <iterator>
#include <algorithm>

#include <pastel/neighbor/fixed_neighbor_list.hpp>
#include <pastel/neighbor/aligned_fixed_neighbor_list.hpp>
#include <pastel/neighbor/neighbor.hpp>
#include <pastel/neighbor/neighbor_io.hpp>
#include <pastel/neighbor/force.hpp>
#include <pastel/neighbor/num_keys.hpp>
#include <pastel/neighbor/num_partners.hpp>
#include <pastel/neighbor/partner_begin.hpp>
#include <pastel/neighbor/partner_end.hpp>
#include <pastel/neighbor/meta/value_of.hpp>
#include <pastel/neighbor/meta/force_of.hpp>
#include <pastel/neighbor/meta/is_partner_data_accessible.hpp>
#include <pastel/geometry/vector.hpp>
#include <pastel/force/linear_spring.hpp>


template <typename NeighborList>
void test(typename pastel::neighbor::meta::force_of<NeighborList>::type&& force)
{
  BOOST_TEST(pastel::neighbor::meta::is_partner_data_accessible<NeighborList>::value);

  auto const tmp_force = force;
  constexpr auto num_neighbors1 = 5u;
  using value_type = typename pastel::neighbor::meta::value_of<NeighborList>::type;
  value_type neighbors1[num_neighbors1] = {{0u, 1u}, {0u, 3u}, {3u, 2u}, {4u, 0u}, {2u, 4u}};
  constexpr std::size_t possible_num_keys1 = 3u;
  constexpr std::size_t num_partners1[possible_num_keys1] = {3u, 0u, 2u};
  constexpr std::size_t possible_partners1[num_neighbors1] = {1u, 3u, 4u, 3u, 4u};

  auto const neighbor_list1 = NeighborList{neighbors1, neighbors1+num_neighbors1, std::move(force)};
  std::sort(neighbors1, neighbors1+num_neighbors1);
  BOOST_TEST(std::equal(std::begin(neighbor_list1), std::end(neighbor_list1), neighbors1));
  BOOST_TEST(pastel::neighbor::force(neighbor_list1).spring_coefficient() == tmp_force.spring_coefficient());

  auto neighbor_list2 = NeighborList{neighbors1, neighbors1+num_neighbors1};
  BOOST_TEST(std::equal(neighbor_list1.rbegin(), neighbor_list1.rend(), neighbor_list2.rbegin()));
  pastel::neighbor::force(neighbor_list2, tmp_force);
  BOOST_TEST(pastel::neighbor::force(neighbor_list2).spring_coefficient() == tmp_force.spring_coefficient());
  auto const are_both_equal = neighbor_list1 == neighbor_list2;
  BOOST_TEST(are_both_equal);

  auto const num_keys1 = pastel::neighbor::num_keys(neighbor_list1);
  BOOST_TEST(num_keys1 == possible_num_keys1);
  auto partners = std::vector<std::size_t>{};
  partners.reserve(num_neighbors1);
  for (auto index = static_cast<decltype(num_keys1)>(0); index < num_keys1; ++index)
  {
    BOOST_TEST(pastel::neighbor::num_partners(neighbor_list1, index) == num_partners1[index]);
    auto iter = pastel::neighbor::partner_begin(neighbor_list1, index);
    auto const last = pastel::neighbor::partner_end(neighbor_list1, index);
    for (; iter != last; ++iter)
      partners.push_back(*iter);
  }
  BOOST_TEST(std::equal(std::begin(partners), std::end(partners), possible_partners1));

  constexpr auto num_neighbors2 = 3u;
  value_type neighbors2[num_neighbors2] = {{0u, 2u}, {2u, 1u}, {4u, 5u}};
  constexpr auto possible_num_keys2 = 5u;
  constexpr std::size_t num_partners2[possible_num_keys2] = {1u, 1u, 0u, 0u, 1u};
  constexpr std::size_t possible_partners2[num_neighbors2] = {2u, 2u, 5u};

  neighbor_list2.assign(neighbors2, neighbors2+num_neighbors2);
  auto const num_keys2 = pastel::neighbor::num_keys(neighbor_list2);
  BOOST_TEST(num_keys2 == possible_num_keys2);
  partners.clear();
  partners.reserve(num_neighbors2);
  for (auto index = static_cast<decltype(num_keys2)>(0); index < num_keys2; ++index)
  {
    BOOST_TEST(pastel::neighbor::num_partners(neighbor_list2, index) == num_partners2[index]);
    auto iter = pastel::neighbor::partner_begin(neighbor_list2, index);
    auto const last = pastel::neighbor::partner_end(neighbor_list2, index);
    for (; iter != last; ++iter)
      partners.push_back(*iter);
  }
  BOOST_TEST(std::equal(std::begin(partners), std::end(partners), possible_partners2));

  BOOST_TEST(!neighbor_list2.empty());
  BOOST_TEST(neighbor_list2.size() == num_neighbors2);
  neighbor_list2.reserve(100u);
  BOOST_TEST(neighbor_list2.capacity() == 100u);
  neighbor_list2.shrink_to_fit();
  BOOST_TEST(neighbor_list2.capacity() == num_neighbors2);
  auto is_insertion_failed = !neighbor_list2.insert({0u, 2u}).second;
  BOOST_TEST(is_insertion_failed);
  neighbor_list2.insert(neighbor_list2.end(), {6u, 8u});
  BOOST_TEST((*neighbor_list2.rbegin() == value_type{6u, 8u}));

  neighbor_list2.insert({value_type{0u, 3u}, value_type{5u, 4u}, value_type{3u, 5u}});
  neighbor_list2.emplace_hint(neighbor_list2.begin(), 0u, 1u);
  constexpr auto num_neighbors2_2 = 7u;
  constexpr auto possible_num_keys2_2 = 7u;
  constexpr std::size_t num_partners2_2[possible_num_keys2_2] = {3u, 1u, 0u, 1u, 1u, 0u, 1u};
  constexpr std::size_t possible_partners2_2[num_neighbors2_2] = {1u, 2u, 3u, 2u, 5u, 5u, 8u};
  auto const num_keys2_2 = pastel::neighbor::num_keys(neighbor_list2);
  BOOST_TEST(num_keys2_2 == possible_num_keys2_2);
  partners.clear();
  partners.reserve(num_neighbors2_2);
  for (auto index = static_cast<decltype(num_keys2_2)>(0); index < num_keys2_2; ++index)
  {
    BOOST_TEST(pastel::neighbor::num_partners(neighbor_list2, index) == num_partners2_2[index]);
    auto iter = pastel::neighbor::partner_begin(neighbor_list2, index);
    auto const last = pastel::neighbor::partner_end(neighbor_list2, index);
    for (; iter != last; ++iter)
      partners.push_back(*iter);
  }
  BOOST_TEST(std::equal(std::begin(partners), std::end(partners), possible_partners2_2));

  BOOST_TEST((neighbor_list2.erase({7u, 1u}) == 0u));
  BOOST_TEST((neighbor_list2.erase({8u, 6u}) == 1u));
  BOOST_TEST((neighbor_list2.count({0u, 2u}) == 1u));
  BOOST_TEST((neighbor_list2.count({6u, 8u}) == 0u));
  auto const is_6_8_pair_erased = neighbor_list2.find({8u, 6u}) == neighbor_list2.end();
  BOOST_TEST(is_6_8_pair_erased);
  auto const iter = neighbor_list2.find({0u, 2u});
  auto const exists_0_2_pair = iter != neighbor_list2.end();
  BOOST_TEST(exists_0_2_pair);
  neighbor_list2.erase(iter);
  BOOST_TEST((neighbor_list2.count({0u, 2u}) == 0u));
  auto const lower_upper_0_2 = neighbor_list2.equal_range({0u, 2u});
  auto const is_0_2_pair_erased_again = lower_upper_0_2.first == lower_upper_0_2.second;
  BOOST_TEST(is_0_2_pair_erased_again);
  auto const lower_upper_0_3 = neighbor_list2.equal_range({0u, 3u});
  auto const exists_0_3_pair = lower_upper_0_3.first != lower_upper_0_3.second;
  BOOST_TEST(exists_0_3_pair);
  BOOST_TEST((*lower_upper_0_3.first == value_type{0u, 3u}));
}

BOOST_AUTO_TEST_CASE(fixed_neighbor_list_test, * boost::unit_test::tolerance(0.000001))
{
  using force_type = pastel::force::linear_spring<>;
  auto force = force_type{1.0};
  using neighbor_list_type = pastel::neighbor::fixed_neighbor_list<force_type>;
  test<neighbor_list_type>(std::move(force));
}

BOOST_AUTO_TEST_CASE(aligned_fixed_neighbor_list_test, * boost::unit_test::tolerance(0.000001))
{
  using force_type = pastel::force::linear_spring<>;
  auto force = force_type{1.0};
  using neighbor_list_type = pastel::neighbor::aligned_fixed_neighbor_list<force_type>;
  test<neighbor_list_type>(std::move(force));
}

/*
BOOST_AUTO_TEST_CASE(immobile_particles_test, * boost::unit_test::tolerance(0.000001))
{ immobile_test<pastel::container::particles<3u, pastel::container::mobility_tags::immobile, 1u, 0u>, 1u, 0u>(); }

BOOST_AUTO_TEST_CASE(immobile_aligned_particles_test, * boost::unit_test::tolerance(0.000001))
{ immobile_test<pastel::container::aligned_particles<3u, pastel::container::mobility_tags::immobile, 1u, 0u>, 1u, 0u>(); }
*/

