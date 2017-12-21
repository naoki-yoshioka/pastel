#ifndef PASTEL_UTILITY_TUPLE_META_NONE_OF_HPP
# define PASTEL_UTILITY_TUPLE_META_NONE_OF_HPP

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
        template <typename Tuple, template <typename> typename Predicate>
        struct none_of;

        template <typename T, typename... Args, template <typename> typename Predicate>
        struct none_of<std::tuple<T, Args...>, Predicate>
          : std::conditional<
              Predicate<T>::value,
              std::false_type,
              typename ::pastel::utility::tuple::meta::none_of<std::tuple<Args...>, Predicate>::type>::type
        { };

        template <template <typename> typename Predicate>
        struct none_of<std::tuple<>, Predicate>
          : std::true_type
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_NONE_OF_HPP

