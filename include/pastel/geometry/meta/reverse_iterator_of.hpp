#ifndef PASTEL_GEOMETRY_META_REVERSE_ITERATOR_OF_HPP
# define PASTEL_GEOMETRY_META_REVERSE_ITERATOR_OF_HPP

# include <iterator>


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct reverse_iterator_of
      { using type = typename Quantity::reverse_iterator; };

      template <typename Quantity>
      struct reverse_iterator_of<Quantity const>
      { using type = typename Quantity::const_reverse_iterator; };

      template <>
      struct reverse_iterator_of<float>
      { using type = std::reverse_iterator<float*>; };

      template <>
      struct reverse_iterator_of<float const>
      { using type = std::reverse_iterator<float const*>; };

      template <>
      struct reverse_iterator_of<double>
      { using type = std::reverse_iterator<double*>; };

      template <>
      struct reverse_iterator_of<double const>
      { using type = std::reverse_iterator<double const*>; };

      template <>
      struct reverse_iterator_of<long double>
      { using type = std::reverse_iterator<long double*>; };

      template <>
      struct reverse_iterator_of<long double const>
      { using type = std::reverse_iterator<long double const*>; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_REVERSE_ITERATOR_OF_HPP

