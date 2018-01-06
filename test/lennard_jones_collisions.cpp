#define BOOST_TEST_MODULE Lennard-Jones collisions test
#include <boost/test/included/unit_test.hpp>

#include <cstddef>
#include <vector>
#include <iterator>
#include <utility>

#include <pastel/geometry/get.hpp>
#include <pastel/particle/tags.hpp>
#include <pastel/container/aligned_simple_particles.hpp>
#include <pastel/container/get.hpp>
#include <pastel/container/maximal_speed.hpp>
#include <pastel/container/clear_forces.hpp>
#include <pastel/container/meta/point_of.hpp>
#include <pastel/container/meta/vector_of.hpp>
#include <pastel/neighbor/aligned_fixed_neighbor_list.hpp>
#include <pastel/neighbor/aligned_brute_force_neighbor_list.hpp>
#include <pastel/neighbor/brute_force_updater.hpp>
#include <pastel/neighbor/update_forces.hpp>
#include <pastel/force/lennard_jones.hpp>
#include <pastel/system/simple_system.hpp>
#include <pastel/system/particles.hpp>
#include <pastel/system/neighbor_list.hpp>
#include <pastel/integrate/vverlet/update.hpp>
#include <pastel/utility/pair.hpp>

#include <pastel/neighbor/neighbor_io.hpp>


BOOST_AUTO_TEST_CASE(lennard_jones_collisions_test, * boost::unit_test::tolerance(0.001))
{
  constexpr auto speed = 1.0;
  constexpr auto diameter = 1.0;
  constexpr auto cutoff_length = 3.0 * diameter;
  using force_type = pastel::force::lennard_jones<true>;
  auto force = force_type{1.0, diameter, cutoff_length};
  constexpr auto time_step = 0.001;
  constexpr auto num_steps = 10000u;

  using particles_type = pastel::container::aligned_simple_particles<2u>;
  using point_type = typename pastel::container::meta::point_of<particles_type>::type;
  using vector_type = typename pastel::container::meta::vector_of<particles_type>::type;
  auto particles = particles_type{};
  particles.reserve(2u);
  particles.emplace_back(point_type{-5.0, 0.0}, vector_type{5.0, 0.0}, vector_type{});
  particles.emplace_back(point_type{speed, 0.0}, vector_type{-speed, 0.0}, vector_type{});

  // Fixed Neighbor List
  using fixed_neighbor_list_type = pastel::neighbor::aligned_fixed_neighbor_list<force_type>;
  auto fixed_neighbor_list = fixed_neighbor_list_type{{{0u, 1u}}, force};

  using fixed_system_type = pastel::system::simple_system<particles_type, fixed_neighbor_list_type>;
  auto fixed_system = fixed_system_type{particles, std::move(fixed_neighbor_list)};
  pastel::container::clear_forces(pastel::system::particles<0u>(fixed_system));
  pastel::neighbor::update_forces(pastel::system::neighbor_list<0u>(fixed_system), fixed_system);

  std::vector<double> fixed_result;
  fixed_result.reserve(num_steps+1u);
  fixed_result.push_back(
    pastel::geometry::get<0u>(
      pastel::container::get<pastel::particle::tags::position>(
        pastel::system::particles<0u>(fixed_system), 0u)));
  for (auto step = 1u; step <= num_steps; ++step)
  {
    pastel::integrate::vverlet::update(fixed_system, time_step);

    fixed_result.push_back(
      pastel::geometry::get<0u>(
        pastel::container::get<pastel::particle::tags::position>(
          pastel::system::particles<0u>(fixed_system), 0u)));
  }

  // Brute Force Neighbor List
  constexpr auto search_length = cutoff_length + diameter;
  using brute_force_neighbor_list_type = pastel::neighbor::aligned_brute_force_neighbor_list<force_type>;
  auto brute_force_neighbor_list
    = brute_force_neighbor_list_type{
        particles, std::move(force),
        pastel::neighbor::brute_force_updater<double>{
          search_length, cutoff_length, time_step, pastel::container::maximal_speed(particles)}};

  using brute_force_system_type = pastel::system::simple_system<particles_type, brute_force_neighbor_list_type>;
  auto brute_force_system = brute_force_system_type{std::move(particles), std::move(brute_force_neighbor_list)};
  pastel::container::clear_forces(pastel::system::particles<0u>(brute_force_system));
  pastel::neighbor::update_forces(pastel::system::neighbor_list<0u>(brute_force_system), brute_force_system);

  std::vector<double> brute_force_result;
  brute_force_result.reserve(num_steps+1u);
  brute_force_result.push_back(
    pastel::geometry::get<0u>(
      pastel::container::get<pastel::particle::tags::position>(
        pastel::system::particles<0u>(brute_force_system), 0u)));
  for (auto step = 1u; step <= num_steps; ++step)
  {
    pastel::integrate::vverlet::update(brute_force_system, time_step);

    brute_force_result.push_back(
      pastel::geometry::get<0u>(
        pastel::container::get<pastel::particle::tags::position>(
          pastel::system::particles<0u>(brute_force_system), 0u)));
  }


  BOOST_TEST(fixed_result[num_steps] == brute_force_result[num_steps]);
}

