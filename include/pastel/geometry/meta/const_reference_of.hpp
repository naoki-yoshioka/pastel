#ifndef PASTEL_GEOMETRY_META_CONST_REFERENCE_OF_HPP
# define PASTEL_GEOMETRY_META_CONST_REFERENCE_OF_HPP


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct const_reference_of
      { using type = typename Quantity::const_reference; };

      template <>
      struct const_reference_of<float>
      { using type = float const&; };

      template <>
      struct const_reference_of<float const>
      { using type = float const&; };

      template <>
      struct const_reference_of<double>
      { using type = double const&; };

      template <>
      struct const_reference_of<double const>
      { using type = double const&; };

      template <>
      struct const_reference_of<long double>
      { using type = long double const&; };

      template <>
      struct const_reference_of<long double const>
      { using type = long double const&; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_CONST_REFERENCE_OF_HPP

