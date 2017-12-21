#ifndef PASTEL_UTILITY_TUPLE_META_COPY_BACKWARD_HPP
# define PASTEL_UTILITY_TUPLE_META_COPY_BACKWARD_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace copy_backward_detail
        {
          template <typename Result, std::size_t from_index, typename FromTuple, std::size_t to_index, typename ToTuple>
          struct copy_backward;

          template <typename... ResultArgs, std::size_t from_index, typename FromTuple, std::size_t to_index, typename ToTuple>
          struct copy_backward<std::tuple<ResultArgs...>, from_index, FromTuple, to_index, ToTuple>
            : ::pastel::utilit::tuple::meta::copy_backward_detail::copy_backward<
                std::tuple<typename std::tuple_element<FromTuple, from_index-1u>::type, ResultArgs...>,
                from_index-1u, FromTuple, to_index-1u, ToTuple>
          { };

          template <typename... ResultArgs, typename FromTuple, std::size_t to_index, typename ToTuple>
          struct copy_backward<std::tuple<ResultArgs...>, 0u, FromTuple, to_index, ToTuple>
            : ::pastel::utilit::tuple::meta::copy_backward_detail::copy_backward<
                std::tuple<typename std::tuple_element<ToTuple, to_index-1u>::type, ResultArgs...>,
                0u, FromTuple, to_index-1u, ToTuple>
          { };

          template <typename... ResultArgs, std::size_t from_index, typename FromTuple, typename ToTuple>
          struct copy_backward<std::tuple<ResultArgs...>, from_index, FromTuple, 0u, ToTuple>
            : ::pastel::utilit::tuple::meta::copy_backward_detail::copy_backward<
                std::tuple<typename std::tuple_element<FromTuple, from_index-1u>::type, ResultArgs...>,
                from_index-1u, FromTuple, 0u, ToTuple>
          { };

          template <typename Result, typename FromTuple, typename ToTuple>
          struct copy_backward<Result, 0u, FromTuple, 0u, ToTuple>
          { using type = Result; };
        } // namespace copy_backward_detail


        template <typename FromTuple, typename ToTuple>
        struct copy_backward;

        template <typename... FromArgs, typename... ToArgs>
        struct copy_backward<std::tuple<FromArgs...>, std::tuple<ToArgs...>>
          : ::pastel::utilit::tuple::meta::copy_backward_detail::copy_backward<std::tuple<>, sizeof...(FromArgs), std::tuple<FromArgs...>, sizeof...(ToArgs), std::tuple<ToArgs...>>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_COPY_BACKWARD_HPP

