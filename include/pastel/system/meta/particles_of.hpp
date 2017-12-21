#ifndef PASTEL_SYSTEM_META_PARTICLES_OF_HPP
# define PASTEL_SYSTEM_META_PARTICLES_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <std::size_t index, typename System>
      struct particles_of
      { using type = typename System::template particles_type<index>; };

      template <std::size_t index, typename System>
      struct particles_of<index, System const>
      { using type = typename System::template particles_type<index> const; };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_PARTICLES_OF_HPP

