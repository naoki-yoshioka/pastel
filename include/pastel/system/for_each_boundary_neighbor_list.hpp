#ifndef PASTEL_SYSTEM_FOR_EACH_BOUNDARY_NEIGHBOR_LIST_HPP
# define PASTEL_SYSTEM_FOR_EACH_BOUNDARY_NEIGHBOR_LIST_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/boundary_neighbor_list.hpp>
# include <pastel/system/meta/boundary_neighbor_list_tuple_size_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace for_each_boundary_neighbor_list_detail
    {
      template <std::size_t n, std::size_t last>
      struct nth_step
      {
        template <typename System, typename Function>
        static void call(System& system, Function&& function)
        {
          function(::pastel::system::boundary_neighbor_list<n>(system), system);
          function(::pastel::system::boundary_neighbor_list<n+1u>(system), system);
          function(::pastel::system::boundary_neighbor_list<n+2u>(system), system);
          function(::pastel::system::boundary_neighbor_list<n+3u>(system), system);

          ::pastel::system::for_each_boundary_neighbor_list_detail::nth_step<n+4u, last, System>::call(
            system, std::forward<Function>(function));
        }

        template <typename System, typename Function>
        static void call(System const& system, Function&& function)
        {
          function(::pastel::system::boundary_neighbor_list<n>(system), system);
          function(::pastel::system::boundary_neighbor_list<n+1u>(system), system);
          function(::pastel::system::boundary_neighbor_list<n+2u>(system), system);
          function(::pastel::system::boundary_neighbor_list<n+3u>(system), system);

          ::pastel::system::for_each_boundary_neighbor_list_detail::nth_step<n+4u, last, System>::call(
            system, std::forward<Function>(function));
        }
      }; // struct nth_step<n, last>

      template <std::size_t last>
      struct nth_step<last-3u, last>
      {
        template <typename System, typename Function>
        static void call(System& system, Function&& function)
        {
          function(::pastel::system::boundary_neighbor_list<last-3u>(system), system);
          function(::pastel::system::boundary_neighbor_list<last-2u>(system), system);
          function(::pastel::system::boundary_neighbor_list<last-1u>(system), system);
        }

        template <typename System, typename Function>
        static void call(System const& system, Function&& function)
        {
          function(::pastel::system::boundary_neighbor_list<last-3u>(system), system);
          function(::pastel::system::boundary_neighbor_list<last-2u>(system), system);
          function(::pastel::system::boundary_neighbor_list<last-1u>(system), system);
        }
      }; // struct nth_step<last-3u, last>

      template <std::size_t last>
      struct nth_step<last-2u, last>
      {
        template <typename System, typename Function>
        static void call(System& system, Function&& function)
        {
          function(::pastel::system::boundary_neighbor_list<last-2u>(system), system);
          function(::pastel::system::boundary_neighbor_list<last-1u>(system), system);
        }

        template <typename System, typename Function>
        static void call(System const& system, Function&& function)
        {
          function(::pastel::system::boundary_neighbor_list<last-2u>(system), system);
          function(::pastel::system::boundary_neighbor_list<last-1u>(system), system);
        }
      }; // struct nth_step<last-2u, last>

      template <std::size_t last>
      struct nth_step<last-1u, last>
      {
        template <typename System, typename Function>
        static void call(System& system, Function&& function)
        {
          function(::pastel::system::boundary_neighbor_list<last-1u>(system), system);
        }

        template <typename System, typename Function>
        static void call(System const& system, Function&& function)
        {
          function(::pastel::system::boundary_neighbor_list<last-1u>(system), system);
        }
      }; // struct nth_step<last-1u, last>

      template <std::size_t last>
      struct nth_step<last, last>
      {
        template <typename System, typename Function>
        static void call(System&, Function&&)
        { }

        template <typename System, typename Function>
        static void call(System const&, Function&&)
        { }
      }; // struct nth_step<last, last>
    } // namespace for_each_boundary_neighbor_list_detail


    template <typename System, typename Function>
    inline void for_each_boundary_neighbor_list(System& system, Function&& function)
    {
      ::pastel::system::for_each_boundary_neighbor_list_detail::nth_step<0u, ::pastel::system::meta::boundary_neighbor_list_tuple_size_of<System>::value, System>::call(
        system, std::forward<Function>(function));
    }

    template <typename System, typename Function>
    inline void for_each_boundary_neighbor_list(System const& system, Function&& function)
    {
      ::pastel::system::for_each_boundary_neighbor_list_detail::nth_step<0u, ::pastel::system::meta::boundary_neighbor_list_tuple_size_of<System const>::value, System>::call(
        system, std::forward<Function>(function));
    }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_FOR_EACH_BOUNDARY_NEIGHBOR_LIST_HPP

