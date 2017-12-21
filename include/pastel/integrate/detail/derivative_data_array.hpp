#ifndef PASTEL_INTEGRATE_DETAIL_DERIVATIVE_DATA_ARRAY_HPP
# define PASTEL_INTEGRATE_DETAIL_DERIVATIVE_DATA_ARRAY_HPP

# include <cstddef>

# include <pastel/container/tags.hpp>
# include <pastel/container/meta/vector_of.hpp>


namespace pastel
{
  namespace integrate
  {
    namespace detail
    {
      namespace derivative_data_array_detail
      {
        template <std::size_t index, std::size_t last, typename AdditionalVectorIndexTuple>
        struct initialize
        {
          static constexpr auto additional_vector_index
            = std::tuple_element<index, AdditionalVectorIndexTuple>::type::value;

          template <typename Particles, typename Data>
          static void call(Particles& particles, Data* data)
          {
            data[index] = particles.template data< ::pastel::container::tags::nth_additional_vector<additional_vector_index> >();
            ::pastel::integrate::detail::derivative_data_array_detail::initialize<index+1u, last, AdditionalVectorIndexTuple>::call(particles, data);
          }

          template <typename Particles, typename Data>
          static void call(Particles const& particles, Data* data)
          {
            data[index] = particles.template data< ::pastel::container::tags::nth_additional_vector<additional_vector_index> >();
            ::pastel::integrate::detail::derivative_data_array_detail::initialize<index+1u, last, AdditionalVectorIndexTuple>::call(particles, data);
          }
        }; // struct initialize<index, last, AdditionalVectorIndexTuple>

        template <std::size_t last, typename AdditionalVectorIndexTuple>
        struct initialize<last, last, AdditionalVectorIndexTuple>
        {
          template <typename Particles, typename Data>
          static void call(Particles&, Data*) { }

          template <typename Particles, typename Data>
          static void call(Particles const&, Data*) { }
        }; // struct initialize<last, last, AdditionalVectorIndexTuple>
      } // namespace derivative_data_array_detail

      template <std::size_t num_additional_vectors, typename AdditionalVectorIndexTuple, typename Particles>
      class derivative_data_array
      {
        using vector_type = typename ::pastel::container::meta::vector_of<Particles>::type;
        vector_type* data_[num_additional_vectors];

       public:
        explicit derivative_data_array(Particles& particles)
          : data_{}
        { ::pastel::integrate::detail::derivative_data_array_detail::initialize<0u, num_additional_vectors, AdditionalVectorIndexTuple>::call(particles, data_); }

        vector_type& operator[](std::size_t index) { return data_[index]; }
        vector_type const& operator[](std::size_t index) const { return data_[index]; }
      }; // struct derivative_data_array<num_additional_vectors, AdditionalVectorIndexTuple, Particles>
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_DERIVATIVE_DATA_ARRAY_HPP

