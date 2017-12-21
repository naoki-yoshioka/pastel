#ifndef PASTEL_FORCE_META_HAS_POTENTIAL_ENERGY
# define PASTEL_FORCE_META_HAS_POTENTIAL_ENERGY

# include <type_traits>


namespace pastel
{
  namespace force
  {
    namespace meta
    {
      template <typename Force>
      struct has_potential_energy
        : std::integral_constant<bool, Force::has_potential_energy>
      { };
    } // namespace meta
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_META_HAS_POTENTIAL_ENERGY

