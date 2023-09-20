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

#include "Impl/EnterTransitionImpl.h"
#include "Impl/ExitTransitionImpl.h"
#include "Impl/InternalAutoTransitionImpl.h"
#include "Impl/InternalStepTransitionImpl.h"
#include "Impl/AutoTransitionImpl.h"
#include "Impl/StepTransitionImpl.h"

namespace state_diagram
{

void
Max1Flag
::join(Transition const * const transition)
const
{
  transition->implUpcast()->accept
  (
    [&](SingleStateTransitionImpl * const singleStateTransition)
    {
      singleStateTransition->accept
      (
        [&]
        (
          EnterTransitionImpl * const
#ifndef STATE_DIAGRAM_STRINGLESS
            enterTransition
#endif // STATE_DIAGRAM_STRINGLESS
        )
#ifdef STATE_DIAGRAM_EXIT_ON_ERROR
        noexcept
#endif // STATE_DIAGRAM_EXIT_ON_ERROR
        {
#ifndef STATE_DIAGRAM_STRINGLESS
          throw Max1Flag::Max1FlagOnBoundaryTransitionError(enterTransition->origin()->path());
#else
          STATE_DIAGRAM_HANDLE_ERROR(Max1Flag::max1FlagOnBoundaryTransitionError);
#endif // STATE_DIAGRAM_STRINGLESS
        }
      , [&]
        (
          ExitTransitionImpl * const
#ifndef STATE_DIAGRAM_STRINGLESS
            exitTransition
#endif // STATE_DIAGRAM_STRINGLESS
        )
#ifdef STATE_DIAGRAM_EXIT_ON_ERROR
        noexcept
#endif // STATE_DIAGRAM_EXIT_ON_ERROR
        {
#ifndef STATE_DIAGRAM_STRINGLESS
          throw Max1Flag::Max1FlagOnBoundaryTransitionError(exitTransition->origin()->path());
#else
          STATE_DIAGRAM_HANDLE_ERROR(Max1Flag::max1FlagOnBoundaryTransitionError);
#endif // STATE_DIAGRAM_STRINGLESS
        }
      , [&](InternalAutoTransitionImpl * const internalAutoTransition)
        {
          internalAutoTransition->setMax1Flag();
        }
      , [&](InternalStepTransitionImpl * const internalStepTransition)
        {
          internalStepTransition->setMax1Flag();
        }
      );
    }
  , [&](AutoTransitionImpl * const autoTransition)
    {
      autoTransition->setMax1Flag();
    }
  , [&](StepTransitionImpl * const stepTransition)
    {
      stepTransition->setMax1Flag();
    }
  );
}

} // namespace state_diagram

