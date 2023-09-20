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

TEST(TriggerScopeErrorOnInternalStep)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(void, local, region1);

    FSM_STATE(state, region2);
    FSM_INTERNAL_STEP(state, Trigger(local));

    ASSERT(false);
  }
  catch (TriggeredTransition::TriggerScopeError & err)
  {
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2");
    ASSERT_EQ(err.signalPath, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "local");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(OutputScopeErrorOnInternalStep)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(void, local, region1);

    FSM_STATE(state, region2);
    FSM_INTERNAL_STEP(state, Trigger(global), Output(local));

    ASSERT(false);
  }
  catch (TriggeredTransition::ScopeError::Output & err)
  {
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2" + pathComponentSeparator + "state");
    ASSERT_EQ(err.signalPath, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "local");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(NestedInternalStep1)
{
  try
  {
    bool sawInternal{false};
    bool sawStep{false};

    FSM_TOP(top);
    FSM_SIGNAL(void, global, top);
    FSM_LOCAL_SIGNAL(void, local, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_INTERNAL_STEP(state, Trigger(local), Action([&](){sawInternal = true;}), Max1Flag());
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state, Output(local));
    FSM_STEP(state, top_FINAL, Trigger(global), Output(local), Action([&](){sawStep = true;}));

    top.init();

    bool stat;

    stat = top.step();
    ASSERT(!stat);
    ASSERT(sawInternal);
    ASSERT(!sawStep);

    sawInternal = false;

    stat = top.step(global);
    ASSERT(stat);
    ASSERT(!sawInternal);
    ASSERT(sawStep);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(NestedInternalStep2)
try
{
  bool sawInternal{false};
  bool sawStep{false};

  FSM_TOP(top);
  FSM_LOCAL_SIGNAL(void, local, top);

  FSM_INIT(top);
  FSM_STATE(state, top);
  FSM_INTERNAL_STEP(state, Trigger(local), Action([&](){sawInternal = true;}), Max1Flag());

  FSM_INIT(state);
  FSM_FINAL(state);

  FSM_AUTO(top_INIT, state);
  FSM_AUTO(state_INIT, state_FINAL, Output(local), Action([&](){sawStep = true;}));

  top.init();

  bool stat;

  stat = top.step();
  ASSERT(!stat);
  ASSERT(sawInternal);
  ASSERT(sawStep);

  sawInternal = false;
  sawStep = false;

  stat = top.step();
  ASSERT(!stat);
  ASSERT(!sawInternal);
  ASSERT(!sawStep);
}
catch (Error & err)
{
  cout << err.msg(); cout.flush();
  ASSERT(false);
}





