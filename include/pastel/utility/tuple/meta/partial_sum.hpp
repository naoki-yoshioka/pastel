#ifndef PASTEL_UTILITY_TUPLE_META_PARTIAL_SUM_HPP
# define PASTEL_UTILITY_TUPLE_META_PARTIAL_SUM_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace partial_sum_detail
        {
          template <typename Result, typename LastResultArg, template <typename, typename> typename Function, typename... Args>
          struct partial_sum;

          template <typename... ResultArgs, typename LastResultArg, template <typename, typename> typename Function, typename Type, typename... Args>
          struct partial_sum<std::tuple<ResultArgs...>, LastResultArg, Function, Type, Args...>
            : ::pastel::utility::tuple::meta::partial_sum_detail::partial_sum<
                std::tuple<ResultArgs..., typename Function<LastResultArg, Type>::type>,
                typename Function<LastResultArg, Type>::type, Function, Args...>
          { };

          template <typename Result, typename LastResultArg, template <typename, typename> typename Function>
          struct partial_sum<Result, LastResultArg, Function>
          { using type = Result; };
        } // namespace partial_sum_detail


        template <typename Tuple, template <typename, typename> typename Function>
        struct partial_sum;

        template <typename Type, typename... Args, template <typename, typename> typename Function>
        struct partial_sum<std::tuple<Type, Args...>, Function>
          : ::pastel::utility::tuple::meta::partial_sum_detail::partial_sum<std::tuple<Type>, Type, Function, Args...>
        { };

        template <template <typename, typename> typename Function>
        struct partial_sum<std::tuple<>, Function>
        { using type = void; };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_PARTIAL_SUM_HPP

