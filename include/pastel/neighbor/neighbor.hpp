#ifndef PASTEL_NEIGHBOR_NEIGHBOR_HPP
# define PASTEL_NEIGHBOR_NEIGHBOR_HPP

# include <cassert>
# include <cstddef>
# include <utility>
# include <tuple>
//# include <algorithm>


namespace pastel
{
  namespace neighbor
  {
    /*
    namespace neighbor_detail
    {
      template <typename Index>
      inline constexpr Index const& min(Index const& lhs, Index const& rhs)
      { return lhs < rhs ? lhs : rhs; }

      template <typename Index>
      inline constexpr Index const& max(Index const& lhs, Index const& rhs)
      { return lhs > rhs ? lhs : rhs; }


      template <bool is_intraparticle_>
      struct key;

      template <>
      struct key<true>
      {
        template <typename Tuple>
        static constexpr std::size_t call(Tuple const& tuple)
        {
          // std::min is not a constexpr function in C++11
          // since C++14: return static_cast<std::size_t>(std::min(std::get<0u>(neighbor), std::get<1u>(neighbor)));
          return static_cast<std::size_t>(::pastel::neighbor::neighbor_detail::min(std::get<0u>(tuple), std::get<1u>(tuple)));
        }
      }; // struct key<true>

      template <>
      struct key<false>
      {
        template <typename Tuple>
        static constexpr std::size_t call(Tuple const& tuple)
        { return static_cast<std::size_t>(std::get<0u>(tuple)); }
      }; // struct key<false>


      template <bool is_intraparticle_>
      struct partner;

      template <>
      struct partner<true>
      {
        template <typename Tuple>
        static constexpr std::size_t call(Tuple const& tuple)
        {
          // std::max is not a constexpr function in C++11
          // since C++14: return static_cast<std::size_t>(std::max(std::get<0u>(neighbor), std::get<1u>(neighbor)));
          return static_cast<std::size_t>(::pastel::neighbor::neighbor_detail::max(std::get<0u>(tuple), std::get<1u>(tuple)));
        }
      }; // struct partner<true>

      template <>
      struct partner<false>
      {
        template <typename Tuple>
        static constexpr std::size_t call(Tuple const& tuple)
        { return static_cast<std::size_t>(std::get<1u>(tuple)); }
      }; // struct partner<false>


      template <bool is_intraparticle_>
      struct assertion;

      template <>
      struct assertion<true>
      {
        template <typename Tuple>
        static void call(Tuple const& tuple) { assert(std::get<0u>(tuple) != std::get<1u>(tuple)); }
      }; // struct assertion<true>

      template <>
      struct assertion<false>
      {
        template <typename Tuple>
        static constexpr void call(Tuple const& tuple) { }
      }; // struct assertion<false>
    } // namespace neighbor_detail
    */


    //template <bool is_intraparticle_ = true>
    class neighbor
    {
      std::size_t key_;
      std::size_t partner_;

     public:
      //static constexpr bool is_intraparticle = is_intraparticle_;

      constexpr neighbor() : neighbor{0u, 1u} { }

      //constexpr neighbor(std::size_t index1, std::size_t index2)
      //  : neighbor{std::make_tuple(index1, index2)}
      constexpr neighbor(std::size_t key, std::size_t partner)
        : key_{key}, partner_{partner}
      { }

      /*
      template <typename Index>
      explicit constexpr neighbor(std::pair<Index, Index> const& neighbor_pair)
        : key_{::pastel::neighbor::neighbor_detail::key<is_intraparticle>::call(neighbor_pair)},
          partner_{::pastel::neighbor::neighbor_detail::partner<is_intraparticle>::call(neighbor_pair)}
      { ::pastel::neighbor::neighbor_detail::assertion<is_intraparticle>::call(neighbor_pair); }

      template <typename Index>
      explicit constexpr neighbor(std::tuple<Index, Index> const& neighbor_tuple)
        : key_{::pastel::neighbor::neighbor_detail::key<is_intraparticle>::call(neighbor_tuple)},
          partner_{::pastel::neighbor::neighbor_detail::partner<is_intraparticle>::call(neighbor_tuple)}
      { ::pastel::neighbor::neighbor_detail::assertion<is_intraparticle>::call(neighbor_tuple); }
      */

      constexpr std::size_t key() const noexcept { return key_; }
      void key(std::size_t new_key) { key_ = new_key; }
      constexpr std::size_t partner() const noexcept { return partner_; }
      void partner(std::size_t new_partner) { partner_ = new_partner; }

      constexpr bool operator==(neighbor const& other) const
      { return key_ == other.key_ && partner_ == other.partner_; }
      constexpr bool operator<(neighbor const& other) const
      { return key_ < other.key_ || (key_ == other.key_ && partner_ < other.partner_); }
    //}; // class neighbor<is_intraparticle_>
    }; // class neighbor

  /*
    template <bool is_intraparticle_>
    inline constexpr bool operator!=(
      ::pastel::neighbor::neighbor<is_intraparticle_> const& lhs,
      ::pastel::neighbor::neighbor<is_intraparticle_> const& rhs)
    { return !(lhs == rhs); }

    template <bool is_intraparticle_>
    inline constexpr bool operator>(
      ::pastel::neighbor::neighbor<is_intraparticle_> const& lhs,
      ::pastel::neighbor::neighbor<is_intraparticle_> const& rhs)
    { return rhs < lhs; }

    template <bool is_intraparticle_>
    inline constexpr bool operator<=(
      ::pastel::neighbor::neighbor<is_intraparticle_> const& lhs,
      ::pastel::neighbor::neighbor<is_intraparticle_> const& rhs)
    { return !(lhs > rhs); }

    template <bool is_intraparticle_>
    inline constexpr bool operator>=(
      ::pastel::neighbor::neighbor<is_intraparticle_> const& lhs,
      ::pastel::neighbor::neighbor<is_intraparticle_> const& rhs)
    { return !(lhs < rhs); }
    */
    inline constexpr bool operator!=(
      ::pastel::neighbor::neighbor const& lhs,
      ::pastel::neighbor::neighbor const& rhs)
    { return !(lhs == rhs); }

    inline constexpr bool operator>(
      ::pastel::neighbor::neighbor const& lhs,
      ::pastel::neighbor::neighbor const& rhs)
    { return rhs < lhs; }

    inline constexpr bool operator<=(
      ::pastel::neighbor::neighbor const& lhs,
      ::pastel::neighbor::neighbor const& rhs)
    { return !(lhs > rhs); }

    inline constexpr bool operator>=(
      ::pastel::neighbor::neighbor const& lhs,
      ::pastel::neighbor::neighbor const& rhs)
    { return !(lhs < rhs); }
  } // namespace neighbor
} // namespace pastel


#endif // PASTEL_NEIGHBOR_NEIGHBOR_HPP

