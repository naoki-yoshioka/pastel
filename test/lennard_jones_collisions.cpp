#define BOOST_TEST_MODULE Lennard-Jones collisions test
#include <boost/test/included/unit_test.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <utility>

#include <pastel/geometry/get.hpp>
#include <pastel/geometry/distance.hpp>
#include <pastel/particle/tags.hpp>
#include <pastel/container/aligned_simple_particles.hpp>
#include <pastel/container/get.hpp>
#include <pastel/container/maximal_speed.hpp>
#include <pastel/container/meta/point_of.hpp>
#include <pastel/container/meta/vector_of.hpp>
#include <pastel/neighbor/brute_force_neighbor_list.hpp>
#include <pastel/neighbor/brute_force_updater.hpp>
#include <pastel/neighbor/inexclusive_cells_neighbor_list.hpp>
#include <pastel/neighbor/inexclusive_cells_updater.hpp>
#include <pastel/force/lennard_jones.hpp>
#include <pastel/system/simple_system.hpp>
#include <pastel/system/particles.hpp>
#include <pastel/system/neighbor_list.hpp>
#include <pastel/system/boundary_neighbor_list.hpp>
#include <pastel/system/update_forces.hpp>
#include <pastel/system/periodic_boundary.hpp>
#include <pastel/system/initialize.hpp>
#include <pastel/system/total_kinetic_energy.hpp>
#include <pastel/system/total_potential_energy.hpp>
#include <pastel/system/total_boundary_potential_energy.hpp>
#include <pastel/integrate/vverlet/update.hpp>
#include <pastel/utility/pair.hpp>


