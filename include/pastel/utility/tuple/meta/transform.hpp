#ifndef PASTEL_UTILITY_TUPLE_META_TRANSFORM_HPP
# define PASTEL_UTILITY_TUPLE_META_TRANSFORM_HPP

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
        namespace transform_detail
        {
          template <typename Result, template <typename> typename Operation, typename... Args>
          struct transform;

          template <typename... ResultArgs, template <typename> typename Operation, typename Type, typename... Args>
          struct transform<std::tuple<ResultArgs...>, Operation, Type, Args...>
            : ::pastel::utility::tuple::meta::transform_detail::transform<
                std::tuple<ResultArgs..., typename Operation<Type>::type>, Operation, Args...>
          { };

          template <typename Result, template <typename> typename Operation>
          struct transform<Result, Operation>
          { using type = Result; };


          template <typename Result, typename Tuple1, typename Tuple2, template <typename, typename> typename Operation>
          struct transform2;

          template <typename... ResultArgs, typename Type1, typename... Args1, typename Type2, typename... Args2, template <typename, typename> typename Operation>
          struct transform2<std::tuple<ResultArgs...>, std::tuple<Type1, Args1...>, std::tuple<Type2, Args2...>, Operation>
            : ::pastel::utility::tuple::meta::transform_detail::transform2<
                std::tuple<ResultArgs..., typename Operation<Type1, Type2>::type>, std::tuple<Args1...>, std::tuple<Args2...>, Operation>
          { };

          template <typename Result, typename... Args2, template <typename, typename> typename Operation>
          struct transform2<Result, std::tuple<>, std::tuple<Args2...>, Operation>
          { using type = Result; };

          template <typename Result, typename... Args1, template <typename, typename> typename Operation>
          struct transform2<Result, std::tuple<Args1...>, std::tuple<>, Operation>
          { using type = Result; };

          template <typename Result, template <typename, typename> typename Operation>
          struct transform2<Result, std::tuple<>, std::tuple<>, Operation>
          { using type = Result; };
        } // namespace transform_detail


        template <typename Tuple, template <typename> typename Operation>
        struct transform;

        template <typename... Args, template <typename> typename Operation>
        struct transform<std::tuple<Args...>, Operation>
          : ::pastel::utility::tuple::meta::transform_detail::transform<std::tuple<>, Operation, Args...>
        { };


        template <typename Tuple1, typename Type2, template <typename, typename> typename Operation>
        struct transform2;

        template <typename... Args1, typename Args2, template <typename, typename> typename Operation>
        struct transform2<std::tuple<Args1...>, std::tuple<Args2...>, Operation>
          : ::pastel::utility::tuple::meta::transform_detail::transform2<std::tuple<>, std::tuple<Args1...>, std::tuple<Args2...>, Operation>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_TRANSFORM_HPP

