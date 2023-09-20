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

TEST(SimplestExit)
{
  try
  {
    bool sawE{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_EXIT(state, Action([&](){sawE = true;}));
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state);
    FSM_AUTO(state, top_FINAL);

    top.init();
    top.step();
    bool const stat{top.step()};

    ASSERT(sawE);
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DoubleExit)
{
  try
  {
    bool sawE1{false};
    bool sawE2{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    Exit e1{state, Action([&](){sawE1 = true;})};
    Exit e2{state, Action([&](){sawE2 = true;})};
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state);
    FSM_AUTO(state, top_FINAL);

    top.init();
    top.step();
    bool const stat{top.step()};

    ASSERT(sawE1);
    ASSERT(sawE2);
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(CascadedExit)
{
  try
  {
    bool sawE1{false};
    bool sawE2{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state1, top);
    FSM_EXIT(state1, Action([&](){sawE1 = true;}));
    FSM_FINAL(top);

    FSM_INIT(state1);
    FSM_STATE(state2, state1);
    FSM_EXIT(state2, Action([&](){sawE2 = true;}));
    FSM_FINAL(state1);

    FSM_AUTO(top_INIT, state1);
    FSM_AUTO(state1, top_FINAL);

    FSM_AUTO(state1_INIT, state2);

    top.init();
    top.step();
    bool const stat{top.step()};

    ASSERT(sawE1);
    ASSERT(sawE2);
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DeepExit)
{
  try
  {
    bool sawE1{false};
    bool sawE2{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state1, top);
    FSM_EXIT(state1, Action([&](){sawE1 = true;}));
    FSM_FINAL(top);

    FSM_INIT(state1);
    FSM_STATE(state2, state1);
    FSM_EXIT(state2, Action([&](){sawE2 = true;}));
    FSM_FINAL(state1);

    FSM_AUTO(top_INIT, state1);
    FSM_AUTO(state1_INIT, state2);
    FSM_AUTO(state2, top_FINAL);

    top.init();
    top.step();
    bool const stat{top.step()};

    ASSERT(sawE1);
    ASSERT(sawE2);
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(ExitOnLoop)
{
  try
  {
    bool sawE{false};

    FSM_TOP(top);

    FSM_SIGNAL(void, x, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_EXIT(state, Action([&](){sawE = true;}));

    FSM_AUTO(top_INIT, state);
    FSM_STEP(state, state, Trigger(x), Max1Flag());

    top.init();

    bool const stat1{top.step()};

    ASSERT(!stat1);

    sawE = false;

    bool const stat2{top.step(x)};

    ASSERT(sawE);
    ASSERT(!stat2);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DeepCascadedExit)
{
  try
  {
    bool sawE1{false};
    bool sawE2{false};
    bool sawE3{false};
    bool sawE4{false};

    FSM_TOP(top);

    FSM_INIT(top);
    FSM_STATE(state1, top);
    FSM_EXIT(state1, Action([&](){sawE1 = true;}));
    FSM_FINAL(top);

    FSM_REGION(region1, state1);
    FSM_REGION(region2, state1);

    FSM_INIT(region1);
    FSM_STATE(state2, region1);
    FSM_EXIT(state2, Action([&](){sawE2 = true;}));

    FSM_INIT(region2);
    FSM_STATE(state3, region2);
    FSM_EXIT(state3, Action([&](){sawE3 = true;}));

    FSM_INIT(state2);
    FSM_STATE(state4, state2);
    FSM_EXIT(state4, Action([&](){sawE4 = true;}));

    FSM_AUTO(top_INIT, state1);
    FSM_AUTO(region1_INIT, state2);
    FSM_AUTO(region2_INIT, state3);
    FSM_AUTO(state2_INIT, state4);

    FSM_AUTO(state2, top_FINAL);

    top.init();
    top.step();
    bool const stat{top.step()};

    ASSERT(sawE1);
    ASSERT(sawE2);
    ASSERT(sawE3);
    ASSERT(sawE4);
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(ExitOutputScopeError)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(void, i, region1);

    FSM_STATE(state, region2);
    FSM_EXIT(state, Output(i));

    ASSERT(false);
  }
  catch (Transition::ScopeError::Output const & err)
  {
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2" + pathComponentSeparator + "state");
    ASSERT_EQ(err.signalPath, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "i");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

