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

#include "StateImpl.h"

#include <cassert>

#include "Util/ShufflingAlgorithm.hpp"
#include "EnterTransitionImpl.h"
#include "ExitTransitionImpl.h"
#include "InternalAutoTransitionImpl.h"
#include "InternalStepTransitionImpl.h"
#include "AutoTransitionImpl.h"
#include "StepTransitionImpl.h"
#include "TopStateImpl.h"

namespace state_diagram
{

void
execBoundaryTransitions(ForwardList<BoundaryTransitionImpl * const> const & boundaryTransitions);

StateImpl
::StateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) CompoundStateImpl * const _parent)
:
  StateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent->defaultRegion()}
{
  // This space intentionally left empty
}

StateImpl
::StateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) RegionImpl * const _parent)
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, SubStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
, CompoundStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent, _parent->topState}
, SourceStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
, TargetStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
, m_enterTransitions{}
, m_exitTransitions{}
, m_internalTransitions{}
, m_isPaused{false}
, m_isFrozen{false}
, m_freezeDepth{}
{
  parentRegion()->insertSubState(STATE_DIAGRAM_STRING_ARG_COMMA(name) this);
}

RegionImpl *
StateImpl
::parentRegion()
const
{
  return SubStateImpl::parentRegion();
}

StateImpl *
StateImpl
::asState()
{
  return this;
}

void
StateImpl
::add(EnterTransitionImpl * const enterTransition)
{
  m_enterTransitions.emplace_front(enterTransition);
}

void
StateImpl
::add(ExitTransitionImpl * const exitTransition)
{
  m_exitTransitions.emplace_front(exitTransition);
}

void
StateImpl
::add(InternalAutoTransitionImpl * const internalAutoTransition)
{
  m_internalTransitions.emplace_front(internalAutoTransition);
}

void
StateImpl
::add(InternalStepTransitionImpl * const internalStepTransition)
{
  m_internalTransitions.emplace_front(internalStepTransition);
}

bool
StateImpl
::isDeepMemberOf(RegionImpl const * const region)
const
{
  return SubStateImpl::isDeepMemberOf(region);
}

bool
StateImpl
::hasInScope(LocalSignalDelegateImpl const * const signal)
const
{
  return LocalScope::hasInScope(signal);
}

void
StateImpl
::init()
{
  m_isPaused = true;
  if (!m_isFrozen)
  {
    enter();
    CompoundStateImpl::init();
  }
  else
  {
    switch (m_freezeDepth)
    {
      case FreezeDepth::FULL:
      {
        break;
      }
      case FreezeDepth::SHALLOW:
      {
        CompoundStateImpl::shallowInit();
        break;
      }
      default:
      {
        assert (false);
      }
    }
  }
}

void
StateImpl
::enter()
const
{
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  topState->curLocalScope = this;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  unsetLocalVars();
  execBoundaryTransitions(m_enterTransitions);
}

void
StateImpl
::exit()
const
{
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  topState->curLocalScope = this;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  execBoundaryTransitions(m_exitTransitions);
}

ExecStat
StateImpl
::exec()
const
{
  auto const execInternalTransitions
  {
    [&]()->bool
    {
      bool sawSomeInternalTransitionExecuting{false};
#ifndef STATE_DIAGRAM_NO_SHUFFLING
      vector<InternalTransitionImpl *> shuffler{m_internalTransitions.size()};
      populateShuffle(shuffler, m_internalTransitions);
      for (auto & internalTransition : shuffler)
#else
      for (auto & internalTransition : m_internalTransitions)
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
      {
        sawSomeInternalTransitionExecuting |= internalTransition->exec().stat;
      }
      return sawSomeInternalTransitionExecuting;
    }
  };

  bool sawSomeRegionExecuting{false};
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  vector<RegionImpl *> shuffler{regions().size()};
  populateShuffle(shuffler, regions());
  for (auto & region : shuffler)
#else
  for (auto & region : regions())
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  {
    ExecStat const execStat{region->exec()};
    if (execStat.stat)
    {
      if (execStat.unwindCmd.action == UnwindCmd::Action::UNWIND)
      {
        return execStat;
      }
      sawSomeRegionExecuting = true;
    }
  }

  if (sawSomeRegionExecuting)
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    topState->curLocalScope = this;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    execInternalTransitions();
    return ExecStat{UnwindCmd{}};
  }

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  topState->curLocalScope = parentRegion();
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  ExecStat const execStat{SourceStateImpl::exec()};
  if (execStat.stat)
  {
    return execStat;
  }

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  topState->curLocalScope = this;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  if (execInternalTransitions())
  {
    return ExecStat{UnwindCmd{}};
  }

  return ExecStat{};
}

void
StateImpl
::finalize()
const
{
  CompoundStateImpl::finalize();
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  topState->curLocalScope = this;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  execBoundaryTransitions(m_exitTransitions);
}

void
StateImpl
::unfreeze()
{
  m_isFrozen = false;
}

void
StateImpl
::complete(bool const freeze, FreezeDepth const freezeDepth)
{
  if (freeze)
  {
    switch (freezeDepth)
    {
      case FULL:
      {
        m_freezeDepth = FULL;
        break;
      }
      case SHALLOW:
      {
        CompoundStateImpl::finalize();
        m_freezeDepth = SHALLOW;
        break;
      }
      default:
      {
        assert (false);
      }
    }
    m_isFrozen = true;
  }
  else
  {
    finalize();
    m_isFrozen = false;
  }
}

void
StateImpl
::reload()
{
  CompoundStateImpl::reload();
  SourceStateImpl::reload();
  m_isPaused = false;
  for (auto const & internalTransition : m_internalTransitions)
  {
    internalTransition->reload();
  }
}

bool
StateImpl
::hasTerminated()
const
{
  bool sawAllRegionsTerminated{true};
  auto const queryRegionHasTerminated{[&](RegionImpl const * const region){sawAllRegionsTerminated &= region->hasTerminated();}};
  forEachCRegion(queryRegionHasTerminated);
  return sawAllRegionsTerminated;
}

bool
StateImpl
::isPaused()
const
{
  return m_isPaused;
}

void
execBoundaryTransitions(ForwardList<BoundaryTransitionImpl * const> const & boundaryTransitions)
{
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  vector<BoundaryTransitionImpl *> shuffler{boundaryTransitions.size()};
  populateShuffle(shuffler, boundaryTransitions);
  for (auto const & boundaryTransition : shuffler)
#else
  for (auto const & boundaryTransition : boundaryTransitions)
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  {
    boundaryTransition->exec();
  }
}

} // namespace state_diagram


