#ifndef PASTEL_UTILITY_TUPLE_META_COPY_IF_HPP
# define PASTEL_UTILITY_TUPLE_META_COPY_IF_HPP

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
        namespace copy_if_detail
        {
          template <typename Result, typename FromTuple, typename ToTuple, template <typename> typename Predicate>
          struct copy_if;

          template <typename... ResultArgs, typename FromType, typename... FromArgs, typename ToType, typename... ToArgs, template <typename> typename Predicate>
          struct copy_if<std::tuple<ResultArgs...>, std::tuple<FromType, FromArgs...>, std::tuple<ToType, ToArgs...>, Predicate>
            : std::conditional<
                Predicate<FromType>::value,
                typename ::pastel::utility::tuple::meta::copy_if_detail::copy_if<std::tuple<ResultArgs..., FromType>, std::tuple<FromArgs...>, std::tuple<ToArgs...>, Predicate>::type,
                typename ::pastel::utility::tuple::meta::copy_if_detail::copy_if<std::tuple<ResultArgs...>, std::tuple<FromArgs...>, std::tuple<ToType, ToArgs...>, Predicate>::type>::type
          { };

          template <typename... ResultArgs, typename ToType, typename... ToArgs, template <typename> typename Predicate>
          struct copy_if<std::tuple<ResultArgs...>, std::tuple<>, std::tuple<ToType, ToArgs...>, Predicate>
            : ::pastel::utility::tuple::meta::copy_if_detail::copy_if<std::tuple<ResultArgs..., ToType>, std::tuple<>, std::tuple<ToArgs...>, Predicate>
          { };

          template <typename... ResultArgs, typename FromType, typename... FromArgs, template <typename> typename Predicate>
          struct copy_if<std::tuple<ResultArgs...>, std::tuple<FromType, FromArgs...>, std::tuple<>, Predicate>
            : std::conditional<
                Predicate<FromType>::value,
                typename ::pastel::utility::tuple::meta::copy_if_detail::copy_if<std::tuple<ResultArgs..., FromType>, std::tuple<FromArgs...>, std::tuple<>, Predicate>::type,
                typename ::pastel::utility::tuple::meta::copy_if_detail::copy_if<std::tuple<ResultArgs...>, std::tuple<FromArgs...>, std::tuple<>, Predicate>::type>::type
          { };

          template <typename Result, template <typename> typename Predicate>
          struct copy_if<Result, std::tuple<>, std::tuple<>, Predicate>
          { using type = Result; };
        } // namespace copy_if_detail


        template <typename FromTuple, typename ToTuple, template <typename> typename Predicate>
        struct copy_if;

        template <typename... FromArgs, typename... ToArgs, template <typename> typename Predicate>
        struct copy_if<std::tuple<FromArgs...>, std::tuple<ToArgs...>, Predicate>
          : ::pastel::utility::tuple::meta::copy_if_detail::copy_if<std::tuple<>, std::tuple<FromArgs...>, std::tuple<ToArgs...>, Predicate>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_COPY_IF_HPP

