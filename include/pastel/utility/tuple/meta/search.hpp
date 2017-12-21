#ifndef PASTEL_UTILITY_TUPLE_META_SEARCH_HPP
# define PASTEL_UTILITY_TUPLE_META_SEARCH_HPP

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
        namespace search_detail
        {
          template <std::size_t result, std::size_t index, typename Tuple, typename CompareTuple, template <typename, typename> typename Predicate>
          struct search
            : std::conditional<
                result+index < std::tuple_size<Tuple>::value,
                typename std::conditional<
                  Predicate<
                    typename std::tuple_element<Tuple, result+index>::type,
                    typename std::tuple_element<CompareTuple, index>::type>::value,
                  typename ::pastel::utility::tuple::meta::search_detail::search<result, index+1u, Tuple, CompareTuple, Predicate>::type,
                  typename ::pastel::utility::tuple::meta::search_detail::search<result+1u, 0u, Tuple, CompareTuple, Predicate>::type>::type,
                std::integral_constant<std::size_t, std::tuple_size<Tuple>::value>>::type
          { };

          template <std::size_t result, typename Tuple, typename CompareTuple, template <typename, typename> typename Predicate>
          struct search<result, std::tuple_size<CompareTuple>::value, Tuple, CompareTuple, Predicate>
            : std::integral_consant<std::size_t, result>
          { };
        } // namespace search_detail


        template <typename Tuple, typename CompareTuple, template <typename, typename> typename Predicate = std::is_same>
        struct search;

        template <typename... Args, typename... CompareArgs, template <typename, typename> typename Predicate>
        struct search<std::tuple<Args...>, std::tuple<CompareArgs...>, Predicate>
          : ::pastel::utility::tuple::meta::search_detail::search<
              0u, 0u, std::tuple<Args...>, std::tuple<CompareArgs...>, Predicate>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_SEARCH_HPP

