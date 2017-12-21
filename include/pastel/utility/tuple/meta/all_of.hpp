#ifndef PASTEL_UTILITY_TUPLE_METEA_ALL_OF_HPP
# define PASTEL_UTILITY_TUPLE_METEA_ALL_OF_HPP

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
        struct all_of;

        template <typename T, typename... Args, template <typename> typename Predicate>
        struct all_of<std::tuple<T, Args...>, Predicate>
          : std::conditional<
              Predicate<T>::value,
              typename ::pastel::utility::tuple::meta::all_of<std::tuple<Args...>, Predicate>::type,
              std::false_type>::type
        { };

        template <template <typename> typename Predicate>
        struct all_of<std::tuple<>, Predicate>
          : std::true_type
        { };
      }
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_METEA_ALL_OF_HPP

