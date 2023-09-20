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

#include "FinalStateImpl.h"

#include "CompoundStateImpl.h"

namespace state_diagram
{

FinalStateImpl
::FinalStateImpl(CompoundStateImpl * const _parent)
:
  FinalStateImpl{_parent->defaultRegion()}
{
  // This space intentionally left empty
}

FinalStateImpl
::FinalStateImpl(RegionImpl * const _parent)
:
   NamePathImpl{STATE_DIAGRAM_STRING_ARG(Final::name)}
 , SubStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(Final::name) _parent}
 , TargetStateImpl{STATE_DIAGRAM_STRING_ARG_COMMA(Final::name) _parent}
{
  parent->insertSubState(STATE_DIAGRAM_STRING_ARG_COMMA(name) this);
}

bool
FinalStateImpl
::isTerminal()
const
{
  return true;
}

ExecStat
FinalStateImpl
::exec()
const
{
  return ExecStat{};
}

} // namespace state_diagram
