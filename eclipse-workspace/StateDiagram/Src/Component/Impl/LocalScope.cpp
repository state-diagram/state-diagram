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

#include "LocalScope.h"

#include "LocalSignalDelegateImpl.h"
#include "LocalVarDelegateImpl.h"

namespace state_diagram
{

LocalScope
::LocalScope(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) LocalScope const * const up, TopStateImpl * const _topState)
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, topState{_topState}
, m_up{up}
#ifndef STATE_DIAGRAM_STRINGLESS
, m_localSignalNames{}
, m_localVarNames{}
#endif // STATE_DIAGRAM_STRINGLESS
, m_localSignals{}
, m_localVars{}
{
  // This space intentionally left empty
}

void
LocalScope
::insertLocalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) LocalSignalDelegateImpl * const localSignal)
{
#ifndef STATE_DIAGRAM_STRINGLESS
  auto const stat{m_localSignalNames.insert(_name).second};
  if (!stat)
  {
    throwSignalNameClashError
    (
      _name
    );
  }
#endif // STATE_DIAGRAM_STRINGLESS
  m_localSignals.emplace_front(localSignal);
}

bool
LocalScope
::hasInScope(LocalSignalDelegateImpl const * const localSignal)
const
{
  if (containsItem(m_localSignals, localSignal))
  {
    return true;
  }
  if (m_up != nullptr)
  {
    return m_up->hasInScope(localSignal);
  }
  return false;
}

void
LocalScope
::insertLocalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) LocalVarDelegateImpl * const localVar)
{
#ifndef STATE_DIAGRAM_STRINGLESS
  auto const stat{m_localVarNames.insert(_name).second};
  if (!stat)
  {
    throwVarNameClashError
    (
      _name
    );
  }
#endif // STATE_DIAGRAM_STRINGLESS
  m_localVars.emplace_front(localVar);
}

bool
LocalScope
::hasInScope(LocalVarDelegateImpl const * const localVar)
const
{
  if (containsItem(m_localVars, localVar))
  {
    return true;
  }
  if (m_up != nullptr)
  {
    return m_up->hasInScope(localVar);
  }
  return false;
}

void
LocalScope
::unsetLocalVars()
const
{
  auto const unsetLocalVar{[](LocalVarDelegateImpl * const localVar){localVar->unset();}};
  forEachItem<LocalVarDelegateImpl>(m_localVars, unsetLocalVar);
}

void
LocalScope
::reload()
const
{
  auto const resetLocalSignalToAbsent{[](LocalSignalDelegateImpl * const localSignal){localSignal->deactivate();}};
  forEachItem<LocalSignalDelegateImpl>(m_localSignals, resetLocalSignalToAbsent);

  unsetLocalVars();
}

} // namespace state_diagram
