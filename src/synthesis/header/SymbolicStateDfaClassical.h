//
// Created by Shufang Zhu on 07/03/2025.
//

#ifndef SymbolicStateDfaClassicalCLASSICAL_H
#define SymbolicStateDfaClassicalCLASSICAL_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include <cuddObj.hh>

#include "ExplicitStateDfa.h"
#include "SymbolicStateDfa.h"
#include "ExplicitStateDfaMona.h"

namespace Syft {
  /**
   * \brief A DFA with symbolic states and transitions in classical encoding.
   */
  class SymbolicStateDfaClassical {
  private:
    std::shared_ptr<VarMgr> var_mgr_;
    std::size_t automaton_id_;

    std::vector<int> initial_state_;
    CUDD::BDD final_states_;
    CUDD::BDD transition_function_;

    SymbolicStateDfaClassical(std::shared_ptr<VarMgr> var_mgr);


    static CUDD::BDD symbolic_transition_function_classical(
     const SymbolicStateDfa &symbolic_dfa);


  public:

    static SymbolicStateDfaClassical from_explicit_classical(const ExplicitStateDfa &explicit_dfa);


    /**
     * \brief Returns the variable manager.
     */
    std::shared_ptr<VarMgr> var_mgr() const;

    /**
     * \brief Returns the automaton ID.
     *
     * This ID can be used to retrieve the associated state variables from the
     * variable manager.
     */
    std::size_t automaton_id() const;

    /**
     * \brief Returns the bitvector representing the initial state of the DFA.
     */
    std::vector<int> initial_state() const;

    // /**
    //  * \brief Returns the BDD representing the initial state of the DFA.
    //  */
    // CUDD::BDD initial_state_bdd() const;

    /**
     * \brief Returns the BDD encoding the set of final states.
     */
    CUDD::BDD final_states() const;


    CUDD::BDD transition_function() const;


    void dump_dot(const std::string &filename) const;

    std::size_t bdd_nodes_num() const;

    std::size_t bdd_nodes_num_final_states() const;

    std::size_t bdd_nodes_num_transitions() const;
  };
}



#endif //SymbolicStateDfaClassicalCLASSICAL_H
