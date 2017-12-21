#ifndef PASTEL_UTILITY_TUPLE_META_COPY_HPP
# define PASTEL_UTILITY_TUPLE_META_COPY_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace copy_detail
        {
          template <typename Result, typename FromTuple, typename ToTuple>
          struct copy;

          template <typename... ResultArgs, typename FromType, typename... FromArgs, typename ToType, typename...ToArgs>
          struct copy<std::tuple<ResultArgs...>, std::tuple<FromType, FromArgs...>, std::tuple<ToType, ToArgs...>>
            : ::pastel::utility::tuple::meta::copy_detail::copy<std::tuple<ResultArgs..., FromType>, std::tuple<FromArgs...>, std::tuple<ToArgs...>>
          { };

          template <typename... ResultArgs, typename ToType, typename...ToArgs>
          struct copy<std::tuple<ResultArgs...>, std::tuple<>, std::tuple<ToType, ToArgs...>>
            : ::pastel::utility::tuple::meta::copy_detail::copy<std::tuple<ResultArgs..., ToType>, std::tuple<>, std::tuple<ToArgs...>>
          { };

          template <typename... ResultArgs, typename FromType, typename... FromArgs>
          struct copy<std::tuple<ResultArgs...>, std::tuple<FromType, FromArgs...>, std::tuple<>>
            : ::pastel::utility::tuple::meta::copy_detail::copy<std::tuple<ResultArgs..., FromType>, std::tuple<FromArgs...>, std::tuple<>>
          { };

          template <typename Result>
          struct copy<Result, std::tuple<>, std::tuple<>>
          { using type = Result; };
        } // namespace copy_detail


        template <typename FromTuple, typename ToTuple>
        struct copy;

        template <typename... FromArgs, typename... ToArgs>
        struct copy<std::tuple<FromArgs...>, std::tuple<ToArgs...>>
          : ::pastel::utility::tuple::meta::copy_detail::copy<std::tuple<>, std::tuple<FromArgs...>, std::tuple<ToArgs...>>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_COPY_HPP

