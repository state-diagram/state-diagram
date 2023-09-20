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

#include "Impl/NamePathImpl.h"

namespace state_diagram
{

NamePath
::NamePath(NamePathImpl * const impl)
:
  PImplUpcast<NamePathImpl>{impl}
{
  // This space intentionally left empty
}

#ifndef STATE_DIAGRAM_STRINGLESS

string const &
NamePath
::name()
const
{
  return m_implUpcast->name;
}

void
NamePath
::path(ostream & to)
const
{
  m_implUpcast->path(to);
}

string
NamePath
::path()
const
{
  return m_implUpcast->path();
}

#endif // STATE_DIAGRAM_SRINGLESS

} // namespace state_diagram
