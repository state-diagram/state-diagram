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

CompoundState::RegionError::DefaultRegionRequest
::DefaultRegionRequest(string const & _compoundStatePath, size_t const _nrOfNonDefaultRegions)
:
  CompoundState::RegionError{_compoundStatePath}
, nrOfNonDefaultRegions{_nrOfNonDefaultRegions}
{
  // This space intentionally left empty
}

string
CompoundState::RegionError::DefaultRegionRequest
::specific()
const
{
  return
    string() +
    "Default region on compound state\n" +
    "\"" + compoundStatePath + "\"\n" +
    "requested. State in question already has " + to_string(nrOfNonDefaultRegions) + " directly inserted (i.e. non-default) region(s).";
}

} // namespace state_diagram

#endif // STATE_DIAGRAM_STRINGLESS
