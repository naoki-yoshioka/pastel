#ifndef PASTEL_SYSTEM_META_NEIGHBOR_OF_HPP
# define PASTEL_SYSTEM_META_NEIGHBOR_OF_HPP

# include <pastel/system/meta/neighbor_list_of.hpp>
# include <pastel/neighbor_list/meta/value_of.hpp>


namespace pastel
{
  namespace system
  {
    namespace meta
    {
      template <std::size_t index, typename System>
      struct neighbor_of
        : ::pastel::container::meta::value_of<typename ::pastel::system::meta::neighbor_list_of<index, System>::type>
      { };
    } // namespace meta
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_META_NEIGHBOR_OF_HPP

