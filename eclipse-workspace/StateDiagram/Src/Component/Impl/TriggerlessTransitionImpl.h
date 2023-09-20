/*   
   (c) Copyright 2019-2021 State Diagram Contributors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_TRIGGERLESSTRANSITIONIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_TRIGGERLESSTRANSITIONIMPL_H_

#include "state_diagram/state_diagram.h"

#include "Util/ForwardList.hpp"
#include "TransitionImpl.h"

namespace state_diagram
{

class TriggerlessTransitionImpl
:
  public virtual TransitionImpl
{
public:
  TriggerlessTransitionImpl();

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
  void
  accept
  (
    function<void (TriggerlessTransitionImpl * const)> const onTriggerless
  , function<void (TriggeredTransitionImpl * const)> const onTriggered
  )
  override;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

  void add(Trigger const * const trigger);
  void add(Guard const * const guard);
  void add(Output const * const output);
  void add(Action const * const action);

protected:
  virtual string transitionTypeIndicator() const = 0;

  ExecStat exec();

private:
#ifndef STATE_DIAGRAM_STRINGLESS
  void throwTriggeredSpecOnTriggerlessTransitionError(string const & specTypeIndicator);
#endif // STATE_DIAGRAM_STRINGLESS

  ForwardList<TriggerlessGuard const> m_guards;
  ForwardList<TriggerlessOutputFun const> m_outputFuns;
  ForwardList<TriggerlessAction const> m_actions;
};

}

#endif // STATE_DIAGRAM_COMPONENT_IMPL_TRIGGERLESSTRANSITIONIMPL_H_
