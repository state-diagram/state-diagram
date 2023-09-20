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

#include "SignalDelegateImpl.h"

#include <cassert>

namespace state_diagram
{

SignalDelegateImpl
::SignalDelegateImpl(STATE_DIAGRAM_STRING_PARAM(_name))
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, m_isActive{false}
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
, m_isSet{false}
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
{
  // This space intentionally left empty
}

#ifndef STATE_DIAGRAM_STRINGLESS

void
SignalDelegateImpl
::pathPrefix(ostream &)
const
{
  assert (false); // Never to be called
}

#endif // STATE_DIAGRAM_STRINGLESS

bool
SignalDelegateImpl
::isActive()
const
{
  return m_isActive;
}

void
SignalDelegateImpl
::activate()
{
  m_isActive = true;
}

void
SignalDelegateImpl
::deactivate()
{
  m_isActive = false;
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  m_isSet = false;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
SignalDelegateImpl
::isSet()
const
{
  return m_isSet;
}

void
SignalDelegateImpl
::markAsSet()
{
  m_isSet = true;
}

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

}
