#ifndef PASTEL_UTILITY_TUPLE_PUSH_BACK_HPP
# define PASTEL_UTILITY_TUPLE_PUSH_BACK_HPP

# include <tuple>
# include <type_traits>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace push_back_detail
      {
        template <std::size_t index, std::size_t last>
        struct push_back_after
        {
          template <typename ValueTuple, typename... Args>
          static constexpr auto call(ValueTuple const& value_tuple, Args&&... args)
            -> decltype(::pastel::utility::tuple::push_back_detail::push_back_after<index+1u, last>::call(
                 value_tuple, std::forward<Args>(args)..., std::get<index>(value_tuple)))
          {
            return ::pastel::utility::tuple::push_back_detail::push_back_after<index+1u, last>::call(
              value_tuple, std::forward<Args>(args)..., std::get<index>(value_tuple));
          }
        }; // struct push_back_after<index, last>

        template <std::size_t last>
        struct push_back_after<last, last>
        {
          template <typename ValueTuple, typename... Args>
          static constexpr std::tuple<typename std::remove_reference<Args>::type...>
          call(ValueTuple const&, Args&&... args)
          { return std::make_tuple(std::forward<Args>(args)...); }
        }; // struct push_back_after<last, last>


        template <std::size_t index, std::size_t last>
        struct push_back_before
        {
          template <typename Tuple, typename ValueTuple, typename... Args>
          static constexpr auto call(Tuple const& tuple, ValueTuple const& value_tuple, Args&&... args)
            -> decltype(::pastel::utility::tuple::push_back_detail::push_back_before<index+1u, last>::call(
                 tuple, value_tuple, std::forward<Args>(args)..., std::get<index>(tuple)))
          {
            return ::pastel::utility::tuple::push_back_detail::push_back_before<index+1u, last>::call(
              tuple, value_tuple, std::forward<Args>(args)..., std::get<index>(tuple));
          }
        }; // struct push_back_before<index, last>

        template <std::size_t last>
        struct push_back_before<last, last>
        {
          template <typename Tuple, typename... Values, typename... Args>
          static constexpr auto call(Tuple const&, std::tuple<Values...> const& value_tuple, Args&&... args)
            -> decltype(::pastel::utility::tuple::push_back_detail::push_back_after<0u, sizeof...(Values)>::call(
                 value_tuple, std::forward<Args>(args)...))
          {
            return ::pastel::utility::tuple::push_back_detail::push_back_after<0u, sizeof...(Values)>::call(
              value_tuple, std::forward<Args>(args)...);
          }
        }; // struct push_back_before<last, last>
      } // namespace push_back_detail


      template <typename... Args, typename... Values>
      inline constexpr std::tuple<Args..., typename std::remove_reverence<Values>::type...>
      push_back(std::tuple<Args...> const& tuple, Values&&... values)
      { return ::pastel::utility::tuple::push_back_detail::push_back_before<0u, sizeof...(Args)>::call(tuple, std::make_tuple(std::forward<Values>(values)...)); }
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_PUSH_BACK_HPP

