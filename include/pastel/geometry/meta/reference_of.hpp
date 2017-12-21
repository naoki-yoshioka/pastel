#ifndef PASTEL_GEOMETRY_META_REFERENCE_OF_HPP
# define PASTEL_GEOMETRY_META_REFERENCE_OF_HPP


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct reference_of
      { using type = typename Quantity::reference; };

      template <typename Quantity>
      struct reference_of<Quantity const>
      { using type = typename Quantity::const_reference; };

      template <>
      struct reference_of<float>
      { using type = float&; };

      template <>
      struct reference_of<float const>
      { using type = float const&; };

      template <>
      struct reference_of<double>
      { using type = double&; };

      template <>
      struct reference_of<double const>
      { using type = double const&; };

      template <>
      struct reference_of<long double>
      { using type = long double&; };

      template <>
      struct reference_of<long double const>
      { using type = long double const&; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_REFERENCE_OF_HPP

