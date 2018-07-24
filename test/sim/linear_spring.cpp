#include <iostream>
#include <utility>

#include <pastel/geometry/get.hpp>
#include <pastel/particle/tags.hpp>
#include <pastel/container/aligned_simple_particles.hpp>
#include <pastel/container/get.hpp>
#include <pastel/container/meta/point_of.hpp>
#include <pastel/container/meta/vector_of.hpp>
#include <pastel/force/linear_spring.hpp>
#include <pastel/neighbor/fixed_neighbor_list.hpp>
#include <pastel/system/simple_system.hpp>
#include <pastel/system/particles.hpp>
#include <pastel/system/total_kinetic_energy.hpp>
#include <pastel/system/total_potential_energy.hpp>
//#include <pastel/integrate/euler/update.hpp>
#include <pastel/integrate/vverlet/update.hpp>
//#include <pastel/integrate/gear/update.hpp>


template <typename System, typename Time>
void output(System const& system, Time present_time)
{
  std::cout
    << present_time
    << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::position>(pastel::system::particles<0u>(system), 0u))
    << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::velocity>(pastel::system::particles<0u>(system), 0u))
    << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::force>(pastel::system::particles<0u>(system), 0u))
    << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::position>(pastel::system::particles<0u>(system), 1u))
    << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::velocity>(pastel::system::particles<0u>(system), 1u))
    << ' ' << pastel::geometry::get<0u>(pastel::container::get<pastel::particle::tags::force>(pastel::system::particles<0u>(system), 1u))
    << ' ' << pastel::system::total_kinetic_energy<0u>(system)
    << ' ' << pastel::system::total_potential_energy<0u>(system)
    << std::endl;
}

int main(int argc, char* argv[])
{
  constexpr auto spring_coefficient = 1.0;
  constexpr auto initial_speed = 0.0;
  constexpr auto num_steps = 100000000;
  constexpr auto num_steps_to_output = 10000;
  constexpr auto time_step = 0.001;
  using force_type = pastel::force::linear_spring<false>;
  auto force = force_type{spring_coefficient};

  using particles_type
    = pastel::container::aligned_simple_particles<1u, pastel::container::mobility_tags::mobile, 4u>;
  using point_type = typename pastel::container::meta::point_of<particles_type>::type;
  using vector_type = typename pastel::container::meta::vector_of<particles_type>::type;
  constexpr auto initial_velocity = vector_type{initial_speed};

  auto particles = particles_type{};
  particles.reserve(2u);
  particles.emplace_back(point_type{+0.5}, initial_velocity, vector_type{});
  particles.emplace_back(point_type{-0.5}, initial_velocity, vector_type{});

  using neighbor_list_type = pastel::neighbor::fixed_neighbor_list<force_type>;
  auto neighbor_list = neighbor_list_type{{0u, 1u}};

  using system_type = pastel::system::simple_system<particles_type, neighbor_list_type>;
  auto system = system_type{std::move(particles), std::move(neighbor_list), time_step};

  output(system, 0.0);
  for (auto step = 1; step <= num_steps; ++step)
  {
    //pastel::integrate::euler::update(system, time_step);
    pastel::integrate::vverlet::update(system, time_step);
    //pastel::integrate::gear::update<6u>(system, time_step);
    if (step % num_steps_to_output == 0)
      output(system, time_step * step);
  }
}

