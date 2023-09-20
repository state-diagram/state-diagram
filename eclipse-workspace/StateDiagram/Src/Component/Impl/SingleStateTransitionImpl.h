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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_SINGLESTATETRANSITIONIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_SINGLESTATETRANSITIONIMPL_H_

#include "state_diagram/state_diagram.h"

#include "StateImpl.h"
#include "SubStateImpl.h"
#include "TransitionImpl.h"

namespace state_diagram
{

class SingleStateTransitionImpl
:
  public virtual TransitionImpl
{
public:
  SingleStateTransitionImpl(StateImpl * const host);
  SingleStateTransitionImpl(SingleStateTransitionImpl const &) = delete;

  virtual ~SingleStateTransitionImpl();

  void operator=(SingleStateTransitionImpl const &) = delete;

  StateImpl * const host;

  SourceStateImpl * origin() const override;

  SubStateImpl const * outputScope() const override;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
  virtual
  void
  accept
  (
    function<void (EnterTransitionImpl * const)> const onEnter
  , function<void (ExitTransitionImpl * const)> const onExit
  , function<void (InternalAutoTransitionImpl * const)> const onInternalAuto
  , function<void (InternalStepTransitionImpl * const)> const onInternalStep
  )
  = 0;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
  void
  accept
  (
    function<void (SingleStateTransitionImpl * const)> const onSingleState
  , function<void (AutoTransitionImpl * const)> const onAuto
  , function<void (StepTransitionImpl * const)> const onStep
  )
  override;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_SINGLESTATETRANSITIONIMPL_H_
