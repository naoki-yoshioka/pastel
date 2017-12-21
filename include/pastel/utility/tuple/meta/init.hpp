#ifndef PASTEL_UTILITY_TUPLE_META_INIT_HPP
# define PASTEL_UTILITY_TUPLE_META_INIT_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace init_detail
      {
        template <typename Result, typename... Args>
        struct init;

        template <typename... ResultArgs, typename Type, typename... Args>
        struct init<std::tuple<ResultArgs...>, Type, Args...>
          : ::pastel::utility::tuple::init_detail::init<std::tuple<ResultArgs..., Type>, Args...>
        { };

        template <typename Result, typename Type>
        struct init<Result, Type>
        { using type = Result; };
      } // namespace init_detail

      template <typename Tuple>
      struct init;

      template <typename... Args>
      struct init<std::tuple<Args...>>
        : ::pastel::utility::tuple::init_detail::init<std::tuple<>, Args...>
      { };
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_INIT_HPP

