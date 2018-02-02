#ifndef PASTEL_INTEGRATE_DETAIL_DERIVATIVE_COEFFICIENTS_HPP
# define PASTEL_INTEGRATE_DETAIL_DERIVATIVE_COEFFICIENTS_HPP

# include <cstddef>


namespace pastel
{
  namespace integrate
  {
    namespace detail
    {
      template <std::size_t max_order, typename Time>
      class derivative_coefficients
      {
        Time data_[max_order+1u];

       public:
        explicit derivative_coefficients(Time time_step)
          : data_{}
        {
          data_[0u] = Time{1};
          for (auto index = std::size_t{1}; index <= max_order; ++index)
            data_[index] = data_[index-1u] * time_step / static_cast<Time>(index);
        }

        Time& operator[](std::size_t index) { return data_[index]; }
        Time const& operator[](std::size_t index) const { return data_[index]; }
      }; // struct derivative_coefficients<order, Time>
    } // namespace detail
  } // namespace integrate
} // namespace pastel


#endif // PASTEL_INTEGRATE_DETAIL_DERIVATIVE_COEFFICIENTS_HPP

