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

#include "state_diagram/state_diagram_payload.hpp"

TEST(PayloadGet)
{
  try
  {
    Payload<int, char, float> p{0, 'a', 0.0};

    ASSERT_EQ(p.get<int>(0), 0);
    ASSERT_EQ(p.get<char>(1), 'a');
    ASSERT_EQ(p.get<float>(2), 0.0);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(PayloadTypeMismatchErrorLast)
{
  try
  {
    Payload<int, char, float> p{0, 'a', 0.0};

    p.get<int>(3);

    ASSERT(false);
  }
  catch (PayloadBase<float>::TypeMismatchError & err)
  {
    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(PayloadTypeMismatchErrorMid)
{
  try
  {
    Payload<int, char, float> p{0, 'a', 0.0};

    p.get<int>(1);

    ASSERT(false);
  }
  catch (PayloadBase<char>::TypeMismatchError & err)
  {
    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(PayloadIndexationError)
{
  try
  {
    Payload<int, char, float> p{0, 'a', 0.0};

    p.get<float>(3);

    ASSERT(false);
  }
  catch (PayloadBase<float>::IndexationError & err)
  {
    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(PayloadOnSignalExternal)
{
  try
  {
    int val;

    using SignalPayload = Payload<int, char, float>;

    FSM_TOP(top);
    ExternalSignal<SignalPayload> signal("signal", top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_STEP(top_INIT, top_FINAL, Trigger(signal), Action([&](Event const & trigger){val = trigger.get<SignalPayload>().get<0>();}));

    top.init();
    top.step(signal({1, 'a', 1.0}));
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(PayloadOnSignalLocal)
{
  try
  {
    int val;

    FSM_TOP(top);
    LocalSignal<int, char, float> signal("signal", top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_CONNECTOR(state, top);
    Auto top_INIT_TO_state(top_INIT, state, Output(signal(1, 'a', 1.0F)));
    FSM_STEP(state, top_FINAL, Trigger(signal), Action([&](Event const & trigger){val = trigger.get<int, char, float>().get<0>();}));

    top.init();
    top.step();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TypedSignal)
{
  try
  {
    FSM_TOP(top);
    ExternalSignal<int> signal("signal", top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_STEP(top_INIT, top_FINAL, Trigger(signal), Action([&](Event const & trigger){printf("%d\n", trigger.get<int>());}));

    top.init();
    top.step(signal(1));
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TupleTypedSignal1External)
{
  try
  {
    int val;

    FSM_TOP(top);
    ExternalSignal<int, char, float> signal("signal", top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_STEP(top_INIT, top_FINAL, Trigger(signal), Action([&](Event const & trigger){val = trigger.get<Payload<int, char, float>>().get<0>();}));

    top.init();
    top.step(signal(1, 'a', 1.0));

    ASSERT_EQ(val, 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TupleTypedSignal1Local)
{
  try
  {
    int val;

    FSM_TOP(top);
    LocalSignal<int, char, float> signal("signal", top);

    FSM_INIT(top);
    FSM_CONNECTOR(state, top);
    FSM_FINAL(top);
    Auto top_INIT_TO_state(top_INIT, state, Output(signal(1, 'a', 1.0F)));
    FSM_STEP(state, top_FINAL, Trigger(signal), Action([&](Event const & trigger){val = trigger.get<int, char, float>().get<0>();}));

    top.init();
    top.step();

    ASSERT_EQ(val, 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TupleTypedSignal2External)
{
  try
  {
    int val;

    FSM_TOP(top);
    ExternalSignal<int, char, float> signal("signal", top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_STEP(top_INIT, top_FINAL, Trigger(signal), Action([&](Event const & trigger){val = trigger.get<int, char, float>().get<0>();}));

    top.init();
    top.step(signal(1, 'a', 1.0));

    ASSERT_EQ(val, 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TupleTypedSignal2Local)
{
  try
  {
    int val;

    FSM_TOP(top);
    LocalSignal<int, char, float> signal("signal", top);

    FSM_INIT(top);
    FSM_CONNECTOR(state, top);
    FSM_FINAL(top);
    Auto top_INIT_TO_state(top_INIT, state, Output(signal(1, 'a', 1.0F)));
    FSM_STEP(state, top_FINAL, Trigger(signal), Action([&](Event const & trigger){val = trigger.get<int, char, float>().get<0>();}));

    top.init();
    top.step();

    ASSERT_EQ(val, 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TupleTypedSignal2TTExternal)
{
  try
  {
    int val;

    FSM_TOP(top);
    FSM_SIGNAL(FSM_TT(int, char, float), signal, top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_STEP(top_INIT, top_FINAL, Trigger(signal), Action([&](Event const & trigger){val = trigger.get<int, char, float>().get<0>();}));

    top.init();
    top.step(signal(1, 'a', 1.0));

    ASSERT_EQ(val, 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TupleTypedSignal2TTLocal)
{
  try
  {
    int val;

    FSM_TOP(top);
    FSM_LOCAL_SIGNAL(FSM_TT(int, char, float), signal, top);

    FSM_INIT(top);
    FSM_CONNECTOR(state, top);
    FSM_FINAL(top);
    Auto top_INIT_TO_state(top_INIT, state, Output(signal(1, 'a', 1.0F)));
    FSM_STEP(state, top_FINAL, Trigger(signal), Action([&](Event const & trigger){val = trigger.get<int, char, float>().get<0>();}));

    top.init();
    top.step();

    ASSERT_EQ(val, 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}


