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

#include "SourceStateImpl.h"

#include <cassert>
#ifndef STATE_DIAGRAM_NO_SHUFFLING
#  include "Util/ShufflingAlgorithm.hpp"
#endif // STATE_DIAGRAM_NO_SHUFFLING
#include "AutoTransitionImpl.h"
#include "StepTransitionImpl.h"
#include "TopStateImpl.h"

namespace state_diagram
{

SourceStateImpl
::SourceStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) RegionImpl * const _parent)
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, SubStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
, m_autoTransitions{}
, m_stepTransitions{}
{
  // This space intentionally left empty
}

void
SourceStateImpl
::attach(AutoTransitionImpl * const autoTransition)
{
  m_autoTransitions.emplace_front(autoTransition);
}

void
SourceStateImpl
::attach(StepTransitionImpl * const stepTransition)
{
  m_stepTransitions.emplace_front(stepTransition);
}

size_t
SourceStateImpl
::autoTransitionsSize()
const
{
  return m_autoTransitions.size();
}

SourceStateImpl::AutoTransitions const &
SourceStateImpl
::autoTransitions()
const
{
  return m_autoTransitions;
}

size_t
SourceStateImpl
::stepTransitionsSize()
const
{
  return m_stepTransitions.size();
}

SourceStateImpl::StepTransitions const &
SourceStateImpl
::stepTransitions()
const
{
  return m_stepTransitions;
}

ExecStat
SourceStateImpl
::exec()
const
{
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  parentRegion()->topState->curLocalScope = parentRegion();
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  vector<ExternalTransitionImpl *> shuffler{autoTransitionsSize() + stepTransitionsSize()};
  {
    size_t idx{0};
    for (auto const & autoTransition : autoTransitions())
    {
      shuffler[idx] = autoTransition;
      ++idx;
    }
    for (auto const & stepTransition : stepTransitions())
    {
      shuffler[idx] = stepTransition;
      ++idx;
    }
  }
  random_shuffle(shuffler.begin(), shuffler.end());
  for (auto const & transition : shuffler)
  {
    ExecStat const execStat{transition->exec()};
    if (execStat.stat)
    {
      return execStat;
    }
  }
#else
  for (auto & transition : autoTransitions())
  {
    ExecStat const execStat{transition->exec()};
    if (execStat.stat)
    {
      return execStat;
    }
  }
  for (auto & transition : stepTransitions())
  {
    ExecStat const execStat{transition->exec()};
    if (execStat.stat)
    {
      return execStat;
    }
  }
#endif // STATE_DIAGRAM_NO_SHUFFLING
  return ExecStat{};
}

void
SourceStateImpl
::complete(bool const, FreezeDepth const)
{
  // This space intentionally left empty
}

void
SourceStateImpl
::finalize()
const
{
  // This space intentionally left empty
}

void
SourceStateImpl
::reload()
{
  for (auto & autoTransition : m_autoTransitions)
  {
    autoTransition->reload();
  }
  for (auto & stepTransition : m_stepTransitions)
  {
    stepTransition->reload();
  }
}

bool
SourceStateImpl
::isPaused()
const
{
  return false;
}

bool
SourceStateImpl
::hasTerminated()
const
{
  return true;
}

} // namespace state_diagram
