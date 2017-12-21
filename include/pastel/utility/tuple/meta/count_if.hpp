#ifndef PASTEL_UTILITY_TUPLE_META_COUNT_IF_HPP
# define PASTEL_UTILITY_TUPLE_META_COUNT_IF_HPP

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
        namespace count_if_detail
        {
          template <std::size_t result, template <typename> typename Predicate, typename T, typename... Args>
          struct count_if
            : ::pastel::utility::tuple::meta::count_if_detail::count_if<
                Predicate<T>::value ? result+1u : result,
                Predicate, Args...>
          { };

          template <std::size_t result, template <typename> typename Predicate>
          struct count_if<result, Predicate>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace count_if_detail


        template <typename Tuple, template <typename> typename Predicate>
        struct count_if;

        template <template <typename> typename Predicate, typename... Args>
        struct count_if<std::tuple<Args...>, Predicate>
          : ::pastel::utility::tuple::meta::count_if_detail::count_if<0u, Predicate, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_COUNT_IF_HPP

