#ifndef PASTEL_UTILITY_TUPLE_META_FIND_FIRST_OF_HPP
# define PASTEL_UTILITY_TUPLE_META_FIND_FIRST_OF_HPP

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
        namespace find_first_of_detail
        {
          template <std::size_t result, std::size_t index, typename CompareTuple, template <typename, typename> typename Predicate, typename... Args>
          struct find_first_of;

          template <std::size_t result, std::size_t index, typename CompareTuple, template <typename, typename> typename Predicate, typename Type, typename... Args>
          struct find_first_of<result, index, CompareTuple, Predicate, Type, Args>
            : std::conditional<
                Predicate<Type, typename std::tuple_element<CompareTuple, index>::type>::value,
                std::integral_constant<std::size_t, result>,
                typename ::pastel::utility::tuple::meta::find_first_of_detail::find_first_of<result, index+1u, CompareTuple, Predicate, Type, Args...>::type>::type
          { };

          template <std::size_t result, typename CompareTuple, template <typename, typename> typename Predicate, typename Type, typename... Args>
          struct find_first_of<result, std::tuple_size<CompareTuple>::value, CompareTuple, Predicate, Type, Args...>
            : ::pastel::utility::tuple::meta::find_first_of_detail::find_first_of<result+1u, 0u, CompareTuple, Predicate, Args...>
          { };

          template <std::size_t result, typename CompareTuple, template <typename, typename> typename Predicate>
          struct find_first_of<result, 0u, CompareTuple, Predicate>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace find_first_of_detail


        template <typename Tuple, typename CompareTuple, template <typename, typename> typename Predicate = std::is_same>
        struct find_first_of;

        template <typename... Args, typename... CompareArgs, template <typename, typename> typename Predicate>
        struct find_first_of<std::tuple<Args...>, std::tuple<CompareArgs...>, Predicate>
          : ::pastel::utility::tuple::meta::find_first_of_detail::find_first_of<
              0u, 0u, std::tuple<CompareArgs...>, Predicate, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_FIND_FIRST_OF_HPP