BOOST_AUTO_TEST_CASE(lennard_jones_collisions_test, * boost::unit_test::tolerance(0.001))
{
  constexpr auto speed = 1.0;
  constexpr auto diameter = 1.0;
  constexpr auto cutoff_length = 3.0 * diameter;
  constexpr auto search_length = 4.0 * diameter;
  constexpr auto lower_bound_1d = -10.0 * diameter;
  constexpr auto upper_bound_1d = +10.0 * diameter;
  using force_type = pastel::force::lennard_jones<true>;
  auto force = force_type{1.0, diameter, cutoff_length};
  constexpr auto time_step = 0.001;
  constexpr auto num_steps = 40000u;

  using particles_type = pastel::container::aligned_simple_particles<2u>;
  using point_type = typename pastel::container::meta::point_of<particles_type>::type;
  using vector_type = typename pastel::container::meta::vector_of<particles_type>::type;
  {
  constexpr auto boundary_width = search_length;
  using x_boundary_type = pastel::system::periodic_boundary<>;
  using y_boundary_type = pastel::system::periodic_boundary<>;
  auto const x_boundary = x_boundary_type{lower_bound_1d, upper_bound_1d, boundary_width};
  auto const y_boundary = y_boundary_type{lower_bound_1d, upper_bound_1d, boundary_width};
  using boundary_type = std::tuple<x_boundary_type, y_boundary_type>;
  auto const boundary = boundary_type{x_boundary, y_boundary};

  auto particles = particles_type{};
  particles.reserve(2u);
  particles.emplace_back(point_type{lower_bound_1d/2.0, 0.0}, vector_type{+speed, 0.0}, vector_type{});
  particles.emplace_back(point_type{upper_bound_1d/2.0, 0.0}, vector_type{-speed, 0.0}, vector_type{});

  // Brute Force Neighbor List
  std::cout << "[Brute Force]\n" << std::flush;
  using neighbor_list_type = pastel::neighbor::brute_force_neighbor_list<force_type>;
  auto neighbor_list
    = neighbor_list_type{
        particles, std::move(force),
        pastel::neighbor::brute_force_updater<double>{
          search_length, cutoff_length, time_step, pastel::container::maximal_speed(particles)}};

  using system_type = pastel::system::simple_system<particles_type, neighbor_list_type, boundary_type>;
  auto system
    = system_type{
        std::move(particles), std::move(neighbor_list),
        std::move(boundary), time_step};

  auto const initial_total_energy
    = pastel::system::total_kinetic_energy<0u>(system)
      + pastel::system::total_potential_energy<0u>(system)
      + pastel::system::total_boundary_potential_energy<0u>(system);
  std::cout << "E_0 = " << initial_total_energy << std::endl;
  for (auto step = 1u; step <= num_steps; ++step)
    pastel::integrate::vverlet::update(system, time_step);
  /*
  for (auto step = 1u; step <= num_steps; ++step)
  {
    pastel::integrate::vverlet::update(system, time_step);
    std::cout
      << time_step*step
      << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::position>(pastel::system::particles<0u>(system), 0u))
      << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::position>(pastel::system::particles<0u>(system), 1u))
      << std::endl;
  }
  */

  auto const total_energy
    = pastel::system::total_kinetic_energy<0u>(system)
      + pastel::system::total_potential_energy<0u>(system)
      + pastel::system::total_boundary_potential_energy<0u>(system);
  std::cout << "E = " << total_energy << std::endl;
  BOOST_TEST(total_energy == initial_total_energy);
  }


  {
  constexpr auto num_cells_1d = 4u;
  constexpr auto boundary_width = (upper_bound_1d - lower_bound_1d) / static_cast<decltype(lower_bound_1d)>(num_cells_1d);
  using x_boundary_type = pastel::system::periodic_boundary<>;
  using y_boundary_type = pastel::system::periodic_boundary<>;
  auto const x_boundary = x_boundary_type{lower_bound_1d, upper_bound_1d, boundary_width};
  auto const y_boundary = y_boundary_type{lower_bound_1d, upper_bound_1d, boundary_width};
  using boundary_type = std::tuple<x_boundary_type, y_boundary_type>;
  auto const boundary = boundary_type{x_boundary, y_boundary};

  auto particles = particles_type{};
  particles.reserve(2u);
  particles.emplace_back(point_type{lower_bound_1d/2.0, 0.0}, vector_type{+speed, 0.0}, vector_type{});
  particles.emplace_back(point_type{upper_bound_1d/2.0, 0.0}, vector_type{-speed, 0.0}, vector_type{});

  // Inexclusive Cells Neighbor List
  std::cout << "[Inexclusive Cells]\n" << std::flush;
  auto const lower_bound = point_type{lower_bound_1d, lower_bound_1d};
  auto const upper_bound = point_type{upper_bound_1d, upper_bound_1d};
  using neighbor_list_type = pastel::neighbor::inexclusive_cells_neighbor_list<force_type, point_type>;
  auto neighbor_list
    = neighbor_list_type{
        particles, std::move(force),
        pastel::neighbor::inexclusive_cells_updater<point_type>{
          search_length, cutoff_length, time_step, pastel::container::maximal_speed(particles),
          lower_bound, upper_bound, num_cells_1d, num_cells_1d}};

  using system_type = pastel::system::simple_system<particles_type, neighbor_list_type, boundary_type>;
  auto system
    = system_type{
        std::move(particles), std::move(neighbor_list),
        std::move(boundary), time_step};

  auto const initial_total_energy
    = pastel::system::total_kinetic_energy<0u>(system)
      + pastel::system::total_potential_energy<0u>(system)
      + pastel::system::total_boundary_potential_energy<0u>(system);
  std::cout << "E_0 = " << initial_total_energy << std::endl;
  for (auto step = 1u; step <= num_steps; ++step)
    pastel::integrate::vverlet::update(system, time_step);
  /*
  for (auto step = 1u; step <= num_steps; ++step)
  {
    pastel::integrate::vverlet::update(system, time_step);
    std::cout
      << time_step*step
      << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::position>(pastel::system::particles<0u>(system), 0u))
      << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::position>(pastel::system::particles<0u>(system), 1u))
      << std::endl;
  }
  */

  auto const total_energy
    = pastel::system::total_kinetic_energy<0u>(system)
      + pastel::system::total_potential_energy<0u>(system)
      + pastel::system::total_boundary_potential_energy<0u>(system);
  std::cout << "E = " << total_energy << std::endl;
  BOOST_TEST(total_energy == initial_total_energy);
  }
}

