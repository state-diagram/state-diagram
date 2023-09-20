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

#include "SubStateImpl.h"

#include <cassert>

namespace state_diagram
{

SubStateImpl
::SubStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(_name) RegionImpl * const _parent)
:
  SubComponent{STATE_DIAGRAM_STRING_ARG_COMMA(_name) _parent}
{
  // This space intentionally left empty
}

RegionImpl *
SubStateImpl
::parentRegion()
const
{
  return parent;
}

bool
SubStateImpl
::isDeepMemberOf(RegionImpl const * const region)
const
{
  if (region->isParentOf(this))
  {
    return true;
  }
  return parentRegion()->parentCompoundState()->isDeepMemberOf(region);
}

bool
SubStateImpl
::isTerminal()
const
{
  return false;
}

bool
SubStateImpl
::hasInScope(LocalSignalDelegateImpl const * const signal)
const
{
  return parentRegion()->hasInScope(signal);
}

void
SubStateImpl
::init()
{
  // This space intentionally left empty
}

void
SubStateImpl
::finalize()
const
{
  // This space intentionally left empty
}

void
SubStateImpl
::unfreeze()
{
  // This space intentionally left empty
}

void
SubStateImpl
::reload()
{
  // This space intentionally left empty
}

bool
SubStateImpl
::isCurrent()
const
{
  return parentRegion()->hasAsCurrent(this);
}

} // namespace std

