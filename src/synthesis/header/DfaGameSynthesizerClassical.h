//
// Created by Shufang Zhu on 07/03/2025.
//

#ifndef DFAGAMESYNTHESIZERCLASSICAL_H
#define DFAGAMESYNTHESIZERCLASSICAL_H



#include "Quantification.h"
#include "SymbolicStateDfaClassical.h"
#include "Synthesizer.h"
#include "Transducer.h"

namespace Syft {

  class DfaGameSynthesizerClassical : public Synthesizer<SymbolicStateDfaClassical> {
  protected:

    std::shared_ptr<VarMgr> var_mgr_;
    Player starting_player_;
    Player protagonist_player_;
    std::vector<int> initial_vector_;
    std::vector<CUDD::BDD> transition_vector_;
    std::unique_ptr<Quantification> quantify_independent_variables_;
    std::unique_ptr<Quantification> quantify_non_state_variables_;
    std::unique_ptr<Quantification> quantify_primed_state_variables_;

    CUDD::BDD preimage(const CUDD::BDD& winning_states) const;

    CUDD::BDD project_into_states(const CUDD::BDD& winning_moves) const;

    bool includes_initial_state(const CUDD::BDD& winning_states) const;

  public:

    DfaGameSynthesizerClassical(SymbolicStateDfaClassical spec, Player starting_player, Player protagonist_player);


    /**
     * \brief Computes a winning strategy for the game.
     *
     * \return A transducer representing the winning strategy.
     */
    virtual SynthesisResult run()
        const override = 0;
  };

}


#endif //DFAGAMESYNTHESIZERCLASSICAL_H
