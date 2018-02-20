#ifndef PASTEL_EXTFORCE_CONSTANT_FORCE_HPP
# define PASTEL_EXTFORCE_CONSTANT_FORCE_HPP

# include <utility>

# include <pastel/container/meta/size_of.hpp>


namespace pastel
{
  namespace extforce
  {
    template <typename Vector>
    struct constant_force
    {
     private:
      Vector force_;

     public:
      static constexpr bool is_sph_external_force = false;

      constexpr constant_force() noexcept = default;
      explicit constexpr constant_force(Vector const& force) noexcept
        : force_{force}
      { }

      Vector const& force() const { return force_; }
      template <typename Vector_>
      void force(Vector_&& new_force)
      { force_ = std::forward<Vector_>(new_force); }

      template <typename Particles>
      Vector const& operator()(
        Particles const&,
        typename ::pastel::container::meta::size_of<Particles const>::type) const
      { return force_; }
    }; // struct constant_force
  } // namespacd extforce
} // namespace pastel


#endif // PASTEL_EXTFORCE_CONSTANT_FORCE_HPP

