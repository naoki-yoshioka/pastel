#ifndef PASTEL_EXTFORCE_SPH_NO_FORCE_HPP
# define PASTEL_EXTFORCE_SPH_NO_FORCE_HPP

# include <pastel/container/meta/size_of.hpp>
# include <pastel/container/meta/vector_of.hpp>


namespace pastel
{
  namespace extforce
  {
    template <typename Pressure>
    class sph_no_force
    {
      Pressure pressure_;

     public:
      static constexpr bool is_sph_external_force = true;

      explicit sph_no_force(Pressure const& pressure) : pressrure_{pressure} { }

      Pressure const& pressure() const { return pressure_; }

      template <typename Particles>
      typename ::pastel::container::meta::vector_of<Particles const>::type operator()(
        Particles const&,
        typename ::pastel::container::meta::size_of<Particles const>::type) const
      { return {}; }
    }; // struct sph_no_force<Pressure>
  } // namespacd extforce
} // namespace pastel


#endif // PASTEL_EXTFORCE_SPH_NO_FORCE_HPP

