#ifndef PASTEL_UTILITY_TUPLE_META_REPLACE_HPP
# define PASTEL_UTILITY_TUPLE_META_REPLACE_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace replace_detail
      {
        template <typename Result, typename OldType, typename NewType, typename... Args>
        struct replace;

        template <typename... ResultArgs, typename OldType, typename NewType, typename T, typename... Args>
        struct replace<std::tuple<ResultArgs...>, OldType, NewType, T, Args...>
          : ::pastel::utility::tuple::meta::replace_detail::replace<std::tuple<ResultArgs..., T>, OldType, NewType, Args...>
        { };

        template <typename... ResultArgs, typename OldType, typename NewType, typename... Args>
        struct replace<std::tuple<ResultArgs...>, OldType, NewType, OldType, Args...>
          : ::pastel::utility::tuple::meta::replace_detail::replace<std::tuple<ResultArgs..., NewType>, OldType, NewType, Args...>
        { };

        template <typename Result, typename OldType, typename NewType>
        struct replace<Result, OldType, NewType>
        { using type = Result; };
      } // namespace replace_detail


      template <typename Tuple, typename OldType, typename NewType>
      struct replace;

      template <typename... Args, typename OldType, typename NewType>
      struct replace<std::tuple<Args...>, OldType, NewType>
        : ::pastel::utility::tuple::meta::replace_detail::replace<std::tuple<>, OldType, NewType, Args...>
      { };
    } // namespace utility
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_REPLACE_HPP

