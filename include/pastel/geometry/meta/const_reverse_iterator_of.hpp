#ifndef PASTEL_GEOMETRY_META_CONST_REVERSE_ITERATOR_OF_HPP
# define PASTEL_GEOMETRY_META_CONST_REVERSE_ITERATOR_OF_HPP

# include <iterator>


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct const_reverse_iterator_of
      { using type = typename Quantity::const_reverse_iterator; };

      template <>
      struct const_reverse_iterator_of<float>
      { using type = std::const_reverse_iterator<float const*>; };

      template <>
      struct const_reverse_iterator_of<float const>
      { using type = std::const_reverse_iterator<float const*>; };

      template <>
      struct const_reverse_iterator_of<double>
      { using type = std::const_reverse_iterator<double const*>; };

      template <>
      struct const_reverse_iterator_of<double const>
      { using type = std::const_reverse_iterator<double const*>; };

      template <>
      struct const_reverse_iterator_of<long double>
      { using type = std::const_reverse_iterator<long double const*>; };

      template <>
      struct const_reverse_iterator_of<long double const>
      { using type = std::const_reverse_iterator<long double const*>; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_CONST_REVERSE_ITERATOR_OF_HPP

