//
// Created by Shufang Zhu on 07/03/2025.
//

#include "DfaGameSynthesizerClassical.h"

#include <cassert>

namespace Syft {

DfaGameSynthesizerClassical::DfaGameSynthesizerClassical(SymbolicStateDfaClassical spec,
				       Player starting_player,
                       Player protagonist_player)
    : Synthesizer<SymbolicStateDfaClassical>(spec)
    , starting_player_(starting_player)
    , protagonist_player_(protagonist_player){
  var_mgr_ = spec_.var_mgr();

  // Make versions of the initial state and transition function that can be used
  // with CUDD::BDD::Eval and CUDD::BDD::VectorCompose, respectively
  initial_vector_ = var_mgr_->make_eval_vector(spec_.automaton_id(),
					       spec_.initial_state());
  std::vector<CUDD::BDD> primed_state_vars = var_mgr_->primed_state_variables(spec.automaton_id());
  transition_vector_ = var_mgr_->make_compose_vector(
      spec_.automaton_id(), primed_state_vars);

  CUDD::BDD input_cube = var_mgr_->input_cube();
  CUDD::BDD output_cube = var_mgr_->output_cube();
  CUDD::BDD primed_state_variable_cube = var_mgr_->primed_state_variables_cube(spec_.automaton_id());

  // quantify_independent_variables_ quantifies all variables that the outputs
  // don't depend on (input variables if the agent plays first, or no variables
  // if the environment plays first). quantify_non_state_variables_ quantifies
  // all remaining variables that are not state variables.
  if (starting_player_ == Player::Environment) {
      if (protagonist_player_ == Player::Environment) {
          quantify_independent_variables_ = std::make_unique<Forall>(output_cube);
          quantify_non_state_variables_ = std::make_unique<Exists>(input_cube);
          quantify_primed_state_variables_ = std::make_unique<Exists>(primed_state_variable_cube);
      } else {
          quantify_independent_variables_ = std::make_unique<NoQuantification>();
          quantify_non_state_variables_ = std::make_unique<ForallExists>(input_cube,
                                                                         output_cube);
          quantify_primed_state_variables_ = std::make_unique<Exists>(primed_state_variable_cube);
      }
  } else {
      if (protagonist_player_ == Player::Environment) {
          quantify_independent_variables_ = std::make_unique<NoQuantification>();
          quantify_non_state_variables_ = std::make_unique<ForallExists>(output_cube,
                                                                         input_cube);
          quantify_primed_state_variables_ = std::make_unique<Exists>(primed_state_variable_cube);
      } else {
          quantify_independent_variables_ = std::make_unique<Forall>(input_cube);
          quantify_non_state_variables_ = std::make_unique<Exists>(output_cube);
          quantify_primed_state_variables_ = std::make_unique<Exists>(primed_state_variable_cube);
      }
  }


}

CUDD::BDD DfaGameSynthesizerClassical::preimage(
    const CUDD::BDD& winning_states) const {
    // std::cout << "winning_states: \n";
    // winning_states.PrintCover();
    //
    // std::cout << "VectorCompose\n";
    // winning_states.VectorCompose(transition_vector_).PrintCover();

    // for (int i = 0; i < transition_vector_.size(); ++i) {
    //     std::cout << "transition_vector_\n"+std::to_string(i)<<std::endl;
    //     transition_vector_[i].PrintCover();
    // }

  // Transitions that move into a winning state
  CUDD::BDD temp = spec_.transition_function() & (winning_states.VectorCompose(transition_vector_));
    CUDD::BDD winning_transitions = quantify_primed_state_variables_->apply(temp);
    // std::cout << "winning_transitions: \n";
    // winning_transitions.PrintCover();

  // Quantify all variables that the outputs don't depend on
  return quantify_independent_variables_->apply(winning_transitions);
}

CUDD::BDD DfaGameSynthesizerClassical::project_into_states(
    const CUDD::BDD& winning_moves) const {
  return quantify_non_state_variables_->apply(winning_moves);
}

bool DfaGameSynthesizerClassical::includes_initial_state(
    const CUDD::BDD& winning_states) const {
  // Need to create a copy if we want to define the function as const, since
  // CUDD::BDD::Eval does not take the data as const
  std::vector<int> copy(initial_vector_);

  return winning_states.Eval(copy.data()).IsOne();
}

}
