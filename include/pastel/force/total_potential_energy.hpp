#ifndef PASTEL_FORCE_TOTAL_POTENTIAL_ENERGY_HPP
# define PASTEL_FORCE_TOTAL_POTENTIAL_ENERGY_HPP

# include <pastel/force/tags.hpp>
# include <pastel/force/meta/has_potential_energy.hpp>
# include <pastel/force/meta/value_of.hpp>
# include <pastel/force/meta/required_arguments_category_of.hpp>
# include <pastel/neighbor/num_keys.hpp>
# include <pastel/neighbor/num_partners.hpp>
# include <pastel/neighbor/partner_begin.hpp>
# include <pastel/neighbor/partner_end.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/neighbor/meta/is_partner_data_accessible.hpp>
# include <pastel/system/particles.hpp>
# include <pastel/container/get.hpp>


namespace pastel
{
  namespace force
  {
    namespace total_potential_energy_detail
    {
      template <bool is_partner_data_accessible>
      struct total_potential_energy;

      template <>
      struct total_potential_energy<true>
      {
        template <typename Force, typename NeighborList, typename System>
        static typename ::pastel::force::meta::value_of<Force>::type call(
          Force const& force, NeighborList const& neighbor_list, System const& system,
          ::pastel::force::tags::requires_position)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          auto result = typename ::pastel::force::meta::value_of<Force>::type{0};
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              result
                += force.energy(
                    key_position,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner));
            }
          }

          return result;
        }

        /*
        template <typename Force, typename NeighborList, typename System>
        static typename ::pastel::force::meta::value_of<Force>::type call(
          Force const& force, NeighborList const& neighbor_list, System const& system,
          ::pastel::force::tags::requires_position_orientation)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          auto result = typename ::pastel::force::meta::value_of<Force>::type{0};
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_orientation
              = ::pastel::container::get< ::pastel::particle::tags::orientation >(key_particles, key);

            auto const num_partners = ::pastel::neighbor::num_partners(neighbor_list, key);
            auto const partner_data = neighbor_list.partner_data(key);
            for (auto partner_index = static_cast<decltype(num_partners)>(0); partner_index < num_partners; ++partner_index)
            {
              auto const partner = partner_data[partner_index];
              result
                += force.energy(
                    key_position, key_orientation,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, partner),
                    ::pastel::container::get< ::pastel::particle::tags::orientation >(
                      partner_particles, partner));
            }
          }

          return result;
        }
        */
      }; // struct total_potential_energy<true>

      template <>
      struct total_potential_energy<false>
      {
        template <typename Force, typename NeighborList, typename System>
        static typename ::pastel::force::meta::value_of<Force>::type call(
          Force const& force, NeighborList const& neighbor_list, System const& system,
          ::pastel::force::tags::requires_position)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          auto result = typename ::pastel::force::meta::value_of<Force>::type{0};
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
              result
                += force.energy(
                    key_position,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter));
          }

          return result;
        }

        /*
        template <typename Force, typename NeighborList, typename System>
        static typename ::pastel::force::meta::value_of<Force>::type call(
          Force const& force, NeighborList const& neighbor_list, System const& system,
          ::pastel::force::tags::requires_position_orientation)
        {
          using interaction_pair_type
            = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
          auto& key_particles
            = ::pastel::system::particles<interaction_pair_type::first>(system);
          auto& partner_particles
            = ::pastel::system::particles<interaction_pair_type::second>(system);

          auto result = typename ::pastel::force::meta::value_of<Force>::type{0};
          auto const num_keys = ::pastel::neighbor::num_keys(neighbor_list);
          for (auto key = static_cast<decltype(num_keys)>(0); key < num_keys; ++key)
          {
            auto const& key_position
              = ::pastel::container::get< ::pastel::particle::tags::position >(key_particles, key);
            auto const& key_orientation
              = ::pastel::container::get< ::pastel::particle::tags::orientation >(key_particles, key);

            auto const partner_end = ::pastel::neighbor::partner_end(neighbor_list, key);
            for (auto partner_iter = ::pastel::neighbor::partner_begin(neighbor_list, key);
                 partner_iter != partner_end; ++partner_iter)
              result
                += force.energy(
                    key_position, key_orientation,
                    ::pastel::container::get< ::pastel::particle::tags::position >(
                      partner_particles, *partner_iter),
                    ::pastel::container::get< ::pastel::particle::tags::orientation >(
                      partner_particles, *partner_iter));
          }

          return result;
        }
        */
      }; // struct total_potential_energy<false>
    } // namespace total_potential_energy_detail


    template <typename Force, typename NeighborList, typename System>
    inline typename ::pastel::force::meta::value_of<Force>::type total_potential_energy(
      Force const& force, NeighborList const& neighbor_list, System const& system)
    {
      static_assert(
        ::pastel::force::meta::has_potential_energy<Force>::value,
        "Force must have member function energy to calculate potential energy");

      static constexpr bool is_partner_data_accessible
        = ::pastel::neighbor::meta::is_partner_data_accessible<NeighborList>::value;
      using required_arguments_category
        = typename ::pastel::force::meta::required_arguments_category_of<Force>::type;
      return ::pastel::force::total_potential_energy_detail::total_potential_energy<is_partner_data_accessible>::call(
        force, neighbor_list, system, required_arguments_category{});
    }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_TOTAL_POTENTIAL_ENERGY_HPP

