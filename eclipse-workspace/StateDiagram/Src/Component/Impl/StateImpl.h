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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_STATEIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_STATEIMPL_H_

#include "Util/ForwardList.hpp"
#include "CompoundStateImpl.h"
#include "SourceStateImpl.h"
#include "TargetStateImpl.h"

namespace state_diagram
{

class EnterTransitionImpl;
class ExitTransitionImpl;
class InternalTransitionImpl;
class StepTransitionImpl;

class StateImpl
:
  public CompoundStateImpl
, public SourceStateImpl
, public TargetStateImpl
{
public:
  StateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundStateImpl * const parent);
  StateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) RegionImpl * const parent);

  RegionImpl * parentRegion() const override;

  StateImpl * asState() override;

#ifndef STATE_DIAGRAM_STRINGLESS
  using NamePathImpl::path;
#endif // STATE_DIAGRAM_STRINGLESS

  void add(EnterTransitionImpl * const enterTransition);
  void add(ExitTransitionImpl * const exitTransition);
  void add(InternalAutoTransitionImpl * const internalAutoTransition);
  void add(InternalStepTransitionImpl * const internalStepTransition);

  bool isDeepMemberOf(RegionImpl const * const region) const override;
  bool hasInScope(LocalSignalDelegateImpl const * const signal) const override;

  void init() override;
  void enter() const;
  void exit() const;
  ExecStat exec() const override;
  void finalize() const override;
  void unfreeze() override;
  void complete(bool const freeze, FreezeDepth const freezeDepth) override;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
  void reload() override;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

  bool hasTerminated() const override;

private:
  ForwardList<BoundaryTransitionImpl * const> m_enterTransitions;
  ForwardList<BoundaryTransitionImpl * const> m_exitTransitions;
  ForwardList<InternalTransitionImpl * const> m_internalTransitions;

  bool m_isPaused;
  bool isPaused() const override;

  bool m_isFrozen;
  FreezeDepth m_freezeDepth;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_STATEIMPL_H_
