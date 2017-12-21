#ifndef PASTEL_UTILITY_TUPLE_META_MISMATCH_HPP
# define PASTEL_UTILITY_TUPLE_META_MISMATCH_HPP

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
        namespace mismatch_detail
        {
          template <std::size_t result, typename Tuple1, typename Tuple2, template <typename, typename> typename Predicate>
          struct mismatch;

          template <std::size_t result, typename Type1, typename... Args1, typename Type2, typename... Args2, template <typename, typename> typename Predicate>
          struct mismatch<result, std::tuple<Type1, Args1...>, std::tuple<Type2, Args2...>, Predicate>
            : std::conditional<
                Predicate<Type1, Type2>::value,
                typename ::pastel::utility::tuple::meta::mismatch_detail::mismatch<
                  result+1u, std::tuple<Args1...>, std::tuple<Args2...>>::type,
                std::integral_constant<std::size_t, result>>::type
          { };

          template <std::size_t result, typename Type1, typename... Args1, template <typename, typename> typename Predicate>
          struct mismatch<result, std::tuple<Type1, Args1...>, std::tuple<>, Predicate>
            : std::integral_constant<std::size_t, result>
          { };

          template <std::size_t result, typename Type2, typename... Args2, template <typename, typename> typename Predicate>
          struct mismatch<result, std::tuple<>, std::tuple<Type2, Args2...>, Predicate>
            : std::integral_constant<std::size_t, result>
          { };

          template <std::size_t result, template <typename, typename> typename Predicate>
          struct mismatch<result, std::tuple<>, std::tuple<>, Predicate>
            : std::integral_constant<std::size_t, result>
          { };
        } // namespace mismatch_detail


        template <typename Tuple1, typename Tuple2, template <typename, typename> typename Predicate = std::is_same>
        struct mismatch;

        template <typename... Args1, typename... Args2, template <typename, typename> typename Predicate>
        struct mismatch<std::tuple<Args1...>, std::tuple<Args2...>, Predicate>
          : ::pastel::utility::tuple::meta::mismatch_detail::mismatch<0u, std::tuple<Args1...>, std::tuple<Args2...>, Predicate>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_MISMATCH_HPP

