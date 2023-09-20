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

#include "VarDelegateImpl.h"

#include <cassert>

namespace state_diagram
{

VarDelegateImpl
::VarDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) VarDelegate * const interfaceUpcast)
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, m_interfaceUpcast{interfaceUpcast}
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
, m_isValid{false}
, m_isSet{false}
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
, m_isSetNxt{false}
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
, m_hasBeenRetrieved{false}
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
{
  // This space intentionally left empty
}

#ifndef STATE_DIAGRAM_STRINGLESS

void
VarDelegateImpl
::pathPrefix(ostream &)
const
{
  assert (false); // Never to be called
}

#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
VarDelegateImpl
::isValid()
const
{
  return m_isValid;
}

void
VarDelegateImpl
::markAsValid()
{
  m_isValid = true;
}

bool
VarDelegateImpl
::isSet()
const
{
  return m_isSet;
}

void
VarDelegateImpl
::markAsSet()
{
  m_isSet = true;
}

bool
VarDelegateImpl
::isSetNxt()
const
{
  return m_isSetNxt;
}

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

void
VarDelegateImpl
::markAsSetNxt()
{
  m_isSetNxt = true;
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
VarDelegateImpl
::hasBeenRetrieved()
const
{
  return m_hasBeenRetrieved;
}

void
VarDelegateImpl
::markAsRetrieved()
{
  m_hasBeenRetrieved = true;
}

void
VarDelegateImpl
::unset()
{
  if (m_isSetNxt)
  {
    m_interfaceUpcast->makeNxtCur();
    markAsValid();
    markAsSet();
  }
  else
  {
    m_isSet = false;
  }
  m_isSetNxt = false;
  m_hasBeenRetrieved = false;
}

#else

void
VarDelegateImpl
::unset()
{
  if (m_isSetNxt)
  {
    m_interfaceUpcast->makeNxtCur();
  }
  m_isSetNxt = false;
}

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

}

