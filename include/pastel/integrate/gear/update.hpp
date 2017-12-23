#ifndef PASTEL_INTEGRATE_GEAR_UPDATE_HPP
# define PASTEL_INTEGRATE_GEAR_UPDATE_HPP

# include <cstddef>

# include <pastel/integrate/gear/update_particles.hpp>
# include <pastel/integrate/detail/update.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace gear
    {
      template <std::size_t order, std::size_t acceleration_index, typename AdditionalVectorIndexTuple, typename System, typename Time>
      inline void update(System& system, Time time_step)
      {
        static_assert(order >= 4u && order <= 6u, "order must satisfy 4u <= order <= 6u");

        ::pastel::integrate::detail::update(
          system, time_step,
          [](System& system, Time time_step)
          { ::pastel::integrate::gear::update_particles<order, acceleration_index, AdditionalVectorIndexTuple>(system, time_step); });
      }

      namespace update_detail
      {
        template <std::size_t order>
        struct update;

        template <>
        struct update<4u>
        {
          template <typename System, typename Time>
          static void call(System& system, Time time_step)
          {
            using additional_vector_index_tuple_type
              = std::tuple<std::integral_constant<std::size_t, 1u>>;
            ::pastel::integrate::gear::update<4u, 0u, additional_vector_index_tuple_type>(system, time_step);
          }
        }; // struct update<4u>

        template <>
        struct update<5u>
        {
          template <typename System, typename Time>
          static void call(System& system, Time time_step)
          {
            using additional_vector_index_tuple_type
              = std::tuple<
                  std::integral_constant<std::size_t, 1u>,
                  std::integral_constant<std::size_t, 2u>>;
            ::pastel::integrate::gear::update<5u, 0u, additional_vector_index_tuple_type>(system, time_step);
          }
        }; // struct update<5u>

        template <>
        struct update<6u>
        {
          template <typename System, typename Time>
          static void call(System& system, Time time_step)
          {
            using additional_vector_index_tuple_type
              = std::tuple<
                  std::integral_constant<std::size_t, 1u>,
                  std::integral_constant<std::size_t, 2u>,
                  std::integral_constant<std::size_t, 3u>>;
            ::pastel::integrate::gear::update<6u, 0u, additional_vector_index_tuple_type>(system, time_step);
          }
        }; // struct update<6u>
      } // namespace update_detail

      template <std::size_t order, typename System, typename Time>
      inline void update(System& system, Time time_step)
      {
        static_assert(order >= 4u && order <= 6u, "order must satisfy 4u <= order <= 6u");
        ::pastel::integrate::gear::update_detail::update<order>::call(system, time_step);
      }
    } // namespace gear
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_GEAR_UPDATE_HPP

