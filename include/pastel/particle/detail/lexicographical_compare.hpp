#ifndef PASTEL_PARTICLE_DETAIL_LEXICOGRAPHICAL_COMPARE_HPP
# define PASTEL_PARTICLE_DETAIL_LEXICOGRAPHICAL_COMPARE_HPP

# include <pastel/particle/get.hpp>


namespace pastel
{
  namespace particle
  {
    namespace detail
    {
      template <typename... Tags>
      struct lexicographical_compare;

      template <>
      struct lexicographical_compare<>
      {
        template <std::size_t dimension_, typename Particle>
        static bool call(Particle const& lhs, Particle const& rhs)
        { return false; }
      }; // struct lexicographical_compare<>

      template <typename Tag, typename... Tags>
      struct lexicographical_compare<Tag, Tags...>
      {
        template <std::size_t dimension_, typename Particle>
        static bool call(Particle const& lhs, Particle const& rhs)
        {
          if (::pastel::particle::get<Tag>(lhs) < ::pastel::particle::get<Tag>(rhs))
            return true;
          if (::pastel::particle::get<Tag>(rhs) < ::pastel::particle::get<Tag>(lhs))
            return false;
          return ::pastel::particle::detail::lexicographical_compare<Tags...>::call(lhs, rhs);
        }
      }; // struct lexicographical_compare<Tag, Tags...>
    } // namespace detail
  } // namespace particle
} // namespace pastel


#endif // PASTEL_PARTICLE_DETAIL_LEXICOGRAPHICAL_COMPARE_HPP

