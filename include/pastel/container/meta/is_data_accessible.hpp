#ifndef PASTEL_CONTAINER_META_IS_DATA_ACCESSIBLE_HPP
# define PASTEL_CONTAINER_META_IS_DATA_ACCESSIBLE_HPP

# include <type_traits>


namespace pastel
{
  namespace container
  {
    namespace meta
    {
      template <typename Particles>
      struct is_data_accessible
        : std::integral_constant<bool, Particles::is_data_accessible>
      { };
    } // namespace meta
  } // namespace container
} // namespace pastel


#endif // PASTEL_CONTAINER_META_IS_DATA_ACCESSIBLE_HPP

