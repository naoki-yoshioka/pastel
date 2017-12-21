#ifndef PASTEL_UTILITY_TUPLE_META_FIND_END_HPP
# define PASTEL_UTILITY_TUPLE_META_FIND_END_HPP

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
        namespace find_end_detail
        {
          template <std::size_t result, std::size_t index, std::size_t compare_index, typename Tuple, typename CompareTuple, template <typename, typename> typename Predicate>
          struct find_end
            : std::conditional<
                index+compare_index < std::tuple_size<Tuple>::value,
                typename std::conditional<
                  Predicate<
                    typename std::tuple_element<Tuple, index+compare_index>::type,
                    typename std::tuple_element<CompareTuple, compare_index>::type>::value,
                  typename ::pastel::utility::tuple::meta::find_end_detail::find_end<result, index, compare_index+1u, Tuple, CompareTuple, Predicate>::type,
                  typename ::pastel::utility::tuple::meta::find_end_detail::find_end<result, index+1u, 0u, Tuple, CompareTuple, Predicate>::type>::type,
                typename ::pastel::utility::tuple::meta::find_end_detail::find_end<result, index+1u, 0u, Tuple, CompareTuple, Predicate>::type>::type
          { };

          template <std::size_t result, std::size_t index, typename Tuple, typename CompareTuple, template <typename, typename> typename Predicate>
          struct find_end<result, index, std::tuple_size<CompareTuple>::value, Tuple, CompareTuple, Predicate>
            : ::pastel::utility::tuple::meta::find_end_detail::find_end<index, index+1u, 0u, Tuple, CompareTuple, Predicate>
          { };

          template <std::size_t result, typename Tuple, typename CompareTuple, template <typename, typename> typename Predicate>
          struct find_end<result, std::tuple_size<Tuple>::value, 0u, Tuple, CompareTuple, Predicate>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace find_end_detail


        template <typename Tuple, typename CompareTuple, template <typename, typename> typename Predicate = std::is_same>
        struct find_end;

        template <typename... Args, typename... CompareArgs, template <typename, typename> typename Predicate>
        struct find_end<std::tuple<Args...>, std::tuple<CompareArgs...>, Predicate>
          : ::pastel::utility::tuple::meta::find_end_detail::find_end<
              0u, 0u, 0u, std::tuple<Args...>, std::tuple<CompareArgs...>, Predicate>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_FIND_END_HPP

