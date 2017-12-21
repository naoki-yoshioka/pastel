#define BOOST_TEST_MODULE system test
#include <boost/test/included/unit_test.hpp>

#include <tuple>
#include <utility>
#include <type_traits>

#include <pastel/system/system.hpp>
#include <pastel/system/particles.hpp>
#include <pastel/system/neighbor_list.hpp>
#include <pastel/system/meta/particles_tuple_size_of.hpp>
#include <pastel/system/meta/neighbor_list_tuple_size_of.hpp>
#include <pastel/system/meta/particles_of.hpp>
#include <pastel/system/meta/neighbor_list_of.hpp>
#include <pastel/system/meta/external_force_of.hpp>
#include <pastel/container/aligned_particles.hpp>
#include <pastel/container/meta/point_of.hpp>
#include <pastel/container/meta/vector_of.hpp>
#include <pastel/container/meta/scalar_of.hpp>
#include <pastel/geometry/vector_io.hpp>
#include <pastel/geometry/point_io.hpp>
#include <pastel/neighbor/aligned_fixed_neighbor_list.hpp>
#include <pastel/neighbor/neighbor_io.hpp>
#include <pastel/neighbor/meta/interaction_pair_of.hpp>
#include <pastel/neighbor/meta/is_intraparticle.hpp>
#include <pastel/force/linear_spring.hpp>
#include <pastel/extforce/no_force.hpp>
#include <pastel/utility/pair.hpp>


