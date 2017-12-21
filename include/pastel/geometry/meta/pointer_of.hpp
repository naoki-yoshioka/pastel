#ifndef PASTEL_GEOMETRY_META_POINTER_OF_HPP
# define PASTEL_GEOMETRY_META_POINTER_OF_HPP


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct pointer_of
      { using type = typename Quantity::pointer; };

      template <typename Quantity>
      struct pointer_of<Quantity const>
      { using type = typename Quantity::const_pointer; };

      template <>
      struct pointer_of<float>
      { using type = float*; };

      template <>
      struct pointer_of<float const>
      { using type = float const*; };

      template <>
      struct pointer_of<double>
      { using type = double*; };

      template <>
      struct pointer_of<double const>
      { using type = double const*; };

      template <>
      struct pointer_of<long double>
      { using type = long double*; };

      template <>
      struct pointer_of<long double const>
      { using type = long double const*; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_POINTER_OF_HPP

