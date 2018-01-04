#ifndef PASTEL_FORCE_META_HAS_CUTOFF_HPP
# define PASTEL_FORCE_META_HAS_CUTOFF_HPP

# include <type_traits>


namespace pastel
{
  namespace force
  {
    namespace meta
    {
      template <typename Force>
      struct has_cutoff
        : std::integral_constant<bool, Force::has_cutoff>
      { };
    } // namespace meta
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_META_HAS_CUTOFF_HPP

