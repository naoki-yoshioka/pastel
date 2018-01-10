#ifndef PASTEL_SYSTEM_INDEX_WITH_ORIGIN_HPP
# define PASTEL_SYSTEM_INDEX_WITH_ORIGIN_HPP

# include <cstddef>


namespace pastel
{
  namespace system
  {
    enum class origin { particles, boundary_particles };

    template <typename Index = std::size_t>
    class index_with_origin
    {
      Index index_;
      ::pastel::system::origin origin_;

     public:
      index_with_origin()
        : index_with_origin{Index{0}, ::pastel::system::origin::particles}
      { }

      index_with_origin(Index index, ::pastel::system::origin origin)
        : index_{index}, origin_{origin}
      { }

      Index index() const { return index_; }
      void index(Index new_index) { index_ = new_index; }

      ::pastel::system::origin origin() const { return origin_; }
      void origin(::pastel::system::origin new_origin) { origin_ = new_origin; }
    }; // class index_with_origin<Index>
  } // namespace system
} // namespace pastel


#endif // PASTEL_SYSTEM_INDEX_WITH_ORIGIN_HPP

