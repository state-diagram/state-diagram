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

#include <cassert>

#include "Impl/SignalDelegateImpl.h"

namespace state_diagram
{

SignalDelegate
::SignalDelegate(SignalDelegateImpl * const impl)
:
  PImplUpcast<SignalDelegateImpl>{impl}
, NamePath{impl}
{
  // This space intentionally left empty
}

SignalDelegateImpl *
SignalDelegate
::implUpcast()
const
{
  return PImplUpcast<SignalDelegateImpl>::m_implUpcast;
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
SignalDelegate
::isActive()
const
{
  return implUpcast()->isActive();
}

bool
SignalDelegate
::isSet()
const
{
  return implUpcast()->isSet();
}

void
SignalDelegate
::markAsSet()
const
{
  implUpcast()->markAsSet();
}

bool
SignalDelegate
::isUnderExecution()
const
{
  return implUpcast()->isUnderExecution();
}

#ifndef STATE_DIAGRAM_STRINGLESS

string
SignalDelegate
::curLocalScopePath()
const
{
  return implUpcast()->curLocalScopePath();
}

#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

}
