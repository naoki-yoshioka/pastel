#ifndef PASTEL_UTILITY_TUPLE_META_FIND_IF_HPP
# define PASTEL_UTILITY_TUPLE_META_FIND_IF_HPP

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
        namespace find_if_detail
        {
          template <std::size_t result, template <typename> typename Predicate, typename T, typename... Args>
          struct find_if
            : std::conditional<
                Predicate<T>::value,
                std::integral_constant<std::size_t, result>,
                typename ::pastel::utility::tuple::meta::find_if_detail::find_if<result+1u, Predicate, Args...>::type>
          { };

          template <std::size_t result, template <typename> typename Predicate>
          struct find_if<result, Predicate>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace find_if_detail


        template <typename Tuple, template <typename> typename Predicate>
        struct find_if;

        template <template <typename> typename Predicate, typename... Args>
        struct find_if<std::tuple<Args...>, Predicate>
          : ::pastel::utility::tuple::meta::find_if_detail::find_if<0u, Predicate, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_FIND_IF_HPP

