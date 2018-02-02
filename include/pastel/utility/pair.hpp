#ifndef PASTEL_UTILITY_PAIR_HPP
# define PASTEL_UTILITY_PAIR_HPP

# include <cstddef>


namespace pastel
{
  namespace utility
  {
    template <typename T, T n, T m>
    struct pair
    {
      using type = T;
      static constexpr T first = n;
      static constexpr T second = m;
    }; // struct pair<T, n, m>

    template <int n, int m>
    using int_pair = ::pastel::utility::pair<int, n, m>;

    template <std::size_t n, std::size_t m>
    using size_pair = ::pastel::utility::pair<std::size_t, n, m>;
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_PAIR_HPP

