#ifndef PASTEL_EXTFORCE_NO_FORCE_HPP
# define PASTEL_EXTFORCE_NO_FORCE_HPP

# include <pastel/container/meta/size_of.hpp>
# include <pastel/container/meta/vector_of.hpp>


namespace pastel
{
  namespace extforce
  {
    struct no_force
    {
      static constexpr bool is_sph_external_force = false;

      template <typename Particles>
      typename ::pastel::container::meta::vector_of<Particles const>::type operator()(
        Particles const&,
        typename ::pastel::container::meta::size_of<Particles const>::type) const
      { return {}; }
    }; // struct no_force
  } // namespacd extforce
} // namespace pastel


#endif // PASTEL_EXTFORCE_NO_FORCE_HPP

