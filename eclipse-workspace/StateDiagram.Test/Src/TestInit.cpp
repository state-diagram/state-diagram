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

TEST(InitToStateToFinalWithExtendedNesting)
{
  try
  {
    bool sawT1{false};
    bool sawT2{false};
    bool sawT3{false};
    bool sawT4{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);
    Auto t1{top_INIT, state, Action([&](){sawT1 = true;})};
    Auto t2{state, top_FINAL, Action([&](){sawT2 = true;})};

    FSM_REGION(region1, state);
    FSM_INIT(region1);
    FSM_FINAL(region1);
    Auto t3{region1_INIT, region1_FINAL, Action([&](){sawT3 = true;})};

    FSM_REGION(region2, state);
    FSM_INIT(region2);
    FSM_FINAL(region2);
    Auto t4{region2_INIT, region2_FINAL, Action([&](){sawT4 = true;})};

    top.init();
    top.step();
    bool const stat{top.step()};
    ASSERT(sawT1);
    ASSERT(sawT2);
    ASSERT(sawT3);
    ASSERT(sawT4);
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(EmptyTop)
{
  try
  {
    FSM_TOP(top);
    top.init();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(MissingInit)
{
  try
  {
    FSM_TOP(top);
    FSM_REGION(region, top);
    top.init();
    ASSERT(false);
  }
  catch (Region::Error::MissingInit & err)
  {
    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(LoneInit)
{
  try
  {
    FSM_TOP(top);
    FSM_INIT(top);
    top.init();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

TEST(InitToFinal)
{
  try
  {
    FSM_TOP(top);
    FSM_INIT(top);
    FSM_FINAL(top);
    Auto t{top_INIT, top_FINAL};
    top.init();
    bool const stat{top.step()};
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(InitToState)
{
  try
  {
    FSM_TOP(top);
    FSM_INIT(top);
    FSM_STATE(state, top);
    Auto t{top_INIT, state};
    top.init();
    bool const stat{top.step()};
    ASSERT(!stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(InitToStateToFinalWithSignal)
{
  try
  {
    FSM_TOP(top);
    FSM_LOCAL_SIGNAL(void, signal, top);
    FSM_INIT(top);
    FSM_CONNECTOR(connector, top);
    FSM_FINAL(top);
    Auto t1{top_INIT, connector, Output(signal)};
    Step t2{connector, top_FINAL, Trigger(signal)};
    top.init();
    bool const stat{top.step()};
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(InitToStateToFinal)
{
  try
  {
    FSM_TOP(top);
    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);
    Auto t1{top_INIT, state};
    Auto t2{state, top_FINAL};
    top.init();
    top.step();
    bool const stat{top.step()};
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(InitToStateToStateToFinal)
{
  try
  {
    FSM_TOP(top);
    FSM_INIT(top);
    FSM_STATE(state1, top);
    FSM_STATE(state2, top);
    FSM_FINAL(top);
    Auto t1{top_INIT, state1};
    Auto t2{state1, state2};
    Auto t3{state2, top_FINAL};
    top.init();
    top.step();
    top.step();
    bool const stat{top.step()};
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(InitToStateToFinalWithNesting)
{
  try
  {
    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);
    Auto t1{top_INIT, state};
    Auto t2{state, top_FINAL};

    FSM_INIT(state);
    FSM_FINAL(state);
    Auto t3{state_INIT, state_FINAL};

    top.init();
    top.step();
    bool const stat{top.step()};
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(NestedInitAndBack)
{
  try
  {
    bool sawNested{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state);
    FSM_AUTO(state, top_FINAL);

    FSM_INIT(state);
    FSM_FINAL(state);

    FSM_AUTO(state_INIT, state_FINAL);
    state_INIT_TO_state_FINAL.add(Action([&](){sawNested = true;}));

    top.init();
    top.step();
    bool const stat{top.step()};

    ASSERT(sawNested);
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