BOOST_AUTO_TEST_CASE(system_test, * boost::unit_test::tolerance(0.000001))
{
  using particles_type = pastel::container::aligned_particles<3u>;
  using force_type = pastel::force::linear_spring<>;
  using neighbor_list1_type = pastel::neighbor::aligned_fixed_neighbor_list<force_type, pastel::utility::size_pair<0u, 0u>>;
  using neighbor_list2_type = pastel::neighbor::aligned_fixed_neighbor_list<force_type, pastel::utility::size_pair<1u, 1u>>;
  using neighbor_list12_type = pastel::neighbor::aligned_fixed_neighbor_list<force_type, pastel::utility::size_pair<0u, 1u>, false>;

  using particles_tuple_type = std::tuple<particles_type, particles_type>;
  using neighbor_list_tuple_type = std::tuple<neighbor_list1_type, neighbor_list2_type, neighbor_list12_type>;
  using system_type = pastel::system::system<particles_tuple_type, neighbor_list_tuple_type>;
  BOOST_TEST(pastel::system::meta::particles_tuple_size_of<system_type>::value == 2u);
  BOOST_TEST(pastel::system::meta::neighbor_list_tuple_size_of<system_type>::value == 3u);
  BOOST_TEST((std::is_same<typename pastel::system::meta::particles_of<0u, system_type>::type, particles_type>::value));
  BOOST_TEST((std::is_same<typename pastel::system::meta::particles_of<1u, system_type>::type, particles_type>::value));
  BOOST_TEST((std::is_same<typename pastel::system::meta::neighbor_list_of<0u, system_type>::type, neighbor_list1_type>::value));
  BOOST_TEST((std::is_same<typename pastel::system::meta::neighbor_list_of<1u, system_type>::type, neighbor_list2_type>::value));
  BOOST_TEST((std::is_same<typename pastel::system::meta::neighbor_list_of<2u, system_type>::type, neighbor_list12_type>::value));
  BOOST_TEST((std::is_same<typename pastel::system::meta::external_force_of<0u, system_type>::type, pastel::extforce::no_force>::value));
  BOOST_TEST((std::is_same<typename pastel::system::meta::external_force_of<1u, system_type>::type, pastel::extforce::no_force>::value));
  BOOST_TEST((pastel::neighbor::meta::interaction_pair_of<typename pastel::system::meta::neighbor_list_of<0u, system_type>::type>::type::first == 0u));
  BOOST_TEST((pastel::neighbor::meta::interaction_pair_of<typename pastel::system::meta::neighbor_list_of<0u, system_type>::type>::type::second == 0u));
  BOOST_TEST((pastel::neighbor::meta::is_intraparticle<typename pastel::system::meta::neighbor_list_of<0u, system_type>::type>::value));
  BOOST_TEST((pastel::neighbor::meta::interaction_pair_of<typename pastel::system::meta::neighbor_list_of<1u, system_type>::type>::type::first == 1u));
  BOOST_TEST((pastel::neighbor::meta::interaction_pair_of<typename pastel::system::meta::neighbor_list_of<1u, system_type>::type>::type::second == 1u));
  BOOST_TEST((pastel::neighbor::meta::is_intraparticle<typename pastel::system::meta::neighbor_list_of<1u, system_type>::type>::value));
  BOOST_TEST((pastel::neighbor::meta::interaction_pair_of<typename pastel::system::meta::neighbor_list_of<2u, system_type>::type>::type::first == 0u));
  BOOST_TEST((pastel::neighbor::meta::interaction_pair_of<typename pastel::system::meta::neighbor_list_of<2u, system_type>::type>::type::second == 1u));
  BOOST_TEST((!pastel::neighbor::meta::is_intraparticle<typename pastel::system::meta::neighbor_list_of<2u, system_type>::type>::value));

  constexpr auto num_particles1 = 10u;
  constexpr auto num_neighbors1 = num_particles1-1u;
  constexpr auto num_particles2 = num_particles1-1u;
  constexpr auto num_neighbors2 = num_particles2-1u;
  constexpr auto num_neighbors12 = num_particles2;

  using point_type = typename pastel::container::meta::point_of<particles_type>::type;
  using vector_type = typename pastel::container::meta::vector_of<particles_type>::type;
  using scalar_type = typename pastel::container::meta::scalar_of<particles_type>::type;
  constexpr auto initial_velocity = vector_type{1.0, 0.0, 0.0};
  constexpr auto initial_force = vector_type{1.0, 0.0, 0.0};
  constexpr auto initial_mass = scalar_type{1.0};
  constexpr auto initial_diameter = scalar_type{1.0};
  auto particles1 = particles_type{};
  particles1.reserve(num_particles1);
  for (auto index = 0u; index < num_particles1; ++index)
    particles1.emplace_back(
      point_type{1.0 * index + 0.5, 1.0, 1.0},
      initial_velocity, initial_force, initial_mass, initial_diameter);
  auto particles2 = particles_type{};
  particles2.reserve(num_particles2);
  for (auto index = 0u; index < num_particles2; ++index)
    particles2.emplace_back(
      point_type{1.0 * index + 1.0, 1.0, 1.0},
      initial_velocity, initial_force, initial_mass, initial_diameter);

  auto neighbor_list1 = neighbor_list1_type{};
  neighbor_list1.reserve(num_neighbors1);
  for (auto index = 0u; index < num_neighbors1; ++index)
    neighbor_list1.emplace_hint(neighbor_list1.end(), index, index+1u);
  auto neighbor_list2 = neighbor_list2_type{};
  neighbor_list2.reserve(num_neighbors2);
  for (auto index = 0u; index < num_neighbors2; ++index)
    neighbor_list2.emplace_hint(neighbor_list2.end(), index, index+1u);
  auto neighbor_list12 = neighbor_list12_type{};
  neighbor_list12.reserve(num_neighbors12);
  for (auto index = 0u; index < num_neighbors12; ++index)
    neighbor_list12.emplace_hint(neighbor_list12.end(), index, index);

  auto system = system_type{std::make_tuple(particles1, particles2), std::make_tuple(neighbor_list1, neighbor_list2, neighbor_list12)};
  auto const is_same_particles1 = pastel::system::particles<0u>(system) == particles1;
  auto const is_same_particles2 = pastel::system::particles<1u>(system) == particles2;
  BOOST_TEST(is_same_particles1);
  BOOST_TEST(is_same_particles2);
  auto const is_same_neighbor_list1 = pastel::system::neighbor_list<0u>(system) == neighbor_list1;
  auto const is_same_neighbor_list2 = pastel::system::neighbor_list<1u>(system) == neighbor_list2;
  auto const is_same_neighbor_list12 = pastel::system::neighbor_list<2u>(system) == neighbor_list12;
  BOOST_TEST(is_same_neighbor_list1);
  BOOST_TEST(is_same_neighbor_list2);
  BOOST_TEST(is_same_neighbor_list12);
}

