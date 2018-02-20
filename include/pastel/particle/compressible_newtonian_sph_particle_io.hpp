#ifndef PASTEL_PARTICLE_COMPRESSIBLE_NEWTONIAN_SPH_PARTICLE_IO_HPP
# define PASTEL_PARTICLE_COMPRESSIBLE_NEWTONIAN_SPH_PARTICLE_IO_HPP

# include <cstddef>
# include <ostream>

# include <pastel/particle/compressible_newtonian_sph_particle.hpp>
# include <pastel/particle/tags.hpp>
# include <pastel/particle/get.hpp>


namespace pastel
{
  namespace particle
  {
    template <typename Character, typename CharacterTraits, std::size_t dimension_, typename Value>
    std::basic_ostream<Character, CharacterTraits>& operator<<(
      std::basic_ostream<Character, CharacterTraits>& output_stream,
      ::pastel::particle::compressible_newtonian_sph_particle<dimension_, Value> const& particle)
    {
      return output_stream
        << '{'
        << ::pastel::particle::get< ::pastel::particle::tags::position >(particle) << ','
        << ::pastel::particle::get< ::pastel::particle::tags::velocity >(particle) << ','
        << ::pastel::particle::get< ::pastel::particle::tags::force >(particle) << ','
        << ::pastel::particle::get< ::pastel::particle::tags::mass >(particle) << ','
        << ::pastel::particle::get< ::pastel::particle::tags::density >(particle) << ','
        << ::pastel::particle::get< ::pastel::particle::tags::sph_divergence >(particle) << ','
        << ::pastel::particle::get< ::pastel::particle::tags::pressure >(particle)
        << '}';
    }
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_COMPRESSIBLE_NEWTONIAN_SPH_PARTICLE_IO_HPP

