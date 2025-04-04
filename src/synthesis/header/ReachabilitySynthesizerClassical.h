//
// Created by Shufang Zhu on 07/03/2025.
//

#ifndef REACHABILITYSYNTHESIZERCLASSICAL_H
#define REACHABILITYSYNTHESIZERCLASSICAL_H



#include "SymbolicStateDfaClassical.h"
#include "DfaGameSynthesizerClassical.h"

namespace Syft {

  class ReachabilitySynthesizerClassical : public DfaGameSynthesizerClassical  {
  private:

    CUDD::BDD goal_states_;
    CUDD::BDD state_space_;

  public:

    ReachabilitySynthesizerClassical(SymbolicStateDfaClassical spec, Player starting_player, Player protagonist_player,
                                  CUDD::BDD goal_states, CUDD::BDD state_space);


    virtual SynthesisResult run() const final;

  };

}


#endif //REACHABILITYSYNTHESIZERCLASSICAL_H
