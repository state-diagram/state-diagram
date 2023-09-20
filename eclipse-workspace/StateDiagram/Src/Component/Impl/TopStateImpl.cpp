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

#include "TopStateImpl.h"

#include <cassert>

#include "ExternalSignalDelegateImpl.h"
#include "ExternalVarDelegateImpl.h"
#include "LocalVarDelegateImpl.h"

namespace state_diagram
{

TopStateImpl
::TopStateImpl(STATE_DIAGRAM_STRING_PARAM(_name))
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, CompoundStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) this}
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
, curLocalScope{}
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
#ifndef STATE_DIAGRAM_STRINGLESS
, m_externalSignalNames{}
, m_externalVarNames{}
#endif // STATE_DIAGRAM_STRINGLESS
, m_externalSignals{}
, m_externalVars{}
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
, m_isUnderExecution{false}
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
{
  // This space intentionally left empty
}

RegionImpl *
TopStateImpl
::parentRegion()
const
{
  assert (false); // Never to be called
}

StateImpl *
TopStateImpl
::asState()
{
  assert (false); // Never to be called
}

#ifndef STATE_DIAGRAM_STRINGLESS

void
TopStateImpl
::path(ostream & to)
const
{
  to << name;
}

#endif // STATE_DIAGRAM_STRINGLESS

bool
TopStateImpl
::isDeepMemberOf(RegionImpl const * const)
const
{
  return false;
}

void
TopStateImpl
::init()
{
  CompoundStateImpl::init();
}

void
TopStateImpl
::activate(ExternalSignalDelegateImpl * const trigger)
{
  trigger->activate();
}

void
TopStateImpl
::reload()
const
{
  CompoundStateImpl::reload();
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  m_isUnderExecution = false;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  for (auto const & externalSignal : m_externalSignals)
  {
    externalSignal->deactivate();
  }
  for (auto const & externalVar : m_externalVars)
  {
    externalVar->unset();
  }
}

bool
TopStateImpl
::exec()
{
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  m_isUnderExecution = true;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  class Reloader
  {
  public:
    Reloader(TopStateImpl const * const subject)
    :
      m_subject{subject}
    {
      // This space intentionally left empty
    }
    Reloader(Reloader const &) = delete;

    ~Reloader()
    {
      m_subject->reload();
    }

    void operator=(Reloader const &) = delete;

  private:
    TopStateImpl const * const m_subject;
  };

  Reloader const reloader(this);

  bool sawAllRegionsTerminated;

  for (;;)
  {
    bool sawSomeRegionActive{false};
    sawAllRegionsTerminated = true;
    auto const stepRegion
    {
      [&](RegionImpl * const region)
      {
        ExecStat const execStat{region->exec()};
        if (execStat.stat && (execStat.unwindCmd.action == UnwindCmd::Action::UNWIND))
        {
          assert (false);
        }
        sawSomeRegionActive |= execStat.stat;
        sawAllRegionsTerminated &= region->hasTerminated();
      }
    };
#ifndef STATE_DIAGRAM_NO_SHUFFLING
    forEachRegionRandomly(stepRegion);
#else
    forEachRegion(stepRegion);
#endif // STATE_DIAGRAM_NO_SHUFFLING
    if ((!sawSomeRegionActive) || sawAllRegionsTerminated)
    {
      break;
    }
  }

  return sawAllRegionsTerminated;
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
TopStateImpl
::isInCurLocalScope(LocalSignalDelegateImpl const * const localSignal)
const
{
  assert (curLocalScope != nullptr);
  return curLocalScope->hasInScope(localSignal);
}

bool
TopStateImpl
::isInCurLocalScope(LocalVarDelegateImpl const * const localVar)
const
{
  assert (curLocalScope != nullptr);
  return curLocalScope->hasInScope(localVar);
}

bool
TopStateImpl
::isUnderExecution()
const
{
  return m_isUnderExecution;
}

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

void
TopStateImpl
::insertExternalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) ExternalSignalDelegateImpl * const externalSignal)
{
#ifndef STATE_DIAGRAM_STRINGLESS
  auto const stat{m_externalSignalNames.emplace(_name).second};
  if (!stat)
  {
    throw ExternalSignalDelegate::NameClashError(_name);
  }
#endif // STATE_DIAGRAM_STRINGLESS
  m_externalSignals.emplace_front(externalSignal);
}

void
TopStateImpl
::insertExternalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) ExternalVarDelegateImpl * const externalVar)
{
#ifndef STATE_DIAGRAM_STRINGLESS
  auto const stat{m_externalVarNames.emplace(_name).second};
  if (!stat)
  {
    throw ExternalVarDelegate::NameClashError(_name);
  }
#endif // STATE_DIAGRAM_STRINGLESS
  m_externalVars.emplace_front(externalVar);
}

} // namespace state_diagram
