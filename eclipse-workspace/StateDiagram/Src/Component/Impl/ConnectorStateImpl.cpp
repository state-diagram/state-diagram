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

#include "ConnectorStateImpl.h"

#include <cassert>

#include "TopStateImpl.h"

namespace state_diagram
{

ConnectorStateImpl
::ConnectorStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) CompoundStateImpl * const _parent)
:
  ConnectorStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent->defaultRegion()}
{
  // This space intentionally left empty
}

ConnectorStateImpl
::ConnectorStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) RegionImpl * const _parent)
:
  NamePathImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name)}
, SubStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
, SourceStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
, TargetStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
{
  parentRegion()->insertSubState(STATE_DIAGRAM_STRING_ARG_COMMA(name) this);
}

} // namespace state_diagram
