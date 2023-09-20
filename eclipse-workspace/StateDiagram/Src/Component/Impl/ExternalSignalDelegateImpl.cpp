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

#include "ExternalSignalDelegateImpl.h"
#include "TopStateImpl.h"

namespace state_diagram
{

ExternalSignalDelegateImpl
::ExternalSignalDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) TopStateImpl * const _parent)
:
  SignalDelegateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, m_parent{_parent}
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
, m_isUnderExecution{false}
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
{
  m_parent->insertExternalSignal(STATE_DIAGRAM_STRING_ARG_COMMA(name) this);
}

void
ExternalSignalDelegateImpl
::accept
(
  function<void (ExternalSignalDelegateImpl * const)> onExternal
, function<void (LocalSignalDelegateImpl  * const)>
)
{
  onExternal(this);
}

#ifndef STATE_DIAGRAM_STRINGLESS

void
ExternalSignalDelegateImpl
::path(ostream & to)
const
{
  to << name;
}

#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

void
ExternalSignalDelegateImpl
::setUnderExecution()
{
  m_isUnderExecution = true;
}

bool
ExternalSignalDelegateImpl
::isUnderExecution()
const
{
  return m_parent->isUnderExecution();
}

bool
ExternalSignalDelegateImpl
::isInCurLocalScope()
const
{
  return true;
}

#ifndef STATE_DIAGRAM_STRINGLESS

string
ExternalSignalDelegateImpl
::curLocalScopePath()
const
{
  return m_parent->curLocalScope->path();
}

#endif // STATE_DIAGRAM_STRINGLESS

void
ExternalSignalDelegateImpl
::deactivate()
{
  SignalDelegateImpl::deactivate();
  m_isUnderExecution = false;
}

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

}
