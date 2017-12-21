#ifndef PASTEL_UTILITY_TUPLE_META_ROTATE_HPP
# define PASTEL_UTILITY_TUPLE_META_ROTATE_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace rotate_detail
        {
          template <typename Result, typename Tuple, std::size_t index, std::size_t last>
          struct rotate;

          template <typename... ResultArgs, typename Tuple, std::size_t index, std::size_t last>
          struct rotate<std::tuple<ResultArgs...>, Tuple, index, last>
            : ::pastel::utility::tuple::meta::rotate_detail::rotate<
                std::tuple<ResultArgs..., typename std::tuple_element<Tuple, index>::type>,
                Tuple, index+1u, last>
          { };

          template <typename... ResultArgs, typename Tuple, std::size_t last>
          struct rotate<std::tuple<ResultArgs...>, Tuple, std::tuple_size<Tuple>::value, last>
            : ::pastel::utility::tuple::meta::rotate_detail::rotate<std::tuple<ResultArgs...>, Tuple, 0u, last>
          { };

          template <typename Result, typename Tuple, std::size_t last>
          struct rotate<Result, Tuple, last, last>
          { using type = Result; };
        } // namespace rotate_detail


        template <typename Tuple, std::size_t index>
        struct rotate;

        template <typename... Args, std::size_t index>
        struct rotate
          : ::pastel::utility::tuple::meta::rotate_detail::rotate<
              std::tuple<typename std::tuple_element<std::tuple<Args...>, index>::type,
              std::tuple<Args...>, index+1u, index>
        { };

        template <>
        struct rotate<std::tuple<>, 0u>
        { using type = std::tuple<>; };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_ROTATE_HPP

