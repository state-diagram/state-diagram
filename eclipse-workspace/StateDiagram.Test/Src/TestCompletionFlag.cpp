/*   
   Copyright 2019-2020 State Diagram Contributors

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

#include "StateDiagramTestSetup.h"

TEST(CompletionOnInternalStepTransition)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_STATE(principal, top);
    FSM_INTERNAL_STEP(principal, Trigger(global), CompletionFlag());

    ASSERT(false);
  }
  catch (CompletionFlag::CompletionFlagOnSingleStateTransitionError const & err)
  {
    ASSERT_EQ(err.hostPath, string() + "top" + pathComponentSeparator + "REGION" + pathComponentSeparator + "principal");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(CompletionFlagOnAutoTransition)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state);
    FSM_AUTO(state, top_FINAL, CompletionFlag());

    FSM_INIT(state);
    FSM_STATE(state_nested, state);
    FSM_FINAL(state);

    FSM_AUTO(state_INIT, state_nested);
    FSM_STEP(state_nested, state_FINAL, Trigger(global));

    bool stat;

    top.init();

    stat = top.step();

    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_nested.isCurrent());

    stat = top.step(global);
    ASSERT(stat);
    ASSERT(!state.isCurrent());
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(CompletionFlagOnStepTransition)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state);
    FSM_STEP(state, top_FINAL, Trigger(global), CompletionFlag());

    FSM_INIT(state);
    FSM_STATE(state_nested, state);
    FSM_FINAL(state);

    FSM_AUTO(state_INIT, state_nested);
    FSM_STEP(state_nested, state_FINAL, Trigger(global));

    bool stat;

    top.init();

    stat = top.step();

    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_nested.isCurrent());

    stat = top.step(global);
    ASSERT(stat);
    ASSERT(!state.isCurrent());
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}


