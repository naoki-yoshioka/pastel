#ifndef PASTEL_GEOMETRY_VECTOR_HPP
# define PASTEL_GEOMETRY_VECTOR_HPP

# include <cstddef>
# include <algorithm>
# include <iterator>
# include <type_traits>
# include <utility>

# include <pastel/geometry/evaluate.hpp>
# include <pastel/geometry/detail/alignment.hpp>
# include <pastel/utility/is_nothrow_swappable.hpp>


namespace pastel
{
  namespace geometry
  {
    template <std::size_t dimension_, typename Value = double>
    class alignas(::pastel::geometry::detail::alignment<dimension_, Value>::value) vector final
    {
      static_assert(dimension_ > 0u, "Value of dimension must be greater than 0");
      Value data_[dimension_];

     public:
      static constexpr std::size_t dimension = dimension_;
      using value_type = typename std::remove_const<Value>::type;
      using pointer = value_type*;
      using const_pointer = value_type const*;
      using reference = value_type&;
      using const_reference = value_type const&;
      using iterator = value_type*;
      using const_iterator = value_type const*;
      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

      constexpr vector() noexcept = default;
      ~vector() noexcept = default;
      vector(vector const&) = default;
      vector(vector&) = default;
      vector(vector&&) noexcept = default;
      vector& operator=(vector const&) & = default;
      vector& operator=(vector&) & = default;
      vector& operator=(vector&&) & noexcept = default;

      template <typename... Values>
      constexpr vector(Values&&... values)
        : data_{static_cast<Value>(std::forward<Values>(values))...}
      { static_assert(sizeof...(Values) == dimension, "The number of arguments must be equal to dimension"); }


      void fill(value_type const& value)
      {
        for (auto index = std::size_t{0}; index < dimension; ++index)
          data_[index] = value;
      }

      void swap(vector& other)
        noexcept(::pastel::utility::is_nothrow_swappable<value_type>::value)
      {
        using std::swap;
        swap(data_, other.data_);
      }

