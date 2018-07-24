#ifndef PASTEL_SYSTEM_UPDATE_SPH_DIVERGENCES_HPP
# define PASTEL_SYSTEM_UPDATE_SPH_DIVERGENCES_HPP

# include <pastel/system/for_each_container.hpp>
# include <pastel/system/for_each_neighbor_list.hpp>
# include <pastel/system/for_each_boundary_neighbor_list.hpp>
# include <pastel/sph/update_sph_divergences.hpp>
# include <pastel/sph/update_boundary_sph_divergences.hpp>
# include <pastel/container/clear_sph_divergences.hpp>


namespace pastel
{
  namespace system
  {
    namespace update_sph_divergences_detail
    {
      struct clear_sph_divergences
      {
        template <typename Particles, typename ExternalForce>
        void operator()(Particles& particles, ExternalForce&&) const
        { ::pastel::container::clear_sph_divergences(particles); }
      }; // struct clear_sph_divergences

      struct update_sph_divergences
      {
        template <typename NeighborList, typename System>
        void operator()(NeighborList const& neighbor_list, System& system) const
        { ::pastel::sph::update_sph_divergences(neighbor_list.force(), neighbor_list, system); }
      }; // struct update_sph_divergences

      struct update_boundary_sph_divergences
      {
        template <typename BoundaryNeighborList, typename System>
        void operator()(BoundaryNeighborList const& boundary_neighbor_list, System& system) const
        { ::pastel::sph::update_boundary_sph_divergences(neighbor_list.force(), boundary_neighbor_list, system); }
      }; // struct update_boundary_sph_density_force
    } // namespace update_sph_divergences_detail

    template <typename System>
    inline void update_sph_divergences(System& system)
    {
      ::pastel::system::for_each_container(
        system,
        ::pastel::system::update_sph_divergences_detail::clear_sph_divergences{});
      ::pastel::system::for_each_neighbor_list(
        system,
        ::pastel::system::update_sph_divergences_detail::update_sph_divergences{});
      ::pastel::system::for_each_boundary_neighbor_list(
        system,
        ::pastel::system::update_sph_divergences_detail::update_boundary_sph_divergences{});
    }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_UPDATE_SPH_DIVERGENCES_HPP

