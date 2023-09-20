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

TEST(FreezeFlagOnAutonomusTransition)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_STATE(principal, top);
    FSM_INTERNAL_STEP(principal, Trigger(global), FreezeFlag(FULL));

    ASSERT(false);
  }
  catch (FreezeFlag::FreezeFlagOnSingleStateTransitionError const & err)
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

TEST(FreezeFlagOnAutoTransition)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_INIT(top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, top_FINAL, FreezeFlag(SHALLOW));

    ASSERT(false);
  }
  catch (FreezeFlag::FreezeFlagOnAutoTransitionError const & err)
  {
    ASSERT_EQ(err.sourcePath, string() + "top" + pathComponentSeparator + "REGION" + pathComponentSeparator + "INIT");
    ASSERT_EQ(err.targetPath, string() + "top" + pathComponentSeparator + "REGION" + pathComponentSeparator + "FINAL");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(BasicFullFreeze)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global1, top);
    FSM_SIGNAL(void, global2, top);
    FSM_SIGNAL(void, global3, top);

    FSM_INIT(top);
    FSM_STATE(state, top);

    FSM_AUTO(top_INIT, state);
    FSM_STEP(state, state, Trigger(global1), FreezeFlag(FULL));

    FSM_INIT(state);
    FSM_STATE(state_state1, state);
    FSM_STATE(state_state2, state);

    FSM_AUTO(state_INIT, state_state1);
    FSM_STEP(state_state1, state_state2, Trigger(global2));
    FSM_STEP(state_state2, state_state1, Trigger(global3));

    bool stat;

    top.init();

    stat = top.step();
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());

    stat = top.step(global2);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state2.isCurrent());

    stat = top.step(global1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state2.isCurrent());
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(FullFreeze)
{
  try
  {
    bool state_enter{false};
    bool state_exit{false};

    bool state_state1_enter{false};
    bool state_state1_exit{false};
    bool state_state2_enter{false};
    bool state_state2_exit{false};

    bool state_state2_state1_enter{false};
    bool state_state2_state1_exit{false};
    bool state_state2_state2_enter{false};
    bool state_state2_state2_exit{false};

    auto const reset
    {
      [&]()
      {
        state_enter = false;
        state_exit = false;

        state_state1_enter = false;
        state_state1_exit = false;
        state_state2_enter = false;
        state_state2_exit = false;

        state_state2_state1_enter = false;
        state_state2_state1_exit = false;
        state_state2_state2_enter = false;
        state_state2_state2_exit = false;
      }
    };

    // Top level
    FSM_TOP(top);

    FSM_SIGNAL(void, global1, top);
    FSM_SIGNAL(void, global2_1, top);
    FSM_SIGNAL(void, global2_2, top);
    FSM_SIGNAL(void, global3_1, top);
    FSM_SIGNAL(void, global3_2, top);

    // Level 0
    FSM_INIT(top);

    FSM_STATE(state, top);
    FSM_ENTER(state, Action([&](){state_enter = true;}));
    FSM_EXIT(state, Action([&](){state_exit = true;}));

    FSM_AUTO(top_INIT, state);
    FSM_STEP(state, state, Trigger(global1), FreezeFlag(FULL), Max1Flag());

    // Level 1
    FSM_INIT(state);

    FSM_STATE(state_state1, state);
    FSM_ENTER(state_state1, Action([&](){state_state1_enter = true;}));
    FSM_EXIT(state_state1, Action([&](){state_state1_exit = true;}));

    FSM_STATE(state_state2, state);
    FSM_ENTER(state_state2, Action([&](){state_state2_enter = true;}));
    FSM_EXIT(state_state2, Action([&](){state_state2_exit = true;}));

    FSM_AUTO(state_INIT, state_state1);
    FSM_STEP(state_state1, state_state2, Trigger(global2_1), Max1Flag());
    FSM_STEP(state_state2, state_state1, Trigger(global2_2), Max1Flag());

    // Level 2
    FSM_INIT(state_state2);

    FSM_STATE(state_state2_state1, state_state2);
    FSM_ENTER(state_state2_state1, Action([&](){state_state2_state1_enter = true;}));
    FSM_EXIT(state_state2_state1, Action([&](){state_state2_state1_exit = true;}));

    FSM_STATE(state_state2_state2, state_state2);
    FSM_ENTER(state_state2_state2, Action([&](){state_state2_state2_enter = true;}));
    FSM_EXIT(state_state2_state2, Action([&](){state_state2_state2_exit = true;}));

    FSM_AUTO(state_state2_INIT, state_state2_state1);
    FSM_STEP(state_state2_state1, state_state2_state2, Trigger(global3_1), Max1Flag());
    FSM_STEP(state_state2_state2, state_state2_state1, Trigger(global3_2), Max1Flag());

    // Init

    top.init();

    // Exec
    bool stat;

    stat = top.step();
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());
    ASSERT(!state_state2.isCurrent());
    ASSERT(!state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(state_enter);
    ASSERT(!state_exit);
    ASSERT(state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global2_1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!state_state1.isCurrent());
    ASSERT(state_state2.isCurrent());
    ASSERT(state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(state_state1_exit);
    ASSERT(state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global3_1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!state_state1.isCurrent());
    ASSERT(state_state2.isCurrent());
    ASSERT(!state_state2_state1.isCurrent());
    ASSERT(state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(state_state2_state1_exit);
    ASSERT(state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!state_state1.isCurrent());
    ASSERT(state_state2.isCurrent());
    ASSERT(!state_state2_state1.isCurrent());
    ASSERT(state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global2_2);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());
    ASSERT(!state_state2.isCurrent());
    ASSERT(!state_state2_state1.isCurrent());
    ASSERT(state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(state_state2_state2_exit);

    reset();

    stat = top.step(global3_2);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());
    ASSERT(!state_state2.isCurrent());
    ASSERT(!state_state2_state1.isCurrent());
    ASSERT(state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());
    ASSERT(!state_state2.isCurrent());
    ASSERT(!state_state2_state1.isCurrent());
    ASSERT(state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global2_1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!state_state1.isCurrent());
    ASSERT(state_state2.isCurrent());
    ASSERT(state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(state_state1_exit);
    ASSERT(state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TrivialShallowFreeze)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global1, top);
    FSM_SIGNAL(void, global2_1, top);
    FSM_SIGNAL(void, global2_2, top);

    FSM_INIT(top);
    FSM_STATE(state, top);

    FSM_AUTO(top_INIT, state);
    FSM_STEP(state, state, Trigger(global1), FreezeFlag(SHALLOW), Max1Flag());

    FSM_INIT(state);
    FSM_STATE(state_state1, state);
    FSM_STATE(state_state2, state);

    FSM_AUTO(state_INIT, state_state1);
    FSM_STEP(state_state1, state_state2, Trigger(global2_1), Max1Flag());
    FSM_STEP(state_state2, state_state1, Trigger(global2_2), Max1Flag());

    top.init();

    bool stat;

    stat = top.step();
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());

    stat = top.step(global2_1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state2.isCurrent());

    stat = top.step(global1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state2.isCurrent());
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(ShallowFreeze)
{
  try
  {
    bool state_enter{false};
    bool state_exit{false};

    bool state_state1_enter{false};
    bool state_state1_exit{false};
    bool state_state2_enter{false};
    bool state_state2_exit{false};

    bool state_state2_state1_enter{false};
    bool state_state2_state1_exit{false};
    bool state_state2_state2_enter{false};
    bool state_state2_state2_exit{false};

    auto const reset
    {
      [&]()
      {
        state_enter = false;
        state_exit = false;

        state_state1_enter = false;
        state_state1_exit = false;
        state_state2_enter = false;
        state_state2_exit = false;

        state_state2_state1_enter = false;
        state_state2_state1_exit = false;
        state_state2_state2_enter = false;
        state_state2_state2_exit = false;
      }
    };

    // Top level
    FSM_TOP(top);

    FSM_SIGNAL(void, global1, top);
    FSM_SIGNAL(void, global2_1, top);
    FSM_SIGNAL(void, global2_2, top);
    FSM_SIGNAL(void, global3_1, top);
    FSM_SIGNAL(void, global3_2, top);

    // Level 0
    FSM_INIT(top);

    FSM_STATE(state, top);
    FSM_ENTER(state, Action([&](){state_enter = true;}));
    FSM_EXIT(state, Action([&](){state_exit = true;}));

    FSM_AUTO(top_INIT, state);
    FSM_STEP(state, state, Trigger(global1), FreezeFlag(SHALLOW), Max1Flag());

    // Level 1
    FSM_INIT(state);

    FSM_STATE(state_state1, state);
    FSM_ENTER(state_state1, Action([&](){state_state1_enter = true;}));
    FSM_EXIT(state_state1, Action([&](){state_state1_exit = true;}));

    FSM_STATE(state_state2, state);
    FSM_ENTER(state_state2, Action([&](){state_state2_enter = true;}));
    FSM_EXIT(state_state2, Action([&](){state_state2_exit = true;}));

    FSM_AUTO(state_INIT, state_state1);
    FSM_STEP(state_state1, state_state2, Trigger(global2_1), Max1Flag());
    FSM_STEP(state_state2, state_state1, Trigger(global2_2), Max1Flag());

    // Level 2
    FSM_INIT(state_state2);

    FSM_STATE(state_state2_state1, state_state2);
    FSM_ENTER(state_state2_state1, Action([&](){state_state2_state1_enter = true;}));
    FSM_EXIT(state_state2_state1, Action([&](){state_state2_state1_exit = true;}));

    FSM_STATE(state_state2_state2, state_state2);
    FSM_ENTER(state_state2_state2, Action([&](){state_state2_state2_enter = true;}));
    FSM_EXIT(state_state2_state2, Action([&](){state_state2_state2_exit = true;}));

    FSM_AUTO(state_state2_INIT, state_state2_state1);
    FSM_STEP(state_state2_state1, state_state2_state2, Trigger(global3_1), Max1Flag());
    FSM_STEP(state_state2_state2, state_state2_state1, Trigger(global3_2), Max1Flag());

    // Init

    top.init();

    // Exec
    bool stat;

    stat = top.step();
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());
    ASSERT(!state_state2.isCurrent());
    ASSERT(!state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(state_enter);
    ASSERT(!state_exit);
    ASSERT(state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global2_1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!state_state1.isCurrent());
    ASSERT(state_state2.isCurrent());
    ASSERT(state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(state_state1_exit);
    ASSERT(state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global3_1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!state_state1.isCurrent());
    ASSERT(state_state2.isCurrent());
    ASSERT(!state_state2_state1.isCurrent());
    ASSERT(state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(state_state2_state1_exit);
    ASSERT(state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!state_state1.isCurrent());
    ASSERT(state_state2.isCurrent());
    ASSERT(state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(state_state2_enter);
    ASSERT(state_state2_exit);
    ASSERT(state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(state_state2_state2_exit);

    reset();

    stat = top.step(global2_2);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());
    ASSERT(!state_state2.isCurrent());
    ASSERT(state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global3_2);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());
    ASSERT(!state_state2.isCurrent());
    ASSERT(state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(!state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(state_state1.isCurrent());
    ASSERT(!state_state2.isCurrent());
    ASSERT(state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(state_state1_enter);
    ASSERT(state_state1_exit);
    ASSERT(!state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(!state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);

    reset();

    stat = top.step(global2_1);
    ASSERT(!stat);
    ASSERT(state.isCurrent());
    ASSERT(!state_state1.isCurrent());
    ASSERT(state_state2.isCurrent());
    ASSERT(state_state2_state1.isCurrent());
    ASSERT(!state_state2_state2.isCurrent());
    ASSERT(!state_enter);
    ASSERT(!state_exit);
    ASSERT(!state_state1_enter);
    ASSERT(state_state1_exit);
    ASSERT(state_state2_enter);
    ASSERT(!state_state2_exit);
    ASSERT(state_state2_state1_enter);
    ASSERT(!state_state2_state1_exit);
    ASSERT(!state_state2_state2_enter);
    ASSERT(!state_state2_state2_exit);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

