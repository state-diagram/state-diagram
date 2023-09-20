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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_TRIGGEREDTRANSITIONIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_TRIGGEREDTRANSITIONIMPL_H_

#include "state_diagram/state_diagram.h"

#include "Util/ForwardList.hpp"
#include "MaxableTransition.h"
#include "Spec_all.h"
#include "TransitionImpl.h"

namespace state_diagram
{

class TriggeredTransitionImpl
:
  public virtual TransitionImpl
, public MaxableTransition
{
public:
  TriggeredTransitionImpl();

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

  void add(Event const * const trigger);
  void add(Trigger const * const trigger);
  void add(Guard const * const guard);
  void add(Output const * const output);
  void add(Action const * const action);

  virtual ExecStat exec();

private:
  Event const * chooseTriggerCheckGuards() const;

  ForwardList<Event const * const> m_triggers;
  ForwardList<TriggeredGuard const> m_guards;
  ForwardList<TriggeredOutput const> m_outputs;
  ForwardList<TriggeredAction const> m_actions;
};

}

#endif // STATE_DIAGRAM_COMPONENT_IMPL_TRIGGEREDTRANSITIONIMPL_H_
