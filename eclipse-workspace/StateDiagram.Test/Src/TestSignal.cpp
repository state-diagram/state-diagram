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

TEST(SimpleSignalSetGet)
{
  try
  {
    FSM_TOP(top);
    FSM_SIGNAL(int, signal, top);

    signal.set(1);

    ASSERT(signal.get() == 1);
    ASSERT(signal.get() == 1);

    signal.set(0);
    signal.set(2);

    ASSERT(signal.get() == 2);
    ASSERT(signal.get() == 2);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(SignalSetOnAlreadySet)
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
  catch (SignalDelegate::SetOnAlreadySetError const & err)
  {
    ASSERT_EQ(err.path, "signal");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(SignalGetOnNotSet)
{
  try
  {
    FSM_TOP(top);
    FSM_SIGNAL(int, signal, top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_STEP(top_INIT, top_FINAL, Trigger(signal), Action([&](){signal.get();}));

    signal.set(1);

    top.init();
    top.step();
    top.step(signal);

    ASSERT(false);
  }
  catch (SignalDelegate::GetOnNotSetError const & err)
  {
    ASSERT_EQ(err.path, "signal");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(SignalScopeErrorOnAuto)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(int, signal, region1);
    FSM_INIT(region1);
    FSM_FINAL(region1);
    FSM_AUTO(region1_INIT, region1_FINAL);

    FSM_INIT(region2);
    FSM_FINAL(region2);
    FSM_AUTO(region2_INIT, region2_FINAL, Action([&](){signal.set(1);}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (SignalDelegate::ScopeError const & err)
  {
    ASSERT_EQ(err.path, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal");
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(SignalScopeErrorOnSpec)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(int, signal1, region1);
    FSM_INIT(region1);
    FSM_FINAL(region1);
    FSM_AUTO(region1_INIT, region1_FINAL);

    FSM_LOCAL_SIGNAL(void, signal2, region2);
    FSM_INIT(region2);
    FSM_CONNECTOR(region2_STATE, region2);
    FSM_FINAL(region2);
    FSM_AUTO(region2_INIT, region2_STATE, Output(signal2));
    FSM_STEP(region2_STATE, region2_FINAL, Trigger(signal2), Action([&](){signal1.set(1);}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (SignalDelegate::ScopeError const & err)
  {
    ASSERT_EQ(err.path, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal1");
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

TEST(SignalExternalNameClashError)
{
  try
  {
    FSM_TOP(top);

    ExternalSignal<void> signal1("signal", top);
    ExternalSignal<void> signal2("signal", top);

    ASSERT(false);
  }
  catch (ExternalSignalDelegate::NameClashError const & err)
  {
    ASSERT_EQ(err.signalName, "signal");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(SignalNameClashErrorInsideRegion)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region, top);

    LocalSignal<void> signal1("signal", region);
    LocalSignal<void> signal2("signal", region);

    ASSERT(false);
  }
  catch (Region::LocalSignalNameClashError const & err)
  {
    ASSERT_EQ(err.signalName, "signal");
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(SignalNameClashErrorInsideState)
{
  try
  {
    FSM_TOP(top);

    LocalSignal<void> signal1("signal", top);
    LocalSignal<void> signal2("signal", top);

    ASSERT(false);
  }
  catch (CompoundState::LocalSignalNameClashError const & err)
  {
    ASSERT_EQ(err.signalName, "signal");
    ASSERT_EQ(err.scopePath, string() + "top");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(SignalScopeErrorOnEnter)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(int, signal1, region1);
    FSM_INIT(region1);
    FSM_FINAL(region1);
    FSM_AUTO(region1_INIT, region1_FINAL);

    FSM_LOCAL_SIGNAL(void, signal2, region2);
    FSM_INIT(region2);
    FSM_STATE(region2_STATE, region2);
    FSM_ENTER(region2_STATE, Action([&](){signal1.set(1);}));
    FSM_AUTO(region2_INIT, region2_STATE);

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (SignalDelegate::ScopeError & err)
  {
    ASSERT_EQ(err.path, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal1");
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2" + pathComponentSeparator + "region2_STATE");

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
TEST(SignalScopeErrorOnExit)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(int, signal1, region1);
    FSM_INIT(region1);
    FSM_FINAL(region1);
    FSM_AUTO(region1_INIT, region1_FINAL);

    FSM_LOCAL_SIGNAL(void, signal2, region2);
    FSM_INIT(region2);
    FSM_STATE(region2_STATE, region2);
    FSM_FINAL(region2);
    FSM_EXIT(region2_STATE, Action([&](){signal1.set(1);}));
    FSM_AUTO(region2_INIT, region2_STATE);
    FSM_AUTO(region2_STATE, region2_FINAL);

    top.init();
    top.step();
    top.step();

    ASSERT(false);
  }
  catch (SignalDelegate::ScopeError & err)
  {
    ASSERT_EQ(err.path, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal1");
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2" + pathComponentSeparator + "region2_STATE");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

TEST(SignalSetGet)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(int, signal, top);

    FSM_INIT(top);
    FSM_CONNECTOR(connector, top);
    FSM_FINAL(top);

    int readout;

    FSM_AUTO(top_INIT, connector, Action([&](){signal.set(1);}));
    FSM_AUTO(connector, top_FINAL, Action([&](){readout = signal.get();}));

    top.init();
    top.step();

    ASSERT_EQ(readout, 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(SignalDualSetGet)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(int, signal1, top);
    FSM_SIGNAL(int, signal2, top);

    FSM_INIT(top);
    FSM_CONNECTOR(connector1, top);
    FSM_CONNECTOR(connector2, top);
    FSM_CONNECTOR(connector3, top);
    FSM_FINAL(top);

    int intReadout1;
    int intReadout2;

    FSM_AUTO(top_INIT, connector1, Action([&](){signal1.set(1);}));
    FSM_AUTO(connector1, connector2, Action([&](){intReadout1 = signal1.get();}));
    FSM_STEP(connector2, connector3, Trigger(signal2), Action([&](){signal1.set(2);}));
    FSM_AUTO(connector3, top_FINAL, Action([&](){intReadout2 = signal1.get();}));

    top.init();
    top.step();
    top.step(signal2);

    ASSERT_EQ(intReadout1, 1);
    ASSERT_EQ(intReadout2, 2);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
