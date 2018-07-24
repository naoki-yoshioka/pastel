#include <iostream>
#include <tuple>
#include <utility>

#include <pastel/geometry/get.hpp>
#include <pastel/particle/tags.hpp>
#include <pastel/container/aligned_simple_newtonian_sph_particles.hpp>
#include <pastel/container/get.hpp>
#include <pastel/container/maximal_speed.hpp>
#include <pastel/container/meta/point_of.hpp>
#include <pastel/container/meta/vector_of.hpp>
#include <pastel/force/newtonian_sph_force.hpp>
#include <pastel/kernel/wendland.hpp>
#include <pastel/neighbor/brute_force_neighbor_list.hpp>
#include <pastel/neighbor/brute_force_updater.hpp>
#include <pastel/system/simple_system.hpp>
#include <pastel/system/periodic_boundary.hpp>
#include <pastel/system/no_boundary.hpp>
#include <pastel/system/particles.hpp>
#include <pastel/integrate/gear/update.hpp>
#include <pastel/sph/tait_pressure.hpp>

#include "sph_extforce.hpp"


template <typename System>
void output(System const& system)
{
  auto const& particles
    = pastel::system::particles<0u>(system);
  auto const num_particles = static_cast<int>(pastel::container::num_particles(particles));
  for (auto index = 0; index < num_particles; ++index)
  {
    auto const& position
      = pastel::container::get<pastel::particle::tags::position>(particles, index);
    auto const& velocity
      = pastel::container::get<pastel::particle::tags::velocity>(particles, index);
    auto const& force
      = pastel::container::get<pastel::particle::tags::force>(particles, index);
    std::cout
      << pastel::geometry::get<0u>(position) << ' '
      << pastel::geometry::get<1u>(position) << ' '
      << pastel::geometry::get<0u>(velocity) << ' '
      << pastel::geometry::get<1u>(velocity) << ' '
      << pastel::geometry::get<0u>(force) << ' '
      << pastel::geometry::get<1u>(force) << ' '
      << pastel::container::get<pastel::particle::tags::density>(particles, index) << ' '
      << pastel::container::get<pastel::particle::tags::pressure>(particles, index) << ' '
      << '\n';
  }
  std::cout << std::flush;

  auto const& boundary_particles
    = pastel::system::boundary_particles<0u>(system);
  auto const num_boundary_particles = static_cast<int>(pastel::container::num_particles(boundary_particles));
  for (auto index = 0; index < num_boundary_particles; ++index)
  {
    auto const& position
      = pastel::container::get<pastel::particle::tags::position>(boundary_particles, index);
    auto const& velocity
      = pastel::container::get<pastel::particle::tags::velocity>(boundary_particles, index);
    auto const& force
      = pastel::container::get<pastel::particle::tags::force>(boundary_particles, index);
    std::cout
      << pastel::geometry::get<0u>(position) << ' '
      << pastel::geometry::get<1u>(position) << ' '
      << pastel::geometry::get<0u>(velocity) << ' '
      << pastel::geometry::get<1u>(velocity) << ' '
      << pastel::geometry::get<0u>(force) << ' '
      << pastel::geometry::get<1u>(force) << ' '
      << pastel::container::get<pastel::particle::tags::density>(boundary_particles, index) << ' '
      << pastel::container::get<pastel::particle::tags::pressure>(boundary_particles, index) << ' '
      << '\n';
  }
  std::cout << std::flush;
}

int main(int argc, char* argv[])
{
  constexpr auto dimension = 2u;
  constexpr auto wall_dimension_index = 1u;

  constexpr auto initial_separation = 1.0;

  constexpr auto half_num_particles_x = 30;
  constexpr auto num_particles_x = 2 * half_num_particles_x;
  constexpr auto num_particles_y = num_particles_x;
  constexpr auto num_particles = num_particles_x * num_particles_y;

  constexpr auto shear_viscosity = 0.005;
  constexpr auto gravitational_acceleration_norm = 10.0;

  constexpr auto cutoff_length = 3.5 * initial_separation;
  constexpr auto search_length = 4.0 * initial_separation;
  static_assert(search_length > cutoff_length);
  constexpr auto boundary_width = search_length;

  constexpr auto initial_liquid_upper_bound = initial_separation * half_num_particles_x;
  constexpr auto initial_liquid_lower_bound = -1.0 * initial_liquid_upper_bound;

  constexpr auto upper_bound = 2.0 * initial_liquid_upper_bound;
  constexpr auto lower_bound = 2.0 * initial_liquid_lower_bound;

  constexpr auto reference_density = 1.0; // mass / initial_separation^dimension
  constexpr auto sound_speed = 1.0;
  constexpr auto exponent = 3.0;

  using pressure_type = pastel::sph::tait_pressure<>;
  auto pressure = pressure_type{reference_density, sound_speed, exponent};

  constexpr auto num_steps = 10000;
  //constexpr auto num_steps = 1000000;
  //constexpr auto num_steps_to_output = 1000;
  constexpr auto time_step = 0.001;

  using kernel_type = pastel::kernel::wendland<1u, dimension>;
  auto kernel = kernel_type{cutoff_length};
  using force_type = pastel::force::newtonian_sph_force<kernel_type>;
  auto force = force_type{std::move(kernel), shear_viscosity};

  using x_boundary_type = pastel::system::periodic_boundary<>;
  using y_boundary_type = pastel::system::no_boundary;
  auto const x_boundary = x_boundary_type{lower_bound, upper_bound, boundary_width};
  auto const y_boundary = y_boundary_type{};
  using boundary_type = std::tuple<x_boundary_type, y_boundary_type>;
  auto const boundary = boundary_type{x_boundary, y_boundary};

  using particles_type
    = pastel::container::aligned_simple_newtonian_sph_particles<dimension, pastel::container::mobility_tags::mobile, 4u>;
  auto particles = particles_type{};
  particles.reserve(num_particles);

  using point_type = typename pastel::container::meta::point_of<particles_type>::type;
  using vector_type = typename pastel::container::meta::vector_of<particles_type>::type;
  for (auto index_y = 0; index_y < num_particles_y; ++index_y)
    for (auto index_x = 0; index_x < num_particles_x; ++index_x)
      particles.emplace_back(
        point_type{(index_x + 0.5) * initial_separation + initial_liquid_lower_bound, (index_y + 0.5) * initial_separation + lower_bound},
        vector_type{},
        vector_type{},
        1.0, 1.0);

  using neighbor_list_type = pastel::neighbor::brute_force_neighbor_list<force_type>;
  auto neighbor_list
    = neighbor_list_type{
        particles, std::move(force),
        pastel::neighbor::brute_force_updater<double>{
          search_length, cutoff_length, time_step, pastel::container::maximal_speed(particles)}};

  auto const gravitational_acceleration = vector_type{0.0, -gravitational_acceleration_norm};
  using external_force_type = sph_extforce<pressure_type, vector_type, wall_dimension_index>;
  auto external_force = external_force_type{pressure, gravitational_acceleration, lower_bound};

  using system_type = pastel::system::simple_system<particles_type, neighbor_list_type, boundary_type, external_force_type>;
  auto system = system_type{std::move(particles), std::move(neighbor_list), std::move(boundary), std::move(external_force), time_step};


  /*
  output(system, 0.0);
  for (auto step = 1; step <= num_steps; ++step)
  {
    pastel::integrate::gear::update<6u>(system, time_step);
    if (step % num_steps_to_output == 0)
      output(system, time_step * step);
  }
  */
  for (auto step = 1; step <= num_steps; ++step)
    pastel::integrate::gear::update<6u>(system, time_step);
  output(system);
}

