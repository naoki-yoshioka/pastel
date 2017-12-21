#ifndef PASTEL_PARTICLE_GET_HPP
# define PASTEL_PARTICLE_GET_HPP

# include <utility>


namespace pastel
{
  namespace particle
  {
    template <typename Tag, typename Particle>
    inline auto get(Particle&& particle) -> decltype(std::forward<Particle>(particle).template get<Tag>())
    { return std::forward<Particle>(particle).template get<Tag>(); }
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_GET_HPP

