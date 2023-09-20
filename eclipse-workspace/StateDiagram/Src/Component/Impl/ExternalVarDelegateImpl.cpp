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

#include "ExternalVarDelegateImpl.h"

#include <cassert>

#include "TopStateImpl.h"

namespace state_diagram
{

ExternalVarDelegateImpl
::ExternalVarDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) TopStateImpl * const _parent, ExternalVarDelegate * const _interface)
:
  VarDelegateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _interface}
, m_parent{_parent}
{
  m_parent->insertExternalVar(STATE_DIAGRAM_STRING_ARG_COMMA(name) this);
}

void
ExternalVarDelegateImpl
::accept(function<void (ExternalVarDelegateImpl * const)> onExternal, function<void (LocalVarDelegateImpl  * const)>)
{
  onExternal(this);
}

#ifndef STATE_DIAGRAM_STRINGLESS

void
ExternalVarDelegateImpl
::path(ostream & to)
const
{
  to << name;
}

#endif // STATE_DIAGRAM_STRINGLESS

void
ExternalVarDelegateImpl
::unset()
{
  VarDelegateImpl::unset();
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
ExternalVarDelegateImpl
::isUnderExecution()
const
{
  return m_parent->isUnderExecution();
}

bool
ExternalVarDelegateImpl
::isInCurLocalScope()
const
{
  return true;
}

#ifndef STATE_DIAGRAM_STRINGLESS

string
ExternalVarDelegateImpl
::curLocalScopePath()
const
{
  return m_parent->curLocalScope->path();
}

#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

}
