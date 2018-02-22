#include <iostream>
#include <random>
#include <tuple>
#include <utility>

#include <pastel/geometry/get.hpp>
#include <pastel/particle/tags.hpp>
#include <pastel/container/aligned_simple_particles.hpp>
#include <pastel/container/get.hpp>
#include <pastel/container/maximal_speed.hpp>
#include <pastel/container/meta/point_of.hpp>
#include <pastel/container/meta/vector_of.hpp>
#include <pastel/force/lennard_jones.hpp>
#include <pastel/neighbor/brute_force_neighbor_list.hpp>
#include <pastel/neighbor/brute_force_updater.hpp>
#include <pastel/system/simple_system.hpp>
#include <pastel/system/periodic_boundary.hpp>
#include <pastel/system/particles.hpp>
#include <pastel/integrate/gear/update.hpp>


template <typename System>
void output(System const& system)
{
  auto const& particles
    = pastel::system::particles<0u>(system);
  auto const num_particles = pastel::container::num_particles(particles);
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
      << pastel::geometry::get<1u>(force) << '\n';
  }
  std::cout << std::flush;

  auto const& boundary_particles
    = pastel::system::boundary_particles<0u>(system);
  auto const num_boundary_particles = pastel::container::num_particles(boundary_particles);
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
      << pastel::geometry::get<1u>(force) << '\n';
  }
  std::cout << std::flush;
}

#include <pastel/geometry/point_io.hpp>
int main(int argc, char* argv[])
{
  using rng_type = std::mt19937_64;
  constexpr auto seed = rng_type::result_type{0};

  constexpr auto num_particles = 100;
  constexpr auto num_particles_x = 10;
  constexpr auto num_particles_y = 10;
  static_assert(num_particles <= num_particles_x * num_particles_y);
  constexpr auto initial_typical_speed = 1.0;
  constexpr auto energy_depth = 1.0;
  constexpr auto diameter = 1.0;
  constexpr auto cutoff_length = 3.0 * diameter;
  constexpr auto search_length = 4.0 * diameter;
  constexpr auto lower_bound = -10.0 * diameter;
  constexpr auto upper_bound = +10.0 * diameter;
  constexpr auto boundary_width = search_length;
  constexpr auto num_steps = 10000;
  //constexpr auto num_steps = 1000000;
  //constexpr auto num_steps_to_output = 1000;
  constexpr auto time_step = 0.001;
  using force_type = pastel::force::lennard_jones<true>;
  auto force = force_type{energy_depth, diameter, cutoff_length};

  using x_boundary_type = pastel::system::periodic_boundary<>;
  using y_boundary_type = pastel::system::periodic_boundary<>;
  auto const x_boundary = x_boundary_type{lower_bound, upper_bound, boundary_width};
  auto const y_boundary = y_boundary_type{lower_bound, upper_bound, boundary_width};
  using boundary_type = std::tuple<x_boundary_type, y_boundary_type>;
  auto const boundary = boundary_type{x_boundary, y_boundary};

  using particles_type
    = pastel::container::aligned_simple_particles<2u, pastel::container::mobility_tags::mobile, 4u>;
  auto particles = particles_type{};
  particles.reserve(num_particles);

  using point_type = typename pastel::container::meta::point_of<particles_type>::type;
  using vector_type = typename pastel::container::meta::vector_of<particles_type>::type;
  auto rng = rng_type{seed};
  constexpr auto cell_length_x = (upper_bound - lower_bound) / num_particles_x;
  constexpr auto cell_length_y = (upper_bound - lower_bound) / num_particles_y;
  auto x_dist = std::uniform_real_distribution<>{-cell_length_x*0.1, cell_length_x*0.1};
  auto y_dist = std::uniform_real_distribution<>{-cell_length_y*0.1, cell_length_y*0.1};
  auto speed_dist = std::normal_distribution<>{0.0, initial_typical_speed};
  for (auto index_y = 0; index_y < num_particles_y; ++index_y)
    for (auto index_x = 0; index_x < num_particles_x; ++index_x)
    {
      if (index_y * num_particles_y + index_x >= num_particles)
        break;
      particles.emplace_back(
        point_type{(index_x + 0.5) * cell_length_x + x_dist(rng), (index_y + 0.5) * cell_length_y + y_dist(rng)},
        vector_type{speed_dist(rng), speed_dist(rng)},
        vector_type{});
    }

  using neighbor_list_type = pastel::neighbor::brute_force_neighbor_list<force_type>;
  auto neighbor_list
    = neighbor_list_type{
        particles, std::move(force),
        pastel::neighbor::brute_force_updater<double>{
          search_length, cutoff_length, time_step, pastel::container::maximal_speed(particles)}};

  using system_type = pastel::system::simple_system<particles_type, neighbor_list_type, boundary_type>;
  auto system = system_type{std::move(particles), std::move(neighbor_list), std::move(boundary), time_step};

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

