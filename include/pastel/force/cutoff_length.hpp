#ifndef PASTEL_FORCE_CUTOFF_LENGTH_HPP
# define PASTEL_FORCE_CUTOFF_LENGTH_HPP

# include <pastel/force/meta/value_of.hpp>


namespace pastel
{
  namespace force
  {
    template <typename Force>
    inline typename ::pastel::force::meta::value_of<Force const>::type const& cutoff_length(Force const& force)
    { return force.cutoff_length(); }

    template <typename Force>
    inline void cutoff_length(Force& force, typename ::pastel::force::meta::value_of<Force>::type const& new_cutoff_length)
    { force.cutoff_length(new_cutoff_length); }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_CUTOFF_LENGTH_HPP

