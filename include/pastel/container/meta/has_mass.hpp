#ifndef PASTEL_CONTAINER_META_HAS_MASS_HPP
# define PASTEL_CONTAINER_META_HAS_MASS_HPP

# include <type_traits>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct has_mass
        : std::integral_constant<bool, Particles::has_mass>
      { };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_HAS_MASS_HPP

