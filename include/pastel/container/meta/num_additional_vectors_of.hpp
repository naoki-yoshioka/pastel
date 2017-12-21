#ifndef PASTEL_CONTAINER_META_NUM_ADDITIONAL_VECTORS_OF_HPP
# define PASTEL_CONTAINER_META_NUM_ADDITIONAL_VECTORS_OF_HPP

# include <cstddef>
# include <type_traits>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct num_additional_vectors_of
        : std::integral_constant<std::size_t, Particles::num_additional_vectors>
      { };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_NUM_ADDITIONAL_VECTORS_OF_HPP

