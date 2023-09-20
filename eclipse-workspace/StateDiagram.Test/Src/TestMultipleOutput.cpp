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

TEST(MultipleDirectOutputOnAuto)
{
  try
  {
    bool sawFinalStep{false};

    FSM_TOP(top);

    FSM_LOCAL_SIGNAL(void, signal1, top);
    FSM_LOCAL_SIGNAL(void, signal2, top);

    FSM_INIT(top);
    FSM_CONNECTOR(connector1, top);
    FSM_CONNECTOR(connector2, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, connector1, Output(FSM_LEV{signal1, signal2}));
    FSM_STEP(connector1, connector2, Trigger(signal1));
    FSM_STEP(connector2, top_FINAL, Trigger(signal2), Action([&](){sawFinalStep = true;}));

    top.init();
    top.step();

    ASSERT(sawFinalStep);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(MultipleDirectOutputOnStep)
{
  try
  {
    bool sawFinalStep{false};

    FSM_TOP(top);

    FSM_SIGNAL(void, signal, top);

    FSM_LOCAL_SIGNAL(void, signal1, top);
    FSM_LOCAL_SIGNAL(void, signal2, top);

    FSM_INIT(top);
    FSM_CONNECTOR(connector1, top);
    FSM_CONNECTOR(connector2, top);
    FSM_FINAL(top);

    FSM_STEP(top_INIT, connector1, Trigger(signal), Output(FSM_LEV{signal1, signal2}));
    FSM_STEP(connector1, connector2, Trigger(signal1));
    FSM_STEP(connector2, top_FINAL, Trigger(signal2), Action([&](){sawFinalStep = true;}));

    top.init();
    top.step(signal);

    ASSERT(sawFinalStep);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(MultipleFunOutputOnAuto)
{
  try
  {
    bool sawFinalStep{false};

    FSM_TOP(top);

    FSM_LOCAL_SIGNAL(void, signal1, top);
    FSM_LOCAL_SIGNAL(void, signal2, top);

    FSM_INIT(top);
    FSM_CONNECTOR(connector1, top);
    FSM_CONNECTOR(connector2, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, connector1, Output([&](){return FSM_LEV{signal1, signal2};}));
    FSM_STEP(connector1, connector2, Trigger(signal1));
    FSM_STEP(connector2, top_FINAL, Trigger(signal2), Action([&](){sawFinalStep = true;}));

    top.init();
    top.step();

    ASSERT(sawFinalStep);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(MultipleFunOutputOnStep)
{
  try
  {
    bool sawFinalStep{false};

    FSM_TOP(top);

    FSM_SIGNAL(void, signal, top);

    FSM_LOCAL_SIGNAL(void, signal1, top);
    FSM_LOCAL_SIGNAL(void, signal2, top);

    FSM_INIT(top);
    FSM_CONNECTOR(connector1, top);
    FSM_CONNECTOR(connector2, top);
    FSM_FINAL(top);

    FSM_STEP(top_INIT, connector1, Trigger(signal), Output([&](){return FSM_LEV{signal1, signal2};}));
    FSM_STEP(connector1, connector2, Trigger(signal1));
    FSM_STEP(connector2, top_FINAL, Trigger(signal2), Action([&](){sawFinalStep = true;}));

    top.init();
    top.step(signal);

    ASSERT(sawFinalStep);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(MultipleTriggeredOutputOnAuto)
{
  try
  {
    int testVar{0};

    FSM_TOP(top);

    FSM_LOCAL_SIGNAL(void, signal1, top);
    FSM_LOCAL_SIGNAL(int, signal2, top);

    FSM_INIT(top);
    FSM_CONNECTOR(connector1, top);
    FSM_CONNECTOR(connector2, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, connector1, Output([&](){return FSM_LEV{signal1, signal2(1)};}));
    FSM_STEP(connector1, connector2, Trigger(signal1));
    FSM_STEP(connector2, top_FINAL, Trigger(signal2), Action([&](Event const & trigger){testVar = trigger.get<int>();}));

    top.init();
    top.step();

    ASSERT_EQ(testVar, 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(MultipleTriggeredOutputOnStep)
{
  try
  {
    int testVar{0};

    FSM_TOP(top);

    FSM_SIGNAL(void, signal, top);

    FSM_LOCAL_SIGNAL(void, signal1, top);
    FSM_LOCAL_SIGNAL(int, signal2, top);

    FSM_INIT(top);
    FSM_CONNECTOR(connector1, top);
    FSM_CONNECTOR(connector2, top);
    FSM_FINAL(top);

    FSM_STEP(top_INIT, connector1, Trigger(signal), Output([&](){return FSM_LEV{signal1, signal2(1)};}));
    FSM_STEP(connector1, connector2, Trigger(signal1));
    FSM_STEP(connector2, top_FINAL, Trigger(signal2), Action([&](Event const & trigger){testVar = trigger.get<int>();}));

    top.init();
    top.step(signal);

    ASSERT_EQ(testVar, 1);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(OutputScopeErrorOnDirectMultipleOutputAuto)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(void, signal1, region1);
    FSM_LOCAL_SIGNAL(void, signal2, region2);
    FSM_CONNECTOR(connector, region2);

    FSM_AUTO(connector, connector, Output(FSM_LEV{signal1, signal2}));

    ASSERT(false);
  }
  catch (Transition::ScopeError::Output const & err)
  {
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2" + pathComponentSeparator + "connector");
    ASSERT_EQ(err.signalPath, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal1");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(OutputScopeErrorOnDirectMultipleOutputStep)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(void, signal1, region1);
    FSM_LOCAL_SIGNAL(void, signal2, region2);
    FSM_CONNECTOR(connector, region2);

    FSM_STEP(connector, connector, Trigger(signal2), Output(FSM_LEV{signal1, signal2}));

    ASSERT(false);
  }
  catch (Transition::ScopeError::Output const & err)
  {
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2" + pathComponentSeparator + "connector");
    ASSERT_EQ(err.signalPath, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal1");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(OutputScopeErrorOnFunMultipleOutputAuto)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(void, signal1, region1);
    FSM_LOCAL_SIGNAL(void, signal2, region2);

    FSM_INIT(region1);

    FSM_INIT(region2);
    FSM_CONNECTOR(connector, region2);

    FSM_AUTO(region2_INIT, connector);
    FSM_AUTO(connector, connector, Output([&](){return FSM_LEV{signal1, signal2};}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (Transition::ScopeError::Output const & err)
  {
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2");
    ASSERT_EQ(err.signalPath, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal1");

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
TEST(OutputScopeErrorOnFunMultipleOutputStep)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, signal, top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(void, signal1, region1);
    FSM_LOCAL_SIGNAL(void, signal2, region2);

    FSM_INIT(region1);

    FSM_INIT(region2);
    FSM_CONNECTOR(connector, region2);

    FSM_AUTO(region2_INIT, connector);
    FSM_STEP(connector, connector, Trigger(signal), Output([&](){return FSM_LEV{signal1, signal2};}));

    top.init();
    top.step(signal);

    ASSERT(false);
  }
  catch (Transition::ScopeError::Output const & err)
  {
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2");
    ASSERT_EQ(err.signalPath, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal1");

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
TEST(OutputScopeErrorOnTriggeredMultipleOutputStep)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, signal, top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_SIGNAL(void, signal1, region1);
    FSM_LOCAL_SIGNAL(void, signal2, region2);

    FSM_INIT(region1);

    FSM_INIT(region2);
    FSM_CONNECTOR(connector, region2);

    FSM_AUTO(region2_INIT, connector);
    FSM_STEP(connector, connector, Trigger(signal), Output([&](Event const &){return FSM_LEV{signal1, signal2};}));

    top.init();
    top.step(signal);

    ASSERT(false);
  }
  catch (Transition::ScopeError::Output const & err)
  {
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region2");
    ASSERT_EQ(err.signalPath, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "signal1");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif
