//
// Created by Shufang Zhu on 07/03/2025.
//

#include "SymbolicStateDfaClassical.h"
#include <algorithm>

#include <bitset>
#include <CLI/TypeTools.hpp>

namespace Syft {

SymbolicStateDfaClassical::SymbolicStateDfaClassical(std::shared_ptr<VarMgr> var_mgr)
  : var_mgr_(std::move(var_mgr))
{}


SymbolicStateDfaClassical SymbolicStateDfaClassical::from_explicit_classical(
    const ExplicitStateDfa& explicit_dfa) {
  std::shared_ptr<VarMgr> var_mgr = explicit_dfa.var_mgr();
  SymbolicStateDfa symbolic_dfa = SymbolicStateDfa::from_explicit(std::move(explicit_dfa));
  
  CUDD::BDD transition_function = symbolic_transition_function_classical(std::move(symbolic_dfa));
    

  SymbolicStateDfaClassical symbolic_dfa_classical(var_mgr);
  symbolic_dfa_classical.automaton_id_ = symbolic_dfa.automaton_id();
  symbolic_dfa_classical.initial_state_ = std::move(symbolic_dfa.initial_state());
  symbolic_dfa_classical.final_states_ = std::move(symbolic_dfa.final_states());
  symbolic_dfa_classical.transition_function_ = std::move(transition_function);

  return symbolic_dfa_classical;
}

CUDD::BDD SymbolicStateDfaClassical::symbolic_transition_function_classical(
       const SymbolicStateDfa &symbolic_dfa){
  std::shared_ptr<VarMgr> var_mgr = symbolic_dfa.var_mgr();
  std::size_t bit_count = var_mgr->state_variable_count(symbolic_dfa.automaton_id());
  std::size_t classical_id = var_mgr->create_primed_state_variables(bit_count);
  assert(symbolic_dfa.automaton_id() == classical_id);

  std::vector<CUDD::BDD> partitioned_trnsition_function = symbolic_dfa.transition_function();
  CUDD::BDD transition_function = var_mgr->cudd_mgr()->bddOne();
  for (size_t i = 0; i < partitioned_trnsition_function.size(); ++i) {
    CUDD::BDD transition = (partitioned_trnsition_function[i] & var_mgr->primed_state_variable(classical_id, i)) |
      (!partitioned_trnsition_function[i] & !var_mgr->primed_state_variable(classical_id, i));
    transition_function = transition_function * transition;
  }
  return transition_function;
}


std::shared_ptr<VarMgr> SymbolicStateDfaClassical::var_mgr() const {
  return var_mgr_;
}

std::size_t SymbolicStateDfaClassical::automaton_id() const {
  return automaton_id_;
}
  
std::vector<int> SymbolicStateDfaClassical::initial_state() const {
  return initial_state_;
}

// CUDD::BDD SymbolicStateDfaClassical::initial_state_bdd() const {
//   return state_to_bdd(var_mgr_, automaton_id_, 1);
// }

CUDD::BDD SymbolicStateDfaClassical::final_states() const {
  return final_states_;
}

CUDD::BDD SymbolicStateDfaClassical::transition_function() const {
  return transition_function_;
}




void SymbolicStateDfaClassical::dump_dot(const std::string& filename) const {
  std::vector<std::string> function_labels;
  function_labels.push_back("Transition");
  function_labels.push_back("Final");

  std::vector<CUDD::ADD> adds;
  adds.push_back(transition_function_.Add());
  adds.push_back(final_states_.Add());

  var_mgr_->dump_dot(adds, function_labels, filename);
}


  std::size_t SymbolicStateDfaClassical::bdd_nodes_num_transitions() const {
    std::vector<CUDD::BDD> bdd_vec;
    bdd_vec.push_back(transition_function_);
    return var_mgr()->cudd_mgr()->nodeCount(bdd_vec);
  }

  std::size_t SymbolicStateDfaClassical::bdd_nodes_num_final_states() const {
    std::vector<CUDD::BDD> bdd_vec;
    bdd_vec.push_back(final_states_);
    return var_mgr()->cudd_mgr()->nodeCount(bdd_vec);
  }

  std::size_t SymbolicStateDfaClassical::bdd_nodes_num() const {
    std::vector<CUDD::BDD> bdd_vec;
    bdd_vec.push_back(transition_function_);
    bdd_vec.push_back(final_states_);
    return var_mgr()->cudd_mgr()->nodeCount(bdd_vec);
  }


}

