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

#include "NamePathImpl.h"

#ifndef STATE_DIAGRAM_STRINGLESS
#include <sstream>
#endif // STATE_DIAGRAM_STRINGLESS

namespace state_diagram
{

NamePathImpl
::NamePathImpl(STATE_DIAGRAM_STRING_PARAM(_name))
#ifndef STATE_DIAGRAM_STRINGLESS
:
  name{_name}
#endif // STATE_DIAGRAM_STRINGLESS
{
  // This space intentionally left empty
}

NamePathImpl
::~NamePathImpl()
{
  // This space intentionally left empty
}

#ifndef STATE_DIAGRAM_STRINGLESS
void
NamePathImpl
::pathPrefix(ostream & to)
const
{
  path(to);
  to << pathComponentSeparator;
}

string
NamePathImpl
::path()
const
{
  stringstream ss;
  path(ss);
  return ss.str();
}
#endif // STATE_DIAGRAM_STRINGLESS

} // namespace state_diagram


