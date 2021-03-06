#ifndef PASTEL_SYSTEM_NEIGHBOR_LIST_BEGIN_HPP
# define PASTEL_SYSTEM_NEIGHBOR_LIST_BEGIN_HPP

# include <cstddef>
# include <iterator>

# include <pastel/system/neighbor_list.hpp>
# include <pastel/system/meta/neighbor_list_of.hpp>
# include <pastel/neighbor/meta/iterator_of.hpp>


namespace pastel
{
  namespace system
  {
    template <std::size_t index, typename System>
    inline typename ::pastel::neighbor::meta::iterator_of<
      typename ::pastel::system::meta::neighbor_list_of<index, System>::type>::type
    neighbor_list_begin(System& system)
    { return std::begin(::pastel::system::neighbor_list<index>(system)); }

    template <std::size_t index, typename System>
    inline typename ::pastel::neighbor::meta::iterator_of<
      typename ::pastel::system::meta::neighbor_list_of<index, System const>::type>::type
    neighbor_list_begin(System const& system)
    { return std::begin(::pastel::system::neighbor_list<index>(system)); }
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_NEIGHBOR_LIST_BEGIN_HPP

