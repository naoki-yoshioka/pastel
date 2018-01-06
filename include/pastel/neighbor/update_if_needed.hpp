#ifndef PASTEL_NEIGHBOR_UPDATED_IF_NEEDED_HPP
# define PASTEL_NEIGHBOR_UPDATED_IF_NEEDED_HPP

# include <cassert>
# include <cstddef>

# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/system/particles.hpp>


namespace pastel
{
  namespace neighbor
  {
    namespace update_if_needed_detail
    {
      template <std::size_t key_index, std::size_t partner_index>
      struct update_if_needed
      {
        template <typename NeighborList, typename System, typename Time>
        static void call(NeighborList& neighbor_list, System const& system, Time time_step)
        {
          auto& updater = neighbor_list.updater();

          updater.update_status(neighbor_list, ::pastel::system::particles<key_index>(system), ::pastel::system::particles<partner_index>(system), time_step);
          if (updater.is_valid())
            return;

          updater.update_neighbor_list(neighbor_list, ::pastel::system::particles<key_index>(system), ::pastel::system::particles<partner_index>(system));
          updater.reset_status(neighbor_list, ::pastel::system::particles<key_index>(system), ::pastel::system::particles<partner_index>(system), time_step);
          assert(updater.is_valid());
        }
      }; // struct update_if_needed<key_index, partner_index>

      template <std::size_t index>
      struct update_if_needed<index, index>
      {
        template <typename NeighborList, typename System, typename Time>
        static void call(NeighborList& neighbor_list, System const& system, Time time_step)
        {
          auto& updater = neighbor_list.updater();

          updater.update_status(neighbor_list, ::pastel::system::particles<index>(system), time_step);
          if (updater.is_valid())
            return;

          updater.update_neighbor_list(neighbor_list, ::pastel::system::particles<index>(system));
          updater.reset_status(neighbor_list, ::pastel::system::particles<index>(system), time_step);
          assert(updater.is_valid());
        }
      }; // struct update_if_needed<index, index>
    } // namespace update_if_needed_detail


    template <typename NeighborList, typename System, typename Time>
    inline void update_if_needed(NeighborList& neighbor_list, System const& system, Time time_step)
    {
      using interaction_pair_type
        = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
      using update_if_needed_func
        = ::pastel::neighbor::update_if_needed_detail::update_if_needed<interaction_pair_type::first, interaction_pair_type::second>;
      update_if_needed_func::call(neighbor_list, system, time_step);
    }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_UPDATED_IF_NEEDED_HPP

