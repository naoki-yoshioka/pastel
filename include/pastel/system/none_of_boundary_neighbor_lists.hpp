#ifndef PASTEL_SYSTEM_NONE_OF_BOUNDARY_NEIGHBOR_LISTS_HPP
# define PASTEL_SYSTEM_NONE_OF_BOUNDARY_NEIGHBOR_LISTS_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/boundary_neighbor_list.hpp>
# include <pastel/system/meta/boundary_neighbor_list_tuple_size_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace none_of_boundary_neighbor_lists_detail
    {
      template <std::size_t n, std::size_t num>
      struct nth_step
      {
        template <typename System, typename Function>
        static bool call(System const& system, Function&& function)
        {
          return !function(::pastel::system::boundary_neighbor_list<n>(system), system)
            && !function(::pastel::system::boundary_neighbor_list<n+1u>(system), system)
            && !function(::pastel::system::boundary_neighbor_list<n+2u>(system), system)
            && !function(::pastel::system::boundary_neighbor_list<n+3u>(system), system)
            && ::pastel::system::none_of_boundary_neighbor_lists_detail::nth_step<n+4u, num-4u>::call(
                 system, std::forward<Function>(function));
        }
      }; // struct nth_step<n, num>

      template <std::size_t n>
      struct nth_step<n, 3u>
      {
        template <typename System, typename Function>
        static bool call(System const& system, Function&& function)
        {
          return !function(::pastel::system::boundary_neighbor_list<n>(system), system)
            && !function(::pastel::system::boundary_neighbor_list<n+1u>(system), system)
            && !function(::pastel::system::boundary_neighbor_list<n+2u>(system), system);
        }
      }; // struct nth_step<n, 3u>

      template <std::size_t n>
      struct nth_step<n, 2u>
      {
        template <typename System, typename Function>
        static bool call(System const& system, Function&& function)
        {
          return !function(::pastel::system::boundary_neighbor_list<n>(system), system)
            && !function(::pastel::system::boundary_neighbor_list<n+1u>(system), system);
        }
      }; // struct nth_step<n, 2u>

      template <std::size_t n>
      struct nth_step<n, 1u>
      {
        template <typename System, typename Function>
        static bool call(System const& system, Function&& function)
        { return !function(::pastel::system::boundary_neighbor_list<n>(system), system); }
      }; // struct nth_step<n, 1u>

      template <std::size_t n>
      struct nth_step<n, 0u>
      {
        template <typename System, typename Function>
        static bool call(System const&, Function&&) { return true; }
      }; // struct nth_step<n, 1u>
    } // namespace none_of_boundary_neighbor_lists_detail


    template <typename System, typename Function>
    inline bool none_of_boundary_neighbor_lists(System const& system, Function&& function)
    {
      return ::pastel::system::none_of_boundary_neighbor_lists_detail::nth_step<0u, ::pastel::system::meta::boundary_neighbor_list_tuple_size_of<System const>::value>::call(
        system, std::forward<Function>(function));
    }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_NONE_OF_BOUNDARY_NEIGHBOR_LISTS_HPP

