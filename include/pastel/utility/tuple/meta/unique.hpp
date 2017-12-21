#ifndef PASTEL_UTILITY_TUPLE_META_UNIQUE_HPP
# define PASTEL_UTILITY_TUPLE_META_UNIQUE_HPP

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
        namespace unique_detail
        {
          template <typename Result, template <typename, typename> typename Predicate, typename... Args>
          struct unique;

          template <typename... ResultArgs, template <typename, typename> typename Predicate, typename Type1, typename Type2, typename... Args>
          struct unique<std::tuple<ResultArgs...>, Predicate, Type1, Type2, Args...>
            : std::conditional<
                Predicate<Type1, Type2>::value,
                typename ::pastel::utility::tuple::meta::unique_detail::unique<std::tuple<ResultArgs...>, Predicate, Type1, Args...>::type,
                typename ::pastel::utility::tuple::meta::unique_detail::unique<std::tuple<ResultArgs..., Type1>, Predicate, Type2, Args...>::type>::type
          { };

          template <typename... ResultArgs, template <typename, typename> typename Predicate, typename Type>
          struct unique<std::tuple<ResultArgs...>, Predicate, Type>
            : ::pastel::utility::tuple::meta::unique_detail::unique<std::tuple<ResultArgs..., Type>, Predicate>
          { };

          template <typename Result, template <typename, typename> typename Predicate>
          struct unique<Result, Predicate>
          { using type = Result; };
        } // namespace unique_detail


        template <typename Tuple, template <typename, typename> typename Predicate = std::is_same>
        struct unique;

        template <typename... Args, template <typename, typename> typename Predicate>
        struct unique<std::tuple<Args...>, Predicate>
          : ::pastel::utility::tuple::meta::unique_detail<std::tuple<>, Predicate, Args...>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_UNIQUE_HPP

