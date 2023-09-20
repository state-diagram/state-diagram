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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_TRANSITIONIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_TRANSITIONIMPL_H_

#include "state_diagram/state_diagram.h"

#include "SourceStateImpl.h"
#include "SubStateImpl.h"

namespace state_diagram
{

class TriggerlessTransitionImpl;
class TriggeredTransitionImpl;
class SingleStateTransitionImpl;
class AutoTransitionImpl;
class StepTransitionImpl;

class TransitionImpl
{
public:
  virtual ~TransitionImpl();

  virtual SourceStateImpl * origin() const = 0;

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
  virtual
  void
  accept
  (
    function<void (TriggerlessTransitionImpl * const)> const onTriggerless
  , function<void (TriggeredTransitionImpl * const)> const onTriggered
  )
  = 0;

  virtual
  void
  accept
  (
    function<void (SingleStateTransitionImpl * const)> const onSingleState
  , function<void (AutoTransitionImpl * const)> const onAuto
  , function<void (StepTransitionImpl * const)> const onStep
  )
  = 0;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

protected:
  virtual SubStateImpl const * outputScope() const = 0;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_TRANSITIONIMPL_H_
