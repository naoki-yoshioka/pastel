#ifndef PASTEL_GEOMETRY_EVALUATE_HPP
# define PASTEL_GEOMETRY_EVALUATE_HPP


namespace pastel
{
  namespace geometry
  {
    template <typename Expression>
    inline auto evaluate(Expression const& expression) -> decltype(expression.evaluate())
    { return expression.evaluate(); }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_EVALUATE_HPP

