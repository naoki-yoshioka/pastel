#ifndef PASTEL_SYSTEM_META_PARTICLE_OF_HPP
# define PASTEL_SYSTEM_META_PARTICLE_OF_HPP

# include <pastel/system/meta/particles_of.hpp>
# include <pastel/container/meta/value_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <std::size_t index, typename System>
      struct particle_of
        : ::pastel::container::meta::value_of<typename ::pastel::system::meta::particles_of<index, System>::type>
      { };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_PARTICLE_OF_HPP

