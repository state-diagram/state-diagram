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

namespace state_diagram
{

Top::RequestForCurLocalScopeError
::RequestForCurLocalScopeError(string const & _localVarPath)
:
  localVarPath{_localVarPath}
{
  // This space intentionally left empty
}

string
Top::RequestForCurLocalScopeError
::specific()
const
{
  return
    string() +
    "Attempt to access as-yet undefined current local scope in using\n" +
    "local variable \"" + localVarPath + "\".";
}

} // namespace state_diagram

#endif // STATE_DIAGRAM_STRINGLESS
