#ifndef PASTEL_UTILITY_TUPLE_META_SEARCH_N_HPP
# define PASTEL_UTILITY_TUPLE_META_SEARCH_N_HPP

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
        namespace search_n_detail
        {
          template <std::size_t result, std::size_t index, typename Tuple, std::size_t n, typename Type, template <typename, typename> typename Predicate>
          struct search_n
            : std::conditional<
                result+index < std::tuple_size<Tuple>::value,
                typename std::conditional<
                  Predicate<typename std::tuple_element<Tuple, result+index>::type, Type>::value,
                  typename ::pastel::utility::tuple::meta::search_n_detail::search_n<result, index+1u, Tuple, n, Type, Predicate>::type,
                  typename ::pastel::utility::tuple::meta::search_n_detail::search_n<result+1u, 0u, Tuple, n, Type, Predicate>::type>::type,
                std::integral_constant<std::size_t, std::tuple_size<Tuple>::value>>::type
          { };

          template <std::size_t result, typename Tuple, std::size_t n, typename Type, template <typename, typename> typename Predicate>
          struct search_n<result, n, Tuple, n, Type, Predicate>
            : std::integral_consant<std::size_t, result>
          { };
        } // namespace search_n_detail


        template <typename Tuple, std::size_t n, typename Type, template <typename, typename> typename Predicate = std::is_same>
        struct search_n;

        template <typename... Args, std::size_t n, typename Type, template <typename, typename> typename Predicate>
        struct search_n<std::tuple<Args...>, n, Type, Predicate>
          : ::pastel::utility::tuple::meta::search_n_detail::search_n<
              0u, 0u, std::tuple<Args...>, n, Type, Predicate>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_SEARCH_N_HPP

