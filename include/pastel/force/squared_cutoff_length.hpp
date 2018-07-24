#ifndef PASTEL_FORCE_SQUARED_CUTOFF_LENGTH_HPP
# define PASTEL_FORCE_SQUARED_CUTOFF_LENGTH_HPP

# include <pastel/force/cutoff_length.hpp>
# include <pastel/force/meta/value_of.hpp>


namespace pastel
{
  namespace force
  {
    namespace dispatch
    {
      template <typename Force>
      struct squared_cutoff_length
      {
        static typename ::pastel::force::meta::value_of<Force const>::type call(Force const& force)
        { return ::pastel::force::cutoff_length(force) * ::pastel::force::cutoff_length(force); }
      }; // struct squared_cutoff_length<Force>
    } // namespace dispatch

    template <typename Force>
    inline auto squared_cutoff_length(Force const& force)
      -> decltype(::pastel::force::dispatch::squared_cutoff_length<Force>::call(force))
    { return ::pastel::force::dispatch::squared_cutoff_length<Force>::call(force); }
  } // namespace force
} // namespace pastel


#endif // PASTEL_FORCE_CUTOFF_LENGTH_HPP

