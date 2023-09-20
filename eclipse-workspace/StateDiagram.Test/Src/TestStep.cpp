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

TEST(DeadlockFromPrincipal)
{
  try
  {
    FSM_TOP(topState);
    FSM_SIGNAL(void, signal, topState);
    FSM_INIT(topState);
    FSM_STATE(state, topState);
    Auto t(topState_INIT, state);
    topState.init();
    bool const stat{topState.step(signal)};
    ASSERT(!stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(SingleStep)
{
  try
  {
    FSM_TOP(topState);
    FSM_SIGNAL(void, signal, topState);
    FSM_INIT(topState);
    FSM_CONNECTOR(state, topState);
    FSM_FINAL(topState);
    Auto t1{topState_INIT, state};
    Step t2{state, topState_FINAL, Trigger(signal)};
    topState.init();
    bool const stat{topState.step(signal)};
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DeadlockFromNestedPrincipal)
{
  try
  {
    bool sawT1{false};
    bool sawT2{false};

    FSM_TOP(topState);
    FSM_SIGNAL(void, signal, topState);

    FSM_INIT(topState);
    FSM_STATE(state1, topState);
    Auto t1(topState_INIT, state1, Action([&](){sawT1 = true;}));

    FSM_INIT(state1);
    FSM_STATE(state2, state1);
    Auto t2(state1_INIT, state2, Action([&](){sawT2 = true;}));

    topState.init();

    bool const stat1{topState.step(signal)};
    ASSERT(sawT1);
    ASSERT(sawT2);
    ASSERT(!stat1);

    sawT1 = false;
    sawT2 = false;

    bool const stat2{topState.step(signal)};
    ASSERT(!sawT1);
    ASSERT(!sawT2);
    ASSERT(!stat2);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TriggerPresentOrAbsent)
{
  try
  {
    bool sawT1{false};
    bool sawT2{false};

    FSM_TOP(topState);

    FSM_SIGNAL(void, signal1, topState);
    FSM_SIGNAL(void, signal2, topState);

    FSM_INIT(topState);
    FSM_CONNECTOR(state, topState);
    FSM_FINAL(topState);

    Auto t(topState_INIT, state);
    Step t1(state, topState_FINAL, Trigger(signal1), Action([&](){sawT1 = true;}));
    Step t2(state, topState_FINAL, Trigger(signal2), Action([&](){sawT2 = true;}));

    topState.init();
    bool const stat{topState.step(signal1)};

    ASSERT(sawT1);
    ASSERT(!sawT2);
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(GuardFalse)
{
  try
  {
    bool sawT2{false};

    FSM_TOP(topState);

    FSM_INIT(topState);
    FSM_STATE(state, topState);
    FSM_FINAL(topState);

    Auto t1(topState_INIT, state);
    Auto t2(state, topState_FINAL, Guard([](){return false;}), Action([&](){sawT2 = true;}));

    topState.init();
    bool const stat{topState.step()};

    ASSERT(!sawT2);
    ASSERT(!stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(InducedTriggerAcrossConcurrentRegions)
{
  try
  {
    bool sawT2{false};

    FSM_TOP(topState);

    FSM_SIGNAL(void, external, topState);

    FSM_LOCAL_SIGNAL(void, internal, topState);

    FSM_REGION(region1, topState);
    FSM_REGION(region2, topState);

    FSM_INIT(region1);
    FSM_STATE(state1, region1);
    FSM_FINAL(region1);

    FSM_AUTO(region1_INIT, state1);
    FSM_STEP(state1, region1_FINAL, Trigger(external));
    state1_TO_region1_FINAL.add(Output(internal));

    FSM_INIT(region2);
    FSM_STATE(state2, region2);
    FSM_FINAL(region2);

    FSM_AUTO(region2_INIT, state2);
    FSM_STEP(state2, region2_FINAL, Trigger(internal));
    state2_TO_region2_FINAL.add(Action([&](){sawT2 = true;}));;

    topState.init();
    topState.step();
    topState.step(external);

    ASSERT(sawT2);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(ResetTriggersToAbsent)
{
  try
  {
    FSM_TOP(topState);

    FSM_SIGNAL(void, external1, topState);
    FSM_SIGNAL(void, external2, topState);

    FSM_LOCAL_SIGNAL(void, internal, topState);

    FSM_INIT(topState);
    FSM_STATE(state1, topState);
    FSM_STATE(state2, topState);
    FSM_FINAL(topState);

    FSM_AUTO(topState_INIT, state1, Output(internal));

    FSM_STEP(state1, state2, Trigger(external2));

    FSM_STEP(state2, topState_FINAL, Trigger(internal));

    topState.init();
    topState.step(external1);
    bool const stat{topState.step(external2)};

    ASSERT(!stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(SimplestCompletion)
{
  try
  {
    FSM_TOP(topState);

    FSM_INIT(topState);
    FSM_STATE(state, topState);
    FSM_FINAL(topState);

    FSM_INIT(state);
    FSM_FINAL(state);

    FSM_AUTO(topState_INIT, state);
    FSM_AUTO(state, topState_FINAL);

    FSM_AUTO(state_INIT, state_FINAL);

    topState.init();
    topState.step();
    bool const stat{topState.step()};

    ASSERT(stat);
    ASSERT(!topState_INIT.isCurrent());
    ASSERT(!state.isCurrent());
    ASSERT(topState_FINAL.isCurrent());
    ASSERT(!state_INIT.isCurrent());
    ASSERT(state_FINAL.isCurrent());
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(CompletionAfterExternalExec)
try
{
  FSM_TOP(topState);

  FSM_SIGNAL(void, external, topState);

  FSM_INIT(topState);
  FSM_STATE(state, topState);
  FSM_FINAL(topState);

  FSM_INIT(state);
  FSM_STATE(nested, state);
  FSM_FINAL(state);

  FSM_AUTO(topState_INIT, state);
  FSM_AUTO(state, topState_FINAL, Guard([&](){return state.hasTerminated();}));

  FSM_AUTO(state_INIT, nested);
  FSM_STEP(nested, state_FINAL, Trigger(external));

  topState.init();
  bool const statInit{topState.step()};
  ASSERT(!statInit);
  ASSERT(!topState_INIT.isCurrent());
  ASSERT(state.isCurrent());
  ASSERT(!topState_FINAL.isCurrent());
  ASSERT(!state_INIT.isCurrent());
  ASSERT(nested.isCurrent());
  ASSERT(!state_FINAL.isCurrent());

  bool const statStep{topState.step(external)};
  ASSERT(statStep);
  ASSERT(!topState_INIT.isCurrent());
  ASSERT(!state.isCurrent());
  ASSERT(topState_FINAL.isCurrent());
  ASSERT(!state_INIT.isCurrent());
  ASSERT(!nested.isCurrent());
  ASSERT(state_FINAL.isCurrent());
}
catch (Error & err)
{
  cout << err.msg(); cout.flush();
  ASSERT(false);
}

TEST(StepFromInitial)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, signal, top);

    FSM_INIT(top);
    FSM_FINAL(top);

    bool sawStep{false};
    FSM_STEP(top_INIT, top_FINAL, Trigger(signal), Action([&](){sawStep = true;}));

    top.init();
    top.step(signal);

    ASSERT(sawStep);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