      // Iterators
      constexpr iterator begin() noexcept { return data_; }
      constexpr const_iterator begin() const noexcept { return data_; }
      constexpr iterator end() noexcept { return data_ + dimension; }
      constexpr const_iterator end() const noexcept { return data_ + dimension; }
      constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
      constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{end()}; }
      constexpr reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }
      constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator{begin()}; }
      constexpr const_iterator cbegin() const noexcept { return const_iterator{data_}; }
      constexpr const_iterator cend() const noexcept { return const_iterator{data_ + dimension}; }
      constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{cend()}; }
      constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{cbegin()}; }

      // Capacity
      constexpr size_type size() const noexcept { return dimension_; }
      constexpr size_type max_size() const noexcept { return dimension_; }
      constexpr bool empty() const noexcept { return false; }

      // Element access
      template <std::size_t index>
      constexpr reference get() & noexcept
      {
        static_assert(index < dimension, "vector index is within bounds");
        return data_[index];
      }
      template <std::size_t index>
      constexpr const_reference get() const& noexcept
      {
        static_assert(index < dimension, "vector index is within bounds");
        return data_[index];
      }
      template <std::size_t index>
      constexpr value_type get() && noexcept
      {
        static_assert(index < dimension, "vector index is within bounds");
        return std::move(data_[index]);
      }

      constexpr reference operator[](size_type const index) & noexcept
      {
        static_assert(index < dimension, "vector index is within bounds");
        return data_[index];
      }
      constexpr const_reference operator[](size_type const index) const& noexcept
      {
        static_assert(index < dimension, "vector index is within bounds");
        return data_[index];
      }
      constexpr value_type operator[](size_type const index) && noexcept
      {
        static_assert(index < dimension, "vector index is within bounds");
        return std::move(data_[index]);
      }

      constexpr reference at(size_type const index) &
      {
        if (index >= static_cast<size_type>(dimension))
          throw std::out_of_range("vector::at: index >= dimension");
        return data_[index];
      }
      constexpr const_reference at(size_type const index) const&
      {
        if (index >= static_cast<size_type>(dimension))
          throw std::out_of_range("vector::at: index >= dimension");
        return data_[index];
      }
      constexpr value_type at(size_type const index) &&
      {
        if (index >= static_cast<size_type>(dimension))
          throw std::out_of_range("vector::at: index >= dimension");
        return std::move(data_[index]);
      }

      constexpr reference front() & noexcept { return data_[0u]; }
      constexpr const_reference front() const& noexcept { return data_[0u]; }
      constexpr value_type front() && noexcept { return std::move(data_[0u]); }
      constexpr reference back() & noexcept { return data_[dimension-1u]; }
      constexpr const_reference back() const& noexcept { return data_[dimension-1u]; }
      constexpr value_type back() && noexcept { return std::move(data_[dimension-1u]); }

      constexpr pointer data() noexcept { return data_; }
      constexpr const_pointer data() const noexcept { return data_; }


      vector operator+() const { return *this; }
      vector operator-() const
      {
        auto result = vector{};
        for (auto index = std::size_t{0}; index < dimension; ++index)
          result.data_[index] = -data_[index];
        return result;
      }

      vector& operator+=(vector const& other)
      {
        for (auto index = std::size_t{0}; index < dimension; ++index)
          data_[index] += other.data_[index];
        return *this;
      }

      vector& operator-=(vector const& other)
      {
        for (auto index = std::size_t{0}; index < dimension; ++index)
          data_[index] -= other.data_[index];
        return *this;
      }

      vector& operator*=(value_type const& scalar)
      {
        for (auto index = std::size_t{0}; index < dimension; ++index)
          data_[index] *= scalar;
        return *this;
      }

      vector& operator/=(value_type const& scalar)
      {
        for (auto index = std::size_t{0}; index < dimension; ++index)
          data_[index] /= scalar;
        return *this;
      }
    }; // class vector<dimension_, Value>


    // Vector comparisons
    template <std::size_t dimension_, typename Value>
    inline bool operator==(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    { return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs)); }

    template <std::size_t dimension_, typename Value>
    inline bool operator!=(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    { return !(lhs == rhs); }

    template <std::size_t dimension_, typename Value>
    inline bool operator<(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    { return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs)); }

    template <std::size_t dimension_, typename Value>
    inline bool operator>(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    { return rhs < lhs; }

    template <std::size_t dimension_, typename Value>
    inline bool operator<=(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    { return !(lhs > rhs); }

    template <std::size_t dimension_, typename Value>
    inline bool operator>=(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    { return !(lhs < rhs); }

    // Vector arithmetics
    template <std::size_t dimension_, typename Value>
    inline ::pastel::geometry::vector<dimension_, Value> operator+(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    { ::pastel::geometry::vector<dimension_, Value> result = lhs; return result += rhs; }

    template <std::size_t dimension_, typename Value>
    inline ::pastel::geometry::vector<dimension_, Value> operator-(
      ::pastel::geometry::vector<dimension_, Value> const& lhs,
      ::pastel::geometry::vector<dimension_, Value> const& rhs)
    { ::pastel::geometry::vector<dimension_, Value> result = lhs; return result -= rhs; }

    template <std::size_t dimension, typename Value, std::size_t dimension_, typename Value_>
    inline ::pastel::geometry::vector<dimension, Value> operator*(
      ::pastel::geometry::vector<dimension, Value> const&,
      ::pastel::geometry::vector<dimension_, Value_> const&)
    = delete;

    template <std::size_t dimension_, typename Value>
    inline ::pastel::geometry::vector<dimension_, Value> operator*(
      ::pastel::geometry::vector<dimension_, Value> const& vector, Value const& scalar)
    { ::pastel::geometry::vector<dimension_, Value> result = vector; return result *= scalar; }

    template <std::size_t dimension_, typename Value>
    inline ::pastel::geometry::vector<dimension_, Value> operator*(
      Value const& scalar, ::pastel::geometry::vector<dimension_, Value> const& vector)
    { ::pastel::geometry::vector<dimension_, Value> result = vector; return result *= scalar; }

    template <typename T, std::size_t dimension_, typename Value>
    inline ::pastel::geometry::vector<dimension_, Value> operator/(
      T const&, ::pastel::geometry::vector<dimension_, Value> const&)
    = delete;

    template <std::size_t dimension_, typename Value>
    inline ::pastel::geometry::vector<dimension_, Value> operator/(
      ::pastel::geometry::vector<dimension_, Value> const& vector, Value const& scalar)
    { ::pastel::geometry::vector<dimension_, Value> result = vector; return result /= scalar; }

    // Other free functions
    template <std::size_t dimension_, typename Value>
    inline void swap(
      ::pastel::geometry::vector<dimension_, Value>& lhs,
      ::pastel::geometry::vector<dimension_, Value>& rhs)
    noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }

    template <std::size_t dimension_, typename Value>
    inline ::pastel::geometry::vector<dimension_, Value> const& evaluate(
      ::pastel::geometry::vector<dimension_, Value> const& vector)
    { return vector; }
  } // namespace geometry
} // namespace pastel


#endif // PASTEL_GEOMETRY_VECTOR_HPP

