#ifndef PASTEL_SYSTEM_UPDATE_FORCES_HPP
# define PASTEL_SYSTEM_UPDATE_FORCES_HPP

# include <type_traits>
# include <utility>

# include <pastel/system/for_each_container.hpp>
# include <pastel/system/for_each_neighbor_list.hpp>
# include <pastel/system/for_each_boundary_neighbor_list.hpp>
# include <pastel/force/update_forces.hpp>
# include <pastel/force/update_boundary_forces.hpp>
# include <pastel/container/clear_forces.hpp>
# include <pastel/container/clear_densities_pressures.hpp>
# include <pastel/container/apply_external_forces.hpp>
# include <pastel/neighbor/meta/required_arguments_category_of.hpp>
# include <pastel/sph/update_densities.hpp>
# include <pastel/sph/update_boundary_densities.hpp>

# include <pastel/particle/tags.hpp>
# include <pastel/extforce/pressure.hpp>
# include <pastel/extforce/meta/is_sph_external_force.hpp>
# include <pastel/container/num_particles.hpp>
# include <pastel/container/get.hpp>
# include <pastel/container/meta/is_data_accessible.hpp>


namespace pastel
{
  namespace system
  {
    namespace update_forces_detail
    {
      struct clear_densities_pressures
      {
        template <typename Particles, typename ExternalForce>
        void operator()(Particles& particles, ExternalForce&&) const
        { ::pastel::container::clear_densities_pressures(particles); }
      }; // struct clear_densities_pressures

      struct update_densities
      {
        template <typename NeighborList, typename System>
        void operator()(NeighborList const& neighbor_list, System& system) const
        { ::pastel::sph::update_densities(neighbor_list.force(), neighbor_list, system); }
      }; // struct update_densities

      struct update_boundary_densities
      {
        template <typename BoundaryNeighborList, typename System>
        void operator()(BoundaryNeighborList const& boundary_neighbor_list, System& system) const
        { ::pastel::sph::update_boundary_densities(boundary_neighbor_list.force(), boundary_neighbor_list, system); }
      }; // struct update_boundary_densities_pressures


      template <bool is_data_accessible>
      struct update_pressures_impl2;

      template <>
      struct update_pressures_impl2<true>
      {
        template <typename Particles, typename ExternalForce>
        static void call(Particles& particles, ExternalForce&& external_force)
        {
          auto const pressures_data = particles.template data< ::pastel::particle::tags::pressure >();
          auto const densities_data = particles.template data< ::pastel::particle::tags::density >();

          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            pressures_data[index] = ::pastel::extforce::pressure(external_force, densities_data[index]);
        }
      }; // struct update_pressures_impl2<true>

      template <>
      struct update_pressures_impl2<false>
      {
        template <typename Particles, typename ExternalForce>
        static void call(Particles& particles, ExternalForce&& external_force)
        {
          auto const num_particles = ::pastel::container::num_particles(particles);
          for (auto index = static_cast<decltype(num_particles)>(0); index < num_particles; ++index)
            ::pastel::container::get< ::pastel::particle::tags::pressure >(particles, index)
              = ::pastel::extforce::pressure(
                  external_force,
                  ::pastel::container::get< ::pastel::particle::tags::density >(particles, index));
        }
      }; // struct update_pressures_impl2<false>

      template <bool is_sph_external_force>
      struct update_pressures_impl;

      template <>
      struct update_pressures_impl<true>
      {
        template <typename Particles, typename ExternalForce>
        static void call(Particles& particles, ExternalForce&& external_force)
        {
          static constexpr bool is_data_accessible
            = ::pastel::container::meta::is_data_accessible<Particles>::value;
          ::pastel::system::update_forces_detail::update_pressures_impl2<is_data_accessible>::call(
            particles, std::forward<ExternalForce>(external_force));
        }
      }; // struct update_pressures_impl<true>

      template <>
      struct update_pressures_impl<false>
      {
        template <typename Particles, typename ExternalForce>
        static void call(Particles&, ExternalForce&&)
        { }
      }; // struct update_pressures_impl<false>

      struct update_pressures
      {
        template <typename Particles, typename ExternalForce>
        void operator()(Particles& particles, ExternalForce&& external_force) const
        {
          static constexpr bool is_sph_external_force
            = ::pastel::extforce::meta::is_sph_external_force<typename std::remove_reference<ExternalForce>::type>::value;
          ::pastel::system::update_forces_detail::update_pressures_impl<is_sph_external_force>::call(
            particles, std::forward<ExternalForce>(external_force));
        }
      }; // struct update_pressures



      struct clear_forces
      {
        template <typename Particles, typename ExternalForce>
        void operator()(Particles& particles, ExternalForce&&) const
        { ::pastel::container::clear_forces(particles); }
      }; // struct clear_forces


      struct update_forces
      {
        template <typename NeighborList, typename System>
        void operator()(NeighborList const& neighbor_list, System& system) const
        { ::pastel::force::update_forces(neighbor_list.force(), neighbor_list, system); }
      }; // struct update_force

      struct update_boundary_forces
      {
        template <typename BoundaryNeighborList, typename System>
        void operator()(BoundaryNeighborList const& boundary_neighbor_list, System& system) const
        { ::pastel::force::update_boundary_forces(boundary_neighbor_list.force(), boundary_neighbor_list, system); }
      }; // struct update_boundary_force


      struct apply_external_forces
      {
        template <typename Particles, typename ExternalForce>
        void operator()(Particles& particles, ExternalForce&& external_force) const
        { ::pastel::container::apply_external_forces(particles, std::forward<ExternalForce>(external_force)); }
      }; // struct apply_external_forces
    } // namespace update_forces_detail

    template <typename System>
    inline void update_forces(System& system)
    {
      // for SPH
      ::pastel::system::for_each_container(
        system,
        ::pastel::system::update_forces_detail::clear_densities_pressures{});
      ::pastel::system::for_each_neighbor_list(
        system,
        ::pastel::system::update_forces_detail::update_densities{});
      ::pastel::system::for_each_boundary_neighbor_list(
        system,
        ::pastel::system::update_forces_detail::update_boundary_densities{});
      ::pastel::system::for_each_container(
        system,
        ::pastel::system::update_forces_detail::update_pressures{});

      ::pastel::system::for_each_container(
        system,
        ::pastel::system::update_forces_detail::clear_forces{});
      ::pastel::system::for_each_neighbor_list(
        system,
        ::pastel::system::update_forces_detail::update_forces{});
      ::pastel::system::for_each_boundary_neighbor_list(
        system,
        ::pastel::system::update_forces_detail::update_boundary_forces{});
      ::pastel::system::for_each_container(
        system,
        ::pastel::system::update_forces_detail::apply_external_forces{});
    }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_UPDATE_FORCES_HPP

