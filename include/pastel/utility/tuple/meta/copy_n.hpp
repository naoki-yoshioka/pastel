#ifndef PASTEL_UTILITY_TUPLE_META_COPY_N_HPP
# define PASTEL_UTILITY_TUPLE_META_COPY_N_HPP

# include <tuple>


namespace pastel
{
  namespace utility
  {
    namespace tuple
    {
      namespace meta
      {
        namespace copy_n_detail
        {
          template <std::size_t n, std::size_t from_index, std::size_t to_index, std::size_t to_last, typename FromTuple, typename ToTuple, typename... Args>
          struct copy_n
            : ::pastel::utility::tuple::meta::copy_n_detail::copy_n<n, from_index+1u, to_index+1u, to_last, FromTuple, ToTuple, Args..., typename std::tuple_element<from_index, FromTuple>::type>
          { };

          template <std::size_t n, std::size_t to_index, std::size_t to_last, typename FromTuple, typename ToTuple, typename... Args>
          struct copy_n<n, n, to_index, to_last, FromTuple, ToTuple, Args...>
            : ::pastel::utility::tuple::meta::copy_n_detail::copy_n<n, n, to_index+1u, to_last, FromTuple, ToTuple, Args..., typename std::tuple_element<to_index, ToTuple>::type>
          { };

          template <std::size_t n, std::size_t from_index, std::size_t to_last, typename FromTuple, typename ToTuple, typename... Args>
          struct copy_n<n, from_index, to_last, to_last, FromTuple, ToTuple, Args...>
            : ::pastel::utility::tuple::meta::copy_n_detail::copy_n<n, from_index+1u, to_last, to_last, FromTuple, ToTuple, Args..., typename std::tuple_element<from_index, FromTuple>::type>
          { };

          template <std::size_t n, std::size_t to_last, typename FromTuple, typename ToTuple, typename... Args>
          struct copy_n<n, n, to_last, to_last, FromTuple, ToTuple, Args...>
          { using type = std::tuple<Args...>; };
        } // namespace copy_n_detail


        template <typename FromTuple, std::size_t n, typename ToTuple>
        struct copy_n;

        template <typename... FromArgs, std::size_t n, typename... ToArgs>
        struct copy_n<std::tuple<FromArgs...>, n, std::tuple<ToArgs...>>
          : ::pastel::utility::tuple::meta::copy_n_detail::copy_n<n, 0u, 0u, sizeof...(ToArgs), std::tuple<FromArgs...>, std::tuple<ToArgs...>>
        { };
      } // namespace meta
    } // namespace tuple
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_TUPLE_META_COPY_N_HPP

