#ifndef PASTEL_GEOMETRY_META_ITERATOR_OF_HPP
# define PASTEL_GEOMETRY_META_ITERATOR_OF_HPP


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct iterator_of
      { using type = typename Quantity::iterator; };

      template <typename Quantity>
      struct iterator_of<Quantity const>
      { using type = typename Quantity::const_iterator; };

      template <>
      struct iterator_of<float>
      { using type = float*; };

      template <>
      struct iterator_of<float const>
      { using type = float const*; };

      template <>
      struct iterator_of<double>
      { using type = double*; };

      template <>
      struct iterator_of<double const>
      { using type = double const*; };

      template <>
      struct iterator_of<long double>
      { using type = long double*; };

      template <>
      struct iterator_of<long double const>
      { using type = long double const*; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_ITERATOR_OF_HPP

