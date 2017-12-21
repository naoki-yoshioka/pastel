#ifndef PASTEL_UTILITY_IS_PAIR_HPP
# define PASTEL_UTILITY_IS_PAIR_HPP

# include <type_traits>

# include <pastel/utility/pair.hpp>


namespace pastel
{
  namespace utility
  {
    template <typename T>
    struct is_pair
      : std::false_type
    { };

    template <typename T, T n, T m>
    struct is_pair< ::pastel::utility::pair<T, n, m> >
      : std::true_type
    { };
  } // namespace utility
} // namespace pastel


#endif // PASTEL_UTILITY_IS_PAIR_HPP

