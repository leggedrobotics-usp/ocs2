

#ifndef OCS2_LOOPSHAPINGCOSTOUTPUTPATTERN_H
#define OCS2_LOOPSHAPINGCOSTOUTPUTPATTERN_H

namespace ocs2 {
template <size_t FULL_STATE_DIM, size_t FULL_INPUT_DIM, size_t SYSTEM_STATE_DIM, size_t SYSTEM_INPUT_DIM, size_t FILTER_STATE_DIM,
          size_t FILTER_INPUT_DIM>
class LoopshapingCostOutputPattern final
    : public LoopshapingCost<FULL_STATE_DIM, FULL_INPUT_DIM, SYSTEM_STATE_DIM, SYSTEM_INPUT_DIM, FILTER_STATE_DIM, FILTER_INPUT_DIM> {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  using BASE = LoopshapingCost<FULL_STATE_DIM, FULL_INPUT_DIM, SYSTEM_STATE_DIM, SYSTEM_INPUT_DIM, FILTER_STATE_DIM, FILTER_INPUT_DIM>;
  using typename BASE::input_matrix_t;
  using typename BASE::input_state_matrix_t;
  using typename BASE::input_vector_t;
  using typename BASE::scalar_t;
  using typename BASE::state_matrix_t;
  using typename BASE::state_vector_t;

  using typename BASE::system_input_matrix_t;
  using typename BASE::system_input_state_matrix_t;
  using typename BASE::system_input_vector_t;
  using typename BASE::system_state_matrix_t;
  using typename BASE::system_state_vector_t;
  using typename BASE::SYSTEMCOST;

  using typename BASE::filter_input_vector_t;
  using typename BASE::filter_state_vector_t;

  LoopshapingCostOutputPattern(const SYSTEMCOST& systemCost, std::shared_ptr<LoopshapingDefinition> loopshapingDefinition)
      : BASE(systemCost, std::move(loopshapingDefinition)) {}

  ~LoopshapingCostOutputPattern() override = default;

  LoopshapingCostOutputPattern(const LoopshapingCostOutputPattern& obj) = default;

  LoopshapingCostOutputPattern* clone() const override { return new LoopshapingCostOutputPattern(*this); };

  void getIntermediateCostDerivativeState(state_vector_t& dLdx) override {
    this->computeApproximation();
    const auto& gamma = loopshapingDefinition_->gamma_;
    auto& r_filter = loopshapingDefinition_->getInputFilter();
    dLdx.segment(0, SYSTEM_STATE_DIM) = gamma * q_filter_ + (1.0 - gamma) * q_system_;
    dLdx.segment(SYSTEM_STATE_DIM, FILTER_STATE_DIM) = gamma * r_filter.getC().transpose() * r_filter_;
  };

  void getIntermediateCostSecondDerivativeState(state_matrix_t& dLdxx) override {
    this->computeApproximation();
    auto& gamma = loopshapingDefinition_->gamma_;
    auto& r_filter = loopshapingDefinition_->getInputFilter();
    dLdxx.block(0, 0, SYSTEM_STATE_DIM, SYSTEM_STATE_DIM) = gamma * Q_filter_ + (1.0 - gamma) * Q_system_;
    dLdxx.block(0, SYSTEM_STATE_DIM, SYSTEM_STATE_DIM, FILTER_STATE_DIM) = gamma * P_filter_.transpose() * r_filter.getC();
    dLdxx.block(SYSTEM_STATE_DIM, 0, FILTER_STATE_DIM, SYSTEM_STATE_DIM) =
        dLdxx.block(0, SYSTEM_STATE_DIM, SYSTEM_STATE_DIM, FILTER_STATE_DIM).transpose();
    dLdxx.block(SYSTEM_STATE_DIM, SYSTEM_STATE_DIM, FILTER_STATE_DIM, FILTER_STATE_DIM) =
        gamma * r_filter.getC().transpose() * R_filter_ * r_filter.getC();
  };

  void getIntermediateCostDerivativeInput(input_vector_t& dLdu) override {
    this->computeApproximation();
    const auto& gamma = loopshapingDefinition_->gamma_;
    auto& r_filter = loopshapingDefinition_->getInputFilter();
    dLdu.segment(0, SYSTEM_INPUT_DIM) = gamma * r_filter.getD().transpose() * r_filter_ + (1.0 - gamma) * r_system_;
  };

  void getIntermediateCostSecondDerivativeInput(input_matrix_t& dLduu) override {
    this->computeApproximation();
    const auto& gamma = loopshapingDefinition_->gamma_;
    auto& r_filter = loopshapingDefinition_->getInputFilter();
    dLduu.block(0, 0, SYSTEM_INPUT_DIM, SYSTEM_INPUT_DIM) =
        gamma * r_filter.getD().transpose() * R_filter_ * r_filter.getD() + (1.0 - gamma) * R_system_;
  };

  void getIntermediateCostDerivativeInputState(input_state_matrix_t& dLdux) override {
    this->computeApproximation();
    const auto& gamma = loopshapingDefinition_->gamma_;
    auto& r_filter = loopshapingDefinition_->getInputFilter();
    dLdux.block(0, 0, SYSTEM_INPUT_DIM, SYSTEM_STATE_DIM) = gamma * r_filter.getD().transpose() * P_filter_ + (1.0 - gamma) * P_system_;
    dLdux.block(0, SYSTEM_STATE_DIM, SYSTEM_INPUT_DIM, FILTER_STATE_DIM) =
        gamma * r_filter.getD().transpose() * R_filter_ * r_filter.getC();
  };

 protected:
  using BASE::c_system_;
  using BASE::loopshapingDefinition_;
  using BASE::P_system_;
  using BASE::Q_system_;
  using BASE::q_system_;
  using BASE::R_system_;
  using BASE::r_system_;

  using BASE::c_filter_;
  using BASE::P_filter_;
  using BASE::Q_filter_;
  using BASE::q_filter_;
  using BASE::R_filter_;
  using BASE::r_filter_;
};
};  // namespace ocs2

#endif  // OCS2_LOOPSHAPINGCOSTOUTPUTPATTERN_H
