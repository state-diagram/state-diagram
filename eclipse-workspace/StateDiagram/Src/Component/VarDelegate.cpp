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

#include "state_diagram/state_diagram.h"

#include "Impl/VarDelegateImpl.h"

namespace state_diagram
{

VarDelegate
::VarDelegate(VarDelegateImpl * const impl, Delegator * const delegator)
:
  PImplUpcast<VarDelegateImpl>{impl}
, NamePath{impl}
, m_delegator{delegator}
{
  // This space intentionally left empty
}

VarDelegateImpl *
VarDelegate
::implUpcast()
const
{
  return PImplUpcast<VarDelegateImpl>::m_implUpcast;
}

void
VarDelegate
::makeNxtCur()
const
{
  m_delegator->makeNxtCur();
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
VarDelegate
::isValid()
const
{
  return implUpcast()->isValid();
}

void
VarDelegate
::markAsValid()
const
{
  implUpcast()->markAsValid();
}

bool
VarDelegate
::isSet()
const
{
  return implUpcast()->isSet();
}

void
VarDelegate
::markAsSet()
const
{
  implUpcast()->markAsSet();
}

bool
VarDelegate
::isSetNxt()
const
{
  return implUpcast()->isSetNxt();
}

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

void
VarDelegate
::markAsSetNxt()
const
{
  implUpcast()->markAsSetNxt();
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
VarDelegate
::hasBeenRetrieved()
const
{
  return implUpcast()->hasBeenRetrieved();
}

void
VarDelegate
::markAsRetrieved()
const
{
  implUpcast()->markAsRetrieved();
}

bool
VarDelegate
::isUnderExecution()
const
{
  return implUpcast()->isUnderExecution();
}

#ifndef STATE_DIAGRAM_STRINGLESS

string
VarDelegate
::curLocalScopePath()
const
{
  return implUpcast()->curLocalScopePath();
}

#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

} // namespace state_diagram
