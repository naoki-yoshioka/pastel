#ifndef PASTEL_SYSTEM_FOR_EACH_CONTAINER_HPP
# define PASTEL_SYSTEM_FOR_EACH_CONTAINER_HPP

# include <cstddef>
# include <utility>

# include <pastel/system/particles.hpp>
# include <pastel/system/external_force.hpp>
# include <pastel/system/meta/particles_tuple_size_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace for_each_container_detail
    {
      template <std::size_t n, std::size_t last>
      struct nth_step
      {
        template <typename System, typename Function>
        static void call(System& system, Function&& function)
        {
          function(
            ::pastel::system::particles<n>(system),
            ::pastel::system::external_force<n>(system));
          function(
            ::pastel::system::particles<n+1u>(system),
            ::pastel::system::external_force<n+1u>(system));
          function(
            ::pastel::system::particles<n+2u>(system),
            ::pastel::system::external_force<n+2u>(system));
          function(
            ::pastel::system::particles<n+3u>(system),
            ::pastel::system::external_force<n+3u>(system));

          ::pastel::system::for_each_container_detail::nth_step<n+4u, last, System>::call(
            system, std::forward<Function>(function));
        }

        template <typename System, typename Function>
        static void call(System const& system, Function&& function)
        {
          function(
            ::pastel::system::particles<n>(system),
            ::pastel::system::external_force<n>(system));
          function(
            ::pastel::system::particles<n+1u>(system),
            ::pastel::system::external_force<n+1u>(system));
          function(
            ::pastel::system::particles<n+2u>(system),
            ::pastel::system::external_force<n+2u>(system));
          function(
            ::pastel::system::particles<n+3u>(system),
            ::pastel::system::external_force<n+3u>(system));

          ::pastel::system::for_each_container_detail::nth_step<n+4u, last, System>::call(
            system, std::forward<Function>(function));
        }
      }; // struct nth_step<n, last>

      template <std::size_t last>
      struct nth_step<last-3u, last>
      {
        template <typename System, typename Function>
        static void call(System& system, Function&& function)
        {
          function(
            ::pastel::system::particles<last-3u>(system),
            ::pastel::system::external_force<last-3u>(system));
          function(
            ::pastel::system::particles<last-2u>(system),
            ::pastel::system::external_force<last-2u>(system));
          function(
            ::pastel::system::particles<last-1u>(system),
            ::pastel::system::external_force<last-1u>(system));
        }

        template <typename System, typename Function>
        static void call(System const& system, Function&& function)
        {
          function(
            ::pastel::system::particles<last-3u>(system),
            ::pastel::system::external_force<last-3u>(system));
          function(
            ::pastel::system::particles<last-2u>(system),
            ::pastel::system::external_force<last-2u>(system));
          function(
            ::pastel::system::particles<last-1u>(system),
            ::pastel::system::external_force<last-1u>(system));
        }
      }; // struct nth_step<last-3u, last>

      template <std::size_t last>
      struct nth_step<last-2u, last>
      {
        template <typename System, typename Function>
        static void call(System& system, Function&& function)
        {
          function(
            ::pastel::system::particles<last-2u>(system),
            ::pastel::system::external_force<last-2u>(system));
          function(
            ::pastel::system::particles<last-1u>(system),
            ::pastel::system::external_force<last-1u>(system));
        }

        template <typename System, typename Function>
        static void call(System const& system, Function&& function)
        {
          function(
            ::pastel::system::particles<last-2u>(system),
            ::pastel::system::external_force<last-2u>(system));
          function(
            ::pastel::system::particles<last-1u>(system),
            ::pastel::system::external_force<last-1u>(system));
        }
      }; // struct nth_step<last-2u, last>

      template <std::size_t last>
      struct nth_step<last-1u, last>
      {
        template <typename System, typename Function>
        static void call(System& system, Function&& function)
        {
          function(
            ::pastel::system::particles<last-1u>(system),
            ::pastel::system::external_force<last-1u>(system));
        }

        template <typename System, typename Function>
        static void call(System const& system, Function&& function)
        {
          function(
            ::pastel::system::particles<last-1u>(system),
            ::pastel::system::external_force<last-1u>(system));
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
    } // namespace for_each_container_detail


    template <typename System, typename Function>
    inline void for_each_container(System& system, Function&& function)
    {
      ::pastel::system::for_each_container_detail::nth_step<0u, ::pastel::system::meta::particles_tuple_size_of<System>::value>::call(
        system, std::forward<Function>(function));
    }

    template <typename System, typename Function>
    inline void for_each_container(System const& system, Function&& function)
    {
      ::pastel::system::for_each_container_detail::nth_step<0u, ::pastel::system::meta::particles_tuple_size_of<System const>::value>::call(
        system, std::forward<Function>(function));
    }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_FOR_EACH_CONTAINER_HPP

