#ifndef PASTEL_PARTICLE_TAGS_HPP
# define PASTEL_PARTICLE_TAGS_HPP


namespace pastel
{
  namespace particle
  {
    namespace tags
    {
      struct position { };
      struct velocity { };
      struct force { };
      struct mass { };
      struct diameter { };

      // for SPH
      struct density { };
      struct sph_divergences { };
      struct pressure { };
    }
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_TAGS_HPP

