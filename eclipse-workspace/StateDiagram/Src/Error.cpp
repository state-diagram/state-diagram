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

#ifndef STATE_DIAGRAM_STRINGLESS

namespace
{
  char const genericMsg[]{"State Diagram user error"};
}

#endif // STATE_DIAGRAM_STRINGLESS

namespace state_diagram
{

using namespace std;

#ifndef STATE_DIAGRAM_STRINGLESS

char const *
Error
::what()
const noexcept
{
  return genericMsg;
}

string
Error
::msg()
const
{
  return
    string() +
    "\n" +
    "========================================================================================================================\n" +
    "State Diagram error:\n"
    "\n" +
    specific() + "\n"
    "========================================================================================================================\n" +
    "\n";
}

#else

#ifndef STATE_DIAGRAM_EXIT_ON_ERROR

Error
::Error(int const _code)
:
  code{_code}
{
  // This space intentionally left empty
}

#endif // STATE_DIAGRAM_EXIT_ON_ERROR

#endif // STATE_DIAGRAM_STRINGLESS

} // namespace state_diagram




