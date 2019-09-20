#ifndef PASTEL_NEIGHBOR_DETAIL_UPDATE_STATUS_HPP
# define PASTEL_NEIGHBOR_DETAIL_UPDATE_STATUS_HPP

# include <pastel/container/maximal_speed.hpp>


namespace pastel
{
  namespace neighbor
  {
    namespace detail
    {
      template <typename NeighborList, typename Particles, typename Value>
      inline Value update_status(NeighborList const& neighbor_list, Particles const& particles, Value time_step, Value buffer_length)
      { return buffer_length - Value{2} * time_step * ::pastel::container::maximal_speed(particles); }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Value>
      inline Value update_status(NeighborList const& neighbor_list, KeyParticles const& key_particles, PartnerParticles const& partner_particles, Value time_step, Value buffer_length)
      {
        return buffer_length - Value{2} * time_step * std::max(
          ::pastel::container::maximal_speed(key_particles),
          ::pastel::container::maximal_speed(partner_particles));
      }
    } // namespace detail
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_DETAIL_UPDATE_STATUS_HPP

