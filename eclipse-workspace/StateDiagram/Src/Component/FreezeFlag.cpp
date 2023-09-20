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

#include "Impl/SingleStateTransitionImpl.h"
#include "Impl/AutoTransitionImpl.h"
#include "Impl/StepTransitionImpl.h"

namespace state_diagram
{

FreezeFlag
::FreezeFlag(FreezeDepth const _freezeDepth)
:
  freezeDepth{_freezeDepth}
{
  // This space intentionally left empty
}

void
FreezeFlag
::join(Transition const * const transition)
const
{
  transition->implUpcast()->accept
  (
    [&]
    (
      SingleStateTransitionImpl * const
#ifndef STATE_DIAGRAM_STRINGLESS
        singleStateTransition
#endif // STATE_DIAGRAM_STRINGLESS
    )
#ifdef STATE_DIAGRAM_EXIT_ON_ERROR
        noexcept
#endif // STATE_DIAGRAM_EXIT_ON_ERROR
    {
#ifndef STATE_DIAGRAM_STRINGLESS
      throw FreezeFlag::FreezeFlagOnSingleStateTransitionError(singleStateTransition->origin()->path());
#else
      STATE_DIAGRAM_HANDLE_ERROR(FreezeFlag::freezeFlagOnSingleStateTransitionError);
#endif // STATE_DIAGRAM_STRINGLESS
    }
  , [&]
    (
      AutoTransitionImpl * const
#ifndef STATE_DIAGRAM_STRINGLESS
        autoTransition
#endif // STATE_DIAGRAM_STRINGLESS
    )
#ifdef STATE_DIAGRAM_EXIT_ON_ERROR
        noexcept
#endif // STATE_DIAGRAM_EXIT_ON_ERROR
    {
#ifndef STATE_DIAGRAM_STRINGLESS
      throw FreezeFlag::FreezeFlagOnAutoTransitionError(autoTransition->source->path(), autoTransition->target->path());
#else
      STATE_DIAGRAM_HANDLE_ERROR(FreezeFlag::freezeFlagOnSingleStateTransitionError);
#endif // STATE_DIAGRAM_STRINGLESS
    }
  , [&](StepTransitionImpl * const stepTransition)
    {
      stepTransition->setFreezeFlag(freezeDepth);
    }
  );
}

} // namespace state_diagram

