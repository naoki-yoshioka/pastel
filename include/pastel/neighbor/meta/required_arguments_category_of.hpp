#ifndef PASTEL_NEIGHBOR_META_REQUIRED_ARGUMENTS_CATEGORY_OF_HPP
# define PASTEL_NEIGHBOR_META_REQUIRED_ARGUMENTS_CATEGORY_OF_HPP

# include <pastel/neighbor/meta/force_of.hpp>
# include <pastel/force/meta/required_arguments_category_of.hpp>


namespace pastel
{
  namespace neighbor
  {
    namespace meta
    {
      template <typename NeighborList>
      struct required_arguments_category_of
        : ::pastel::force::meta::required_arguments_category_of<
            typename ::pastel::neighbor::meta::force_of<NeighborList>::type>
      { };
    } // namespace meta
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_META_REQUIRED_ARGUMENTS_CATEGORY_OF_HPP

