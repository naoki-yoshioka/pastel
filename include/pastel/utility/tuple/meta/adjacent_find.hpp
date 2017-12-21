#ifndef PASTEL_UTILITY_TUPLE_META_ADJACENT_FIND_HPP
# define PASTEL_UTILITY_TUPLE_META_ADJACENT_FIND_HPP

# include <tuple>
# include <type_traits>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace adjacent_find_detail
        {
          template <std::size_t result, template <typename, typename> typename Predicate, typename... Args>
          struct adjacent_find;

          template <std::size_t result, template <typename, typename> typename Predicate, typename Type1, typename Type2, typename... Args>
          struct adjacent_find<result, Predicate, Type1, Type2, Args...>
            : std::conditional<
                Predicate<Type1, Type2>::value,
                std::integral_constant<std::size_t, result>,
                typename ::pastel::utility::tuple::meta::adjacent_find_detail::adjacent_find<result+1u, Predicate, Type2, Args...>::type>::type
          { };

          template <std::size_t result, template <typename, typename> typename Predicate, typename Type>
          struct adjacent_find<result, Predicate, Type>
            : std::integral_constant<std::size_t, result+1u>
          { };

          template <std::size_t result, template <typename, typename> typename Predicate>
          struct adjacent_find<result, Predicate>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace adjacent_find_detail


        template <typename Tuple, template <typename, typename> typename Predicate = std::is_same>
        struct adjacent_find;

        template <template <typename, typename> typename Predicate, typename... Args>
        struct adjacent_find<std::tuple<Args...>, Predicate>
          : ::pastel::utility::tuple::meta::adjacent_find_detail::adjacent_find<0u, Predicate, Args...>
        { };
      }
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_ADJACENT_FIND_HPP

