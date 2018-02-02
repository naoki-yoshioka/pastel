#ifndef PASTEL_CONTAINER_META_NUM_PROPERTY_SCALARS_OF_HPP
# define PASTEL_CONTAINER_META_NUM_PROPERTY_SCALARS_OF_HPP

# include <cstddef>
# include <type_traits>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct num_property_scalars_of
        : std::integral_constant<std::size_t, Particles::num_property_scalars>
      { };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_NUM_PROPERTY_SCALARS_OF_HPP

