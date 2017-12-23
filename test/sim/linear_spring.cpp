#include <iostream>
#include <iterator>
#include <utility>
#include <type_traits>

//#include <pastel/integrate/euler/update.hpp>
//#include <pastel/integrate/vverlet/update.hpp>
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
#include <pastel/container/aligned_simple_particles.hpp>
#include <pastel/container/get.hpp>
#include <pastel/geometry/get.hpp>
#include <pastel/geometry/vector_io.hpp>
#include <pastel/geometry/point_io.hpp>
#include <pastel/particle/tags.hpp>
#include <pastel/particle/particle_io.hpp>
#include <pastel/neighbor/neighbor_io.hpp>
#include <pastel/neighbor/aligned_fixed_neighbor_list.hpp>
#include <pastel/force/linear_spring.hpp>
#include <pastel/force/update_forces.hpp>


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
  //using particles_type = pastel::container::aligned_simple_particles<1u>;
  using particles_type = pastel::container::aligned_simple_particles<1u, pastel::container::mobility_tags::mobile, 4u>;
  using force_type = pastel::force::linear_spring<false>;
  using neighbor_list_type = pastel::neighbor::aligned_fixed_neighbor_list<force_type>;
  using system_type = pastel::system::simple_system<particles_type, neighbor_list_type>;
  auto system = system_type{};

  using point_type = typename pastel::system::meta::point_of<0u, system_type>::type;
  using vector_type = typename pastel::system::meta::vector_of<0u, system_type>::type;
  constexpr auto initial_velocity = vector_type{0.0};
  constexpr auto initial_force = vector_type{0.0};
  pastel::system::particles<0u>(system).reserve(2);
  pastel::system::particles<0u>(system).emplace_back(
    point_type{+0.5}, initial_velocity, initial_force);
  pastel::system::particles<0u>(system).emplace_back(
    point_type{-0.5}, initial_velocity, initial_force);

  constexpr auto spring_coefficient = 1.0;
  pastel::system::neighbor_list<0u>(system).reserve(1u);
  pastel::system::neighbor_list<0u>(system).emplace(0u, 1u);
  pastel::system::force<0u>(system).spring_coefficient(spring_coefficient);

  pastel::force::update_forces(
    pastel::system::force<0u>(system), pastel::system::neighbor_list<0u>(system), system);

  output(system, 0.0);
  constexpr auto num_steps = 100000000u;
  constexpr auto num_steps_to_output = 10000u;
  constexpr auto time_step = 0.001;
  for (auto step = 1u; step <= num_steps; ++step)
  {
    //pastel::integrate::euler::update(system, time_step);
    //pastel::integrate::vverlet::update(system, time_step);
    pastel::integrate::gear::update<6u>(system, time_step);
    if (step % num_steps_to_output == 0u)
      output(system, time_step * step);
  }
}

