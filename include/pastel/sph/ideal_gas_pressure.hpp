#ifndef PASTEL_SPH_IDEAL_GAS_PRESSURE_HPP
# define PASTEL_SPH_IDEAL_GAS_PRESSURE_HPP


namespace pastel
{
  namespace sph
  {
    template <typename Value = double>
    class ideal_gas_pressure
    {
      Value coefficient_;

     public:
      explicit constexpr ideal_gas_pressure(Value const& coefficient) : coefficient_{coefficient} { }

      constexpr Value const& coefficient() const { return coefficient_; }
      void coefficient(Value const& new_coefficient)
      { coefficient_ = new_coefficient; }

      template <typename Density>
      Value operator()(Density const& density) const
      { return coefficient_ * density; }
    }; // class ideal_gas_pressure<Value>
  } // namespace sph
} // namespace pastel


#endif // PASTEL_SPH_IDEAL_GAS_PRESSURE_HPP

