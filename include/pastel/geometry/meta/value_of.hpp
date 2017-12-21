#ifndef PASTEL_GEOMETRY_META_VALUE_OF_HPP
# define PASTEL_GEOMETRY_META_VALUE_OF_HPP


namespace pastel
{
  namespace geometry
  {
    namespace meta
    {
      template <typename Quantity>
      struct value_of
      { using type = typename Quantity::value_type; };

      template <>
      struct value_of<float>
      { using type = float; };

      template <>
      struct value_of<float const>
      { using type = float; };

      template <>
      struct value_of<double>
      { using type = double; };

      template <>
      struct value_of<double const>
      { using type = double; };

      template <>
      struct value_of<long double>
      { using type = long double; };

      template <>
      struct value_of<long double const>
      { using type = long double; };
    } // namespace meta
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_META_VALUE_OF_HPP

