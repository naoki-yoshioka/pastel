#ifndef PASTEL_UTILITY_TUPLE_META_ACCUMULATE_HPP
# define PASTEL_UTILITY_TUPLE_META_ACCUMULATE_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        template <typename Tuple, typename Init, template <typename, typename> typename Function>
        struct accumulate;

        template <typename Type, typename... Args, typename PartialResult, template <typename, typename> typename Function>
        struct accumulate<std::tuple<Type, Args...>, PartialResult, Function>
          : ::pastel::utility::tuple::meta::accumulate<std::tuple<Args...>, typename Function<PartialResult, Type>::type, Function>
        { };

        template <typename Result, template <typename, typename> typename Function>
        struct accumulate<std::tuple<>, Result, Function>
        { using type = Result; };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_ACCUMULATE_HPP

