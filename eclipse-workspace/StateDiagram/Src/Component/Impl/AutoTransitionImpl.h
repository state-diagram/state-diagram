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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_AUTOTRANSITIONIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_AUTOTRANSITIONIMPL_H_

#include "state_diagram/state_diagram.h"

#include "ExternalTransitionImpl.h"
#include "MaxableTransition.h"
#include "TriggerlessTransitionImpl.h"
#include "SourceStateImpl.h"

namespace state_diagram
{

class AutoTransitionImpl
:
  public ExternalTransitionImpl
, public TriggerlessTransitionImpl
, public MaxableTransition
{
public:
  AutoTransitionImpl(SourceStateImpl * const source, TargetStateImpl * const target);

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
  void
  accept
  (
    function<void (SingleStateTransitionImpl * const)> const onSingleState
  , function<void (AutoTransitionImpl * const)> const onAuto
  , function<void (StepTransitionImpl * const)> const onStep
  )
  override;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

  ExecStat exec() override;

private:
  string transitionTypeIndicator() const override;

  SubStateImpl const * outputScope() const override;

  void add(Guard const * const guard) override;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_AUTOTRANSITIONIMPL_H_
