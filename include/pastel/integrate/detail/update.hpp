#ifndef PASTEL_INTEGRATE_DETAIL_UPDATE_HPP
# define PASTEL_INTEGRATE_DETAIL_UPDATE_HPP

# include <cstddef>

# include <pastel/system/for_each_neighbor_list.hpp>
# include <pastel/system/for_each_boundary_neighbor_list.hpp>
# include <pastel/system/any_of_neighbor_lists.hpp>
# include <pastel/system/any_of_boundary_neighbor_lists.hpp>
# ifndef NDEBUG
#   include <pastel/system/all_of_neighbor_lists.hpp>
#   include <pastel/system/all_of_boundary_neighbor_lists.hpp>
# endif
# include <pastel/system/particles.hpp>
# include <pastel/system/boundary.hpp>
# include <pastel/system/meta/dimension_of.hpp>
# include <pastel/neighbor/meta/interaction_pair_of.hpp>
# include <pastel/utility/for_.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace detail
    {
      namespace update_detail
      {
        template <std::size_t key_index, std::size_t partner_index>
        struct update_status_impl
        {
          template <typename Updater, typename NeighborList, typename System, typename Time>
          static void call(Updater& updater, NeighborList const& neighbor_list, System const& system, Time time_step)
          {
            updater.update_status(
              neighbor_list,
              ::pastel::system::particles<key_index>(system),
              ::pastel::system::particles<partner_index>(system),
              time_step);
          }
        }; // struct update_status_impl<key_index, partner_index>

        template <std::size_t index>
        struct update_status_impl<index, index>
        {
          template <typename Updater, typename NeighborList, typename System, typename Time>
          static void call(Updater& updater, NeighborList const& neighbor_list, System const& system, Time time_step)
          {
            updater.update_status(
              neighbor_list,
              ::pastel::system::particles<index>(system),
              time_step);
          }
        }; // struct update_status_impl<index, index>

        template <typename Time>
        struct update_status
        {
         private:
          Time time_step_;

         public:
          update_status(Time time_step) : time_step_{time_step} { }

          template <typename NeighborList, typename System>
          void operator()(NeighborList& neighbor_list, System const& system) const
          {
            using interaction_pair_type
              = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
            using update_status_impl_func
              = ::pastel::neighbor::detail::update_detail::update_status_impl<interaction_pair_type::first, interaction_pair_type::second>;
            update_status_impl_func::call(neighbor_list.updater(), neighbor_list, system, time_step_);
          }
        }; // struct update_status<Time>


        template <typename Time>
        struct update_boundary_status
        {
         private:
          Time time_step_;

         public:
          update_boundary_status(Time time_step) : time_step_{time_step} { }

          template <typename BoundaryNeighborList, typename System>
          void operator()(BoundaryNeighborList& boundary_neighbor_list, System const& system) const
          {
            using interaction_pair_type
              = typename ::pastel::neighbor::meta::interaction_pair_of<BoundaryNeighborList>::type;
            boundary_neighbor_list.updater().update_status(
              boundary_neighbor_list,
              ::pastel::system::particles<interaction_pair_type::first>(system),
              ::pastel::system::boundary_particles<interaction_pair_type::second>(system),
              time_step_);
          }
        }; // struct update_boundary_status<Time>


        template <std::size_t key_index, std::size_t partner_index>
        struct reset_status_impl
        {
          template <typename Updater, typename NeighborList, typename System, typename Time>
          static void call(Updater& updater, NeighborList const& neighbor_list, System const& system, Time time_step)
          {
            updater.reset_status(
              neighbor_list,
              ::pastel::system::particles<key_index>(system),
              ::pastel::system::particles<partner_index>(system),
              time_step);
          }
        }; // struct reset_status_impl<key_index, partner_index>

        template <std::size_t index>
        struct reset_status_impl<index, index>
        {
          template <typename Updater, typename NeighborList, typename System, typename Time>
          static void call(Updater& updater, NeighborList const& neighbor_list, System const& system, Time time_step)
          {
            updater.reset_status(
              neighbor_list,
              ::pastel::system::particles<index>(system),
              time_step);
          }
        }; // struct reset_status_impl<index, index>

        template <typename Time>
        struct reset_status
        {
         private:
          Time time_step_;

         public:
          reset_status(Time time_step) : time_step_{time_step} { }

          template <typename NeighborList, typename System>
          void operator()(NeighborList& neighbor_list, System const& system) const
          {
            using interaction_pair_type
              = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
            using reset_status_impl_func
              = ::pastel::neighbor::detail::update_detail::reset_status_impl<interaction_pair_type::first, interaction_pair_type::second>;
            reset_status_impl_func::call(neighbor_list.updater(), neighbor_list, system, time_step_);
          }
        }; // struct update_status<Time>


        template <typename Time>
        struct reset_boundary_status
        {
         private:
          Time time_step_;

         public:
          reset_boundary_status(Time time_step) : time_step_{time_step} { }

          template <typename BoundaryNeighborList, typename System>
          void operator()(BoundaryNeighborList& boundary_neighbor_list, System const& system) const
          {
            using interaction_pair_type
              = typename ::pastel::neighbor::meta::interaction_pair_of<BoundaryNeighborList>::type;
            boundary_neighbor_list.updater().reset_status(
              boundary_neighbor_list,
              ::pastel::system::particles<interaction_pair_type::first>(system),
              ::pastel::system::boundary_particles<interaction_pair_type::second>(system),
              time_step_);
          }
        }; // struct reset_boundary_status<Time>


        template <std::size_t key_index, std::size_t partner_index>
        struct update_neighbor_list_impl
        {
          template <typename Updater, typename NeighborList, typename System>
          static void call(Updater const& updater, NeighborList& neighbor_list, System const& system)
          {
            updater.update_neighbor_list(
              neighbor_list,
              ::pastel::system::particles<key_index>(system),
              ::pastel::system::particles<partner_index>(system));
          }
        }; // struct update_neighbor_list_impl<key_index, partner_index>

        template <std::size_t index>
        struct update_neighbor_list_impl<index, index>
        {
          template <typename Updater, typename NeighborList, typename System>
          static void call(Updater const& updater, NeighborList& neighbor_list, System const& system)
          {
            updater.update_neighbor_list(
              neighbor_list,
              ::pastel::system::particles<index>(system));
          }
        }; // struct update_neighbor_list_impl<index, index>

        struct update_neighbor_list
        {
          template <typename NeighborList, typename System>
          void operator()(NeighborList& neighbor_list, System const& system) const
          {
            using interaction_pair_type
              = typename ::pastel::neighbor::meta::interaction_pair_of<NeighborList>::type;
            using update_neighbor_list_impl_func
              = ::pastel::neighbor::detail::update_detail::update_neighbor_list_impl<interaction_pair_type::first, interaction_pair_type::second>;
            update_neighbor_list_impl_func::call(neighbor_list.updater(), neighbor_list, system);
          }
        }; // struct update_neighbor_list


        struct update_boundary_neighbor_list
        {
          template <typename BoundaryNeighborList, typename System>
          void operator()(BoundaryNeighborList& boundary_neighbor_list, System const& system) const
          {
            using interaction_pair_type
              = typename ::pastel::neighbor::meta::interaction_pair_of<BoundaryNeighborList>::type;
            boundary_neighbor_list.updater().update_neighbor_list(
              boundary_neighbor_list,
              ::pastel::system::particles<interaction_pair_type::first>(system),
              ::pastel::system::boundary_particles<interaction_pair_type::second>(system));
          }
        }; // struct update_boundary_neighbor_list


# ifndef NDEBUG
        struct is_valid
        {
          template <typename NeighborList, typename System>
          bool operator()(NeighborList const& neighbor_list, System const&) const
          { return neighbor_list.updater().is_valid(); }
        }; // struct is_valid

# endif // NDEBUG
        struct is_invalid
        {
          template <typename NeighborList, typename System>
          bool operator()(NeighborList const& neighbor_list, System const&) const
          { return neighbor_list.updater().is_invalid(); }
        }; // struct is_invalid


        struct clear
        {
          template <typename BoundaryParticles>
          void operator()(BoundaryParticles& boundary_particles) const
          { boundary_particles.clear(); }
        }; // struct clear


        template <std::size_t n, std::size_t index>
        struct call_modify_outside_particles_impl
        {
          static void call(System& system)
          { ::pastel::system::boundary<n>(system).modify_outside_particles<n, index>(system); }
        }; // struct call_modify_outside_particles_impl<index>

        template <std::size_t n>
        struct call_modify_outside_particles
        {
          static void call(System& system)
          {
            template <std::size_t index>
            using call_modify_outside_particles_impl_func
              = ::pastel::integrate::detail::update_detail::call_modify_outside_particles_impl<n, index>;
            ::pastel::utility::for_<std::size_t, 0u, ::pastel::system::meta::boundary_particle_size_of<System>::value, call_modify_outside_particles_impl_func>::call(system);
          }
        }; // struct call_modify_outside_particles<n>


        template <std::size_t n, std::size_t index>
        struct call_initialize_boundary_particles_impl
        {
          static void call(System& system)
          { ::pastel::system::boundary<n>(system).initialize_boundary_particles<n, index>(system); }
        }; // struct call_initialize_boundary_particles_impl<index>

        template <std::size_t n>
        struct call_initialize_boundary_particles
        {
          static void call(System& system)
          {
            template <std::size_t index>
            using call_initialize_boundary_particles_impl_func
              = ::pastel::integrate::detail::update_detail::call_initialize_boundary_particles_impl<n, index>;
            ::pastel::utility::for_<std::size_t, 0u, ::pastel::system::meta::boundary_particle_size_of<System>::value, call_initialize_boundary_particles_impl_func>::call(system);
          }
        }; // struct call_initialize_boundary_particles<n>


        template <std::size_t n, std::size_t index>
        struct call_update_boundary_particles_impl
        {
          static void call(System& system)
          { ::pastel::system::boundary<n>(system).update_boundary_particles<n, index>(system); }
        }; // struct call_update_boundary_particles_impl<index>

        template <std::size_t n>
        struct call_update_boundary_particles
        {
          static void call(System& system)
          {
            template <std::size_t index>
            using call_update_boundary_particles_impl_func
              = ::pastel::integrate::detail::update_detail::call_update_boundary_particles_impl<n, index>;
            ::pastel::utility::for_<std::size_t, 0u, ::pastel::system::meta::boundary_particle_size_of<System>::value, call_update_boundary_particles_impl_func>::call(system);
          }
        }; // struct call_update_boundary_particles<n>
      } // namespace update_detail


      template <typename System, typename Time, typename Function>
      inline void update(System& system, Time time_step, Function&& update_particles)
      {
        ::pastel::system::for_each_neighbor_list(
          system,
          ::pastel::integrate::detail::update_detail::update_status<Time>{time_step});

        ::pastel::system::for_each_boundary_neighbor_list(
          system,
          ::pastel::integrate::detail::update_detail::update_boundary_status<Time>{time_step});

        if (::pastel::system::any_of_neighbor_lists(
              system,
              ::pastel::integrate::detail::update_detail::is_invalid{})
            || ::pastel::system::any_of_boundary_neighbor_lists(
                 system,
                 ::pastel::Integrate::detail::update_detail::is_invalid{}))
        {
          // modify positions (periodic boundary), erase/insert particles (mpi boundary)
          using modify_outside_particles_func
            = ::pastel::utility::for_<
                std::size_t, 0u, ::pastel::system::meta::dimension_of<System>::value,
                ::pastel::integrate::detail::update_detail::call_modify_outside_particles>;
          modify_outside_particles_func::call(system);

          // clear boundary_particles
          ::pastel::system::for_each_boundary_container(
            system,
            ::pastel::integrate::detail::update_detail::clear{});

          // TODO: sort particles

          // initialize boundary
          using initialize_boundary_particles_func
            = ::pastel::utility::for_<
                std::size_t, 0u, ::pastel::system::meta::dimension_of<Systm>::value,
                ::pastel::integrate::detail::update_detail::call_initialize_boundary_particles>;
          initialize_boundary_particles_func::call(system);


          ::pastel::system::for_each_neighbor_list(
            system,
            ::pastel::integrate::detail::update_detail::update_neighbor_list{});

          ::pastel::system::for_each_neighbor_list(
            system,
            ::pastel::integrate::detail::update_detail::reset_status<Time>{time_step});

          assert(
            ::pastel::system::all_of_neighbor_lists(
              system,
              ::pastel::integrate::detail::update_detail::is_valid{}));


          ::pastel::system::for_each_boundary_neighbor_list(
            system,
            ::pastel::integrate::detail::update_detail::update_boundary_neighbor_list{});

          ::pastel::system::for_each_boundary_neighbor_list(
            system,
            ::pastel::integrate::detail::update_detail::reset_boundary_status<Time>{time_step});

          assert(
            ::pastel::system::all_of_boundary_neighbor_lists(
              system,
              ::pastel::integrate::detail::update_detail::is_valid{}));
        }
        else
        {
          // update boundary
          using update_boundary_particles_func
            = ::pastel::utility::for_<
                std::size_t, 0u, ::pastel::system::meta::dimension_of<Systm>::value,
                ::pastel::integrate::detail::update_detail::call_update_boundary_particles>;
          update_boundary_particles_func::call(system);
        }

        update_particles(system, time_step);
      }
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_UPDATE_HPP

