#ifndef PASTEL_PARTICLE_PARTICLE_IO_HPP
# define PASTEL_PARTICLE_PARTICLE_IO_HPP

# include <cstddef>
# include <ostream>

# include <pastel/neighbor/neighbor.hpp>


namespace pastel
{
  namespace neighbor
  {
    /*
    template <typename Character, typename CharacterTraits, bool is_intraparticle_>
    std::basic_ostream<Character, CharacterTraits>& operator<<(
      std::basic_ostream<Character, CharacterTraits>& output_stream,
      ::pastel::neighbor::neighbor<is_intraparticle_> const& neighbor)
    { return output_stream << '<' << neighbor.key() << ',' << neighbor.partner() << '>'; }
    */
    template <typename Character, typename CharacterTraits>
    std::basic_ostream<Character, CharacterTraits>& operator<<(
      std::basic_ostream<Character, CharacterTraits>& output_stream,
      ::pastel::neighbor::neighbor const& neighbor)
    { return output_stream << '<' << neighbor.key() << ',' << neighbor.partner() << '>'; }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_PARTICLE_PARTICLE_IO_HPP

