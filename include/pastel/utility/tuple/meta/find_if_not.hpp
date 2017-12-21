#ifndef PASTEL_UTILITY_TUPLE_META_FIND_IF_NOT_HPP
# define PASTEL_UTILITY_TUPLE_META_FIND_IF_NOT_HPP

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
        namespace find_if_not_detail
        {
          template <std::size_t result, template <typename> typename Predicate, typename T, typename... Args>
          struct find_if_not
            : std::conditional<
                Predicate<T>::value,
                typename ::pastel::utility::tuple::meta::find_if_not_detail::find_if_not<result+1u, Predicate, Args...>::type,
                std::integral_constant<std::size_t, result>>
          { };

          template <std::size_t result, template <typename> typename Predicate>
          struct find_if_not<result, Predicate>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace find_if_not_detail


        template <typename Tuple, template <typename> typename Predicate>
        struct find_if_not;

        template <template <typename> typename Predicate, typename... Args>
        struct find_if_not<std::tuple<Args...>, Predicate>
          : ::pastel::utility::tuple::meta::find_if_not_detail::find_if_not<0u, Predicate, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_FIND_IF_NOT_HPP

