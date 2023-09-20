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

#include "StackSeq_all.h"

#include "../RegionImpl.h"

namespace state_diagram
{

StackSeq::Construct::Continue
::Continue
(
  RegionImpl * const _subjectRegion
, StackSeq::Construct const * const _next
)
:
  subjectRegion{_subjectRegion}
, next{_next}
{
  // This space intentionally left empty
}

void
StackSeq::Construct::Continue
::accept
(
  function<void (StackSeq::Construct::Continue const * const)> const onContinue
, function<void (StackSeq::Construct::Halt const * const)> const
)
const
{
  onContinue(this);
}

void
StackSeq::Construct::Continue
::exec()
const
{
  subjectRegion->descendToTarget(next.get());
}

} // namespace state_diagram
