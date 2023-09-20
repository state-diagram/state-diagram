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

#include "LocalSignalDelegateImpl.h"

#include "CompoundStateImpl.h"
#include "RegionImpl.h"
#include "LocalScope.h"
#include "TopStateImpl.h"

namespace state_diagram
{

LocalSignalDelegateImpl
::LocalSignalDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) CompoundStateImpl * const _scope)
:
  SignalDelegateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, m_scope{_scope}
{
  m_scope->insertLocalSignal(STATE_DIAGRAM_STRING_ARG_COMMA(name) this);
}

LocalSignalDelegateImpl
::LocalSignalDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) RegionImpl * const _scope)
:
  SignalDelegateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, m_scope{_scope}
{
  m_scope->insertLocalSignal(STATE_DIAGRAM_STRING_ARG_COMMA(name) this);
}

void
LocalSignalDelegateImpl
::accept
(
  function<void (ExternalSignalDelegateImpl * const)>
, function<void (LocalSignalDelegateImpl  * const)> onLocal
)
{
  onLocal(this);
}

#ifndef STATE_DIAGRAM_STRINGLESS

void
LocalSignalDelegateImpl
::path(ostream & to)
const
{
  m_scope->pathPrefix(to);
  to << name;
}

#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
LocalSignalDelegateImpl
::isUnderExecution()
const
{
  return m_scope->topState->isUnderExecution();
}

bool
LocalSignalDelegateImpl
::isInCurLocalScope()
const
{
  return m_scope->topState->isInCurLocalScope(this);
}

#ifndef STATE_DIAGRAM_STRINGLESS

string
LocalSignalDelegateImpl
::curLocalScopePath()
const
{
  return m_scope->topState->curLocalScope->path();
}

#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

}
