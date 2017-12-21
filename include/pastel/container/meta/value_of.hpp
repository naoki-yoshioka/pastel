#ifndef PASTEL_CONTAINER_META_VALUE_OF_HPP
# define PASTEL_CONTAINER_META_VALUE_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct value_of
      { using type = typename Particles::value_type; };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_VALUE_OF_HPP

