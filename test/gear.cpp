#define BOOST_TEST_MODULE gear test
#include <boost/test/included/unit_test.hpp>

#include <iterator>
#include <utility>
#include <type_traits>

#include <boost/math/constants/constants.hpp>

#include <pastel/integrate/gear/update.hpp>
#include <pastel/system/simple_system.hpp>
#include <pastel/system/particles.hpp>
#include <pastel/system/neighbor_list.hpp>
#include <pastel/system/force.hpp>
#include <pastel/system/total_kinetic_energy.hpp>
#include <pastel/system/total_potential_energy.hpp>
#include <pastel/system/meta/point_of.hpp>
#include <pastel/system/meta/vector_of.hpp>
#include <pastel/system/meta/scalar_of.hpp>
//#include <pastel/container/aligned_particles.hpp>
#include <pastel/container/aligned_simple_particles.hpp>
#include <pastel/container/get.hpp>
#include <pastel/container/mobility_tags.hpp>
#include <pastel/geometry/get.hpp>
#include <pastel/geometry/vector_io.hpp>
#include <pastel/geometry/point_io.hpp>
#include <pastel/particle/tags.hpp>
#include <pastel/particle/particle_io.hpp>
#include <pastel/neighbor/neighbor_io.hpp>
#include <pastel/neighbor/fixed_neighbor_list.hpp>
#include <pastel/force/linear_spring.hpp>
#include <pastel/force/update_forces.hpp>


BOOST_AUTO_TEST_CASE(gear_test, * boost::unit_test::tolerance(0.001))
{
  //using particles_type = pastel::container::aligned_particles<3u, pastel::container::mobility_tags::mobile, 4u>;
  using particles_type = pastel::container::aligned_simple_particles<3u, pastel::container::mobility_tags::mobile, 4u>;
  using force_type = pastel::force::linear_spring<false>;
  using neighbor_list_type = pastel::neighbor::fixed_neighbor_list<force_type>;
  using system_type = pastel::system::simple_system<particles_type, neighbor_list_type>;
  auto system = system_type{};

  using point_type = typename pastel::system::meta::point_of<0u, system_type>::type;
  using vector_type = typename pastel::system::meta::vector_of<0u, system_type>::type;
  using scalar_type = typename pastel::system::meta::scalar_of<0u, system_type>::type;
  constexpr auto initial_velocity = vector_type{0.0, 0.0, 0.0};
  constexpr auto initial_force = vector_type{0.0, 0.0, 0.0};
  constexpr auto initial_mass = scalar_type{1.0};
  //constexpr auto initial_diameter = scalar_type{1.0};
  pastel::system::particles<0u>(system).reserve(2);
  pastel::system::particles<0u>(system).emplace_back(
    point_type{+0.5, 0.0, 0.0},
    initial_velocity, initial_force);
  //  initial_velocity, initial_force, initial_mass, initial_diameter);
  pastel::system::particles<0u>(system).emplace_back(
    point_type{-0.5, 0.0, 0.0},
    initial_velocity, initial_force);
  //  initial_velocity, initial_force, initial_mass, initial_diameter);

  auto const spring_coefficient = 1.0;
  pastel::system::neighbor_list<0u>(system).reserve(1u);
  pastel::system::neighbor_list<0u>(system).emplace(0u, 1u);
  pastel::system::force<0u>(system).spring_coefficient(spring_coefficient);

  pastel::force::update_forces(
    pastel::system::force<0u>(system), pastel::system::neighbor_list<0u>(system), system);
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(pastel::system::particles<0u>(system), 0u) == vector_type{-1.0, 0.0, 0.0}));
  BOOST_TEST((pastel::container::get<pastel::particle::tags::force>(pastel::system::particles<0u>(system), 1u) == vector_type{+1.0, 0.0, 0.0}));

  auto const initial_total_energy
    = ::pastel::system::total_kinetic_energy<0u>(system)
      + ::pastel::system::total_potential_energy<0u>(system);

  using std::sqrt;
  auto const period = 2.0 * boost::math::constants::pi<double>() / sqrt(initial_mass / (2.0 * spring_coefficient));
  auto const num_steps = 1000000;
  auto const time_step = period / num_steps;
  for (auto step = 0; step < num_steps; ++step)
    pastel::integrate::gear::update<6u>(system, time_step);
  auto const x0 = pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::position>(pastel::system::particles<0u>(system), 0u));
  auto const x1 = pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::position>(pastel::system::particles<0u>(system), 1u));
  BOOST_TEST(x0 == +0.5);
  BOOST_TEST(x1 == -0.5);

  auto const total_energy
    = ::pastel::system::total_kinetic_energy<0u>(system)
      + ::pastel::system::total_potential_energy<0u>(system);
  BOOST_TEST(total_energy == initial_total_energy);
}

