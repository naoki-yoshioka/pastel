#ifndef PASTEL_NEIGHBOR_META_BOUNDARY_UPDATER_OF_HPP
# define PASTEL_NEIGHBOR_META_BOUNDARY_UPDATER_OF_HPP

# include <cstddef>


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename Updater>
      struct boundary_updater_of
      { using type = typename Updater::boundary_updater_type; };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_BOUNDARY_UPDATER_OF_HPP

