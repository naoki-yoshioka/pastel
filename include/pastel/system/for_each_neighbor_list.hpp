#ifndef PASTEL_SYSTEM_FOR_EACH_NEIGHBOR_LIST_HPP
# define PASTEL_SYSTEM_FOR_EACH_NEIGHBOR_LIST_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/neighbor_list.hpp>
# include <pastel/system/meta/neighbor_list_tuple_size_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace for_each_neighbor_list_detail
    {
      template <std::size_t n, std::size_t last, typename System>
      struct nth_step
      {
        template <typename Function>
        static void call(System& system, Function&& function)
        {
          function(::pastel::system::neighbor_list<n>(system), system);

          ::pastel::system::for_each_neighbor_list_detail::nth_step<n+1u, last, System>::call(
            system, std::forward<Function>(function));
        }

        template <typename Function>
        static void call(System const& system, Function&& function)
        {
          function(::pastel::system::neighbor_list<n>(system), system);

          ::pastel::system::for_each_neighbor_list_detail::nth_step<n+1u, last, System>::call(
            system, std::forward<Function>(function));
        }
      }; // struct nth_step<n, last, System>

      template <std::size_t last, typename System>
      struct nth_step<last, last, System>
      {
        template <typename Function>
        static void call(System&, Function&&)
        { }

        template <typename Function>
        static void call(System const&, Function&&)
        { }
      }; // struct nth_step<last, last, System>
    } // namespace for_each_neighbor_list_detail


    template <typename System, typename Function>
    inline void for_each_neighbor_list(System& system, Function&& function)
    {
      ::pastel::system::for_each_neighbor_list_detail::nth_step<0u, ::pastel::system::meta::neighbor_list_tuple_size_of<System>::value, System>::call(
        system, std::forward<Function>(function));
    }

    template <typename System, typename Function>
    inline void for_each_neighbor_list(System const& system, Function&& function)
    {
      ::pastel::system::for_each_neighbor_list_detail::nth_step<0u, ::pastel::system::meta::neighbor_list_tuple_size_of<System>::value, System>::call(
        system, std::forward<Function>(function));
    }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_FOR_EACH_NEIGHBOR_LIST_HPP

