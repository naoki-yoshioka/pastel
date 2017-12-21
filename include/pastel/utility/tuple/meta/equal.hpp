#ifndef PASTEL_UTILITY_TUPLE_META_EQUAL_HPP
# define PASTEL_UTILITY_TUPLE_META_EQUAL_HPP

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
        template <typename Tuple1, typename Tuple2, template <typename, typename> typename Predicate = std::is_same>
        struct equal;

        template <typename Type1, typename... Args1, typename Type2, typename... Args2, template <typename, typename> typename Predicate>
        struct equal<std::tuple<Type1, Args1...>, std::tuple<Type2, Args2...>, Predicate>
          : std::conditional<
              Predicate<Type1, Type2>::value,
              typename ::pastel::utility::tuple::meta::equal<std::tuple<Args1...>, std::tuple<Args2...>, Predicate>::type,
              std::false_type>::type
        { };

        template <typename Type, typename... Args, template <typename, typename> typename Predicate>
        struct equal<std::tuple<>, std::tuple<Type, Args...>, Predicate>
          : std::false_type
        { };

        template <typename Type, typename... Args, template <typename, typename> typename Predicate>
        struct equal<std::tuple<Type, Args...>, std::tuple<>, Predicate>
          : std::false_type
        { };

        template <template <typename, typename> typename Predicate>
        struct equal<std::tuple<>, std::tuple<>, Predicate>
          : std::true_type
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_EQUAL_HPP

