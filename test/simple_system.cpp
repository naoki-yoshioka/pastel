#define BOOST_TEST_MODULE simple system test
#include <boost/test/included/unit_test.hpp>

#include <utility>
#include <type_traits>

#include <pastel/system/simple_system.hpp>
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
#include <pastel/neighbor/fixed_neighbor_list.hpp>
#include <pastel/neighbor/neighbor_io.hpp>
#include <pastel/neighbor/meta/interaction_pair_of.hpp>
#include <pastel/force/linear_spring.hpp>
#include <pastel/extforce/no_force.hpp>


BOOST_AUTO_TEST_CASE(simple_system_test, * boost::unit_test::tolerance(0.000001))
{
  using particles_type = pastel::container::aligned_particles<3u>;
  using force_type = pastel::force::linear_spring<>;
  using neighbor_list_type = pastel::neighbor::fixed_neighbor_list<force_type>;
  using system_type = pastel::system::simple_system<particles_type, neighbor_list_type>;
  BOOST_TEST(pastel::system::meta::particles_tuple_size_of<system_type>::value == 1u);
  BOOST_TEST(pastel::system::meta::neighbor_list_tuple_size_of<system_type>::value == 1u);
  BOOST_TEST((std::is_same<typename pastel::system::meta::particles_of<0u, system_type>::type, particles_type>::value));
  BOOST_TEST((std::is_same<typename pastel::system::meta::neighbor_list_of<0u, system_type>::type, neighbor_list_type>::value));
  BOOST_TEST((std::is_same<typename pastel::system::meta::external_force_of<0u, system_type>::type, pastel::extforce::no_force>::value));
  BOOST_TEST((pastel::neighbor::meta::interaction_pair_of<typename pastel::system::meta::neighbor_list_of<0u, system_type>::type>::type::first == 0u));
  BOOST_TEST((pastel::neighbor::meta::interaction_pair_of<typename pastel::system::meta::neighbor_list_of<0u, system_type>::type>::type::second == 0u));

  constexpr auto num_particles = 10u;
  constexpr auto num_neighbors = num_particles-1u;

  using point_type = typename pastel::container::meta::point_of<particles_type>::type;
  using vector_type = typename pastel::container::meta::vector_of<particles_type>::type;
  using scalar_type = typename pastel::container::meta::scalar_of<particles_type>::type;
  constexpr auto initial_velocity = vector_type{1.0, 0.0, 0.0};
  constexpr auto initial_force = vector_type{1.0, 0.0, 0.0};
  constexpr auto initial_mass = scalar_type{1.0};
  constexpr auto initial_diameter = scalar_type{1.0};
  auto particles = particles_type{};
  particles.reserve(num_particles);
  for (auto index = 0u; index < num_particles; ++index)
    particles.emplace_back(
      point_type{1.0 * index + 0.5, 1.0, 1.0},
      initial_velocity, initial_force, initial_mass, initial_diameter);

  auto neighbor_list = neighbor_list_type{};
  neighbor_list.reserve(num_neighbors);
  for (auto index = 0u; index < num_neighbors; ++index)
    neighbor_list.emplace_hint(neighbor_list.end(), index, index+1u);

  auto const time_step = 0.001;
  auto system = system_type{particles, neighbor_list, time_step};
  // These are different particles because particles in system now have correct forces
  //auto const is_same_particles = pastel::system::particles<0u>(system) == particles;
  //BOOST_TEST(is_same_particles);
  auto const is_same_neighbor_list = pastel::system::neighbor_list<0u>(system) == neighbor_list;
  BOOST_TEST(is_same_neighbor_list);

  pastel::system::particles<0u>(system, std::move(particles));
  pastel::system::neighbor_list<0u>(system, std::move(neighbor_list));
}

