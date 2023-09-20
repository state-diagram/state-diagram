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

TEST(SimpleSetGet)
{
  try
  {
    FSM_TOP(top);
    FSM_SIGNAL(int, signal, top);

    signal.set(1);
    ASSERT(signal.get() == 1);
    ASSERT(signal.get() == 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(SetOnAlreadyActivated)
{
  try
  {
    FSM_TOP(top);

    FSM_LOCAL_SIGNAL(int, signal, top);

    FSM_INIT(top);
    FSM_CONNECTOR(state, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state, Output(signal));
    FSM_STEP(state, top_FINAL, Trigger(signal), Action([&](){signal.set(1);}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (SignalDelegate::SetOnActivatedError & err)
  {
    ASSERT_EQ(err.path, "top" + pathComponentSeparator + "signal");

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
TEST(GetOnNotSet)
{
  try
  {
    FSM_TOP(top);
    FSM_SIGNAL(int, signal, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state);
    FSM_STEP(state, top_FINAL, Trigger(signal), Action([&](){signal.get();}));

    signal.set(1);

    top.init();
    top.step();
    top.step(signal);

    ASSERT(false);
  }
  catch (SignalDelegate::GetOnNotSetError & err)
  {
    ASSERT_EQ(err.path, "signal");

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
TEST(SetOnAlreadySet)
{
  try
  {
    FSM_TOP(top);
    FSM_SIGNAL(int, signal, top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_FINAL, Action([&](){signal.set(1); signal.set(1);}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (SignalDelegate::SetOnAlreadySetError & err)
  {
    ASSERT_EQ(err.path, "signal");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

TEST(RegularSetGet)
{
  try
  {
    FSM_TOP(top);

    FSM_LOCAL_SIGNAL(int, signal, top);

    FSM_INIT(top);
    FSM_CONNECTOR(state1, top);
    FSM_CONNECTOR(state2, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state1, Output([&]() -> LocalEvent const & {return signal(1);}));
    int val1;
    FSM_STEP(state1, state2, Trigger(signal), Action([&](Event const & signal){val1 = signal.get<int>();}));
    int val2;
    FSM_STEP(state2, top_FINAL, Trigger(signal), Action([&](Event const & signal){val2 = signal.get<int>();}));

    top.init();
    top.step();

    ASSERT(val1 == 1);
    ASSERT(val2 == 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(SetDeactivateGetError)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(int, signal, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, state, Action([&](){signal.set(1);}));
    FSM_STEP(state, top_FINAL, Trigger(signal), Action([&](Event const & signal){signal.get<int>();}));

    top.init();
    top.step();
    top.step(signal);

    ASSERT(false);
  }
  catch (SignalDelegate::GetOnNotSetError & err)
  {
    ASSERT_EQ(err.path, "signal");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

