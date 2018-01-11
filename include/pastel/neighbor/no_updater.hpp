#ifndef PASTEL_NEIGHBOR_NO_UPDATER_HPP
# define PASTEL_NEIGHBOR_NO_UPDATER_HPP


namespace pastel
{
  namespace neighbor
  {
    struct no_updater
    {
      constexpr bool is_valid() const { return true; }
      constexpr bool is_invalid() const { return false; }

      constexpr bool make_invalid_if_possible() const { return false; }


      template <typename NeighborList, typename Particles, typename Time>
      constexpr void reset_status(NeighborList const&, Particles const&, Time) const { }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      constexpr void reset_status(NeighborList const&, KeyParticles const&, PartnerParticles const&, Time) const { }


      template <typename NeighborList, typename Particles, typename Time>
      constexpr void update_status(NeighborList const&, Particles const&, Time) const { }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles, typename Time>
      constexpr void update_status(NeighborList const&, KeyParticles const&, PartnerParticles const&, Time) const { }


      template <typename NeighborList, typename Particles>
      constexpr void update_neighbor_list(NeighborList const&, Particles const&) const { }

      template <typename NeighborList, typename KeyParticles, typename PartnerParticles>
      constexpr void update_neighbor_list(NeighborList const&, KeyParticles const&, PartnerParticles const&) const { }
    }; // struct no_updater
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_NO_UPDATER_HPP

