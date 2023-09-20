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

TEST(SimpleVarSetGet)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);

    var.set(1);

    ASSERT(var.get() == 1);
    ASSERT(var.get() == 1);

    var.set(0);
    var.set(2);

    ASSERT(var.get() == 2);
    ASSERT(var.get() == 2);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarOps)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);

    var << 1;
    ASSERT(var.get() == 1);

    int x = 2;
    var << x;
    ASSERT(var.get() == 2);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(VarGetOnNotValid)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_FINAL, Action([&](){var.get();}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (VarDelegate::GetOnNotValidError const & err)
  {
    ASSERT_EQ(err.path, "var");

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
TEST(VarSetOnAlreadySet)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_FINAL, Action([&](){var.set(1); var.set(1);}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (VarDelegate::SetOnAlreadySetError const & err)
  {
    ASSERT_EQ(err.path, "var");

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
TEST(VarSetNxtOnAlreadySetNxt)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_FINAL, Action([&](){var.setNxt(1); var.setNxt(1);}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (VarDelegate::SetNxtOnAlreadySetNxtError const & err)
  {
    ASSERT_EQ(err.path, "var");

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
TEST(VarSetOnAlreadySetNxt)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);
    FSM_SIGNAL(void, signal, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, top_STATE, Action([&](){var.setNxt(0);}));
    FSM_STEP(top_STATE, top_FINAL, Trigger(signal), Action([&](){var.set(0);}));

    top.init();
    top.step();
    top.step(signal);

    ASSERT(false);
  }
  catch (VarDelegate::SetOnAlreadySetError const & err)
  {
    ASSERT_EQ(err.path, "var");

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
TEST(VarSetAfterGet)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top, 1);
    FSM_SIGNAL(void, signal, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE, top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_STATE);
    FSM_STEP(top_STATE, top_FINAL, Trigger(signal), Action([&](){var.get(); var.set(1);}));

    top.init();
    top.step();
    top.step(signal);

    ASSERT(false);
  }
  catch (VarDelegate::SetAfterGetError const & err)
  {
    ASSERT_EQ(err.path, "var");

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
TEST(VarScopeErrorOnSpec)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_VAR(int, var, region1);
    FSM_INIT(region1);
    FSM_FINAL(region1);
    FSM_AUTO(region1_INIT, region1_FINAL);

    FSM_LOCAL_SIGNAL(void, signal, region2);
    FSM_INIT(region2);
    FSM_CONNECTOR(region2_STATE, region2);
    FSM_FINAL(region2);
    FSM_AUTO(region2_INIT, region2_STATE, Output(signal));
    FSM_STEP(region2_STATE, region2_FINAL, Trigger(signal), Action([&](){var.set(1);}));

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (VarDelegate::ScopeError const & err)
  {
    ASSERT_EQ(err.path, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "var");
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

TEST(VarNameClashErrorInsideRegion)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region, top);

    LocalVar<float> var1("var", region);
    LocalVar<float> var2("var", region);

    ASSERT(false);
  }
  catch (Region::LocalVarNameClashError const & err)
  {
    ASSERT_EQ(err.varName, "var");
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "region");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarNameClashErrorInsideState)
{
  try
  {
    FSM_TOP(top);

    LocalVar<char> var1("var", top);
    LocalVar<char> var2("var", top);

    ASSERT(false);
  }
  catch (CompoundState::LocalVarNameClashError const & err)
  {
    ASSERT_EQ(err.varName, "var");
    ASSERT_EQ(err.scopePath, string() + "top");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarExternalNameClashError)
{
  try
  {
    FSM_TOP(top);

    ExternalVar<double> var1("var", top);
    ExternalVar<double> var2("var", top);

    ASSERT(false);
  }
  catch (ExternalVarDelegate::NameClashError const & err)
  {
    ASSERT_EQ(err.varName, "var");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(VarScopeErrorOnEnter)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_VAR(int, var, region1);
    FSM_INIT(region1);
    FSM_FINAL(region1);
    FSM_AUTO(region1_INIT, region1_FINAL);

    FSM_LOCAL_SIGNAL(void, signal, region2);
    FSM_INIT(region2);
    FSM_STATE(region2_STATE, region2);
    FSM_ENTER(region2_STATE, Action([&](){var.set(1);}));
    FSM_AUTO(region2_INIT, region2_STATE);

    top.init();
    top.step();

    ASSERT(false);
  }
  catch (VarDelegate::ScopeError & err)
  {
    ASSERT_EQ(err.path, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "var");
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
TEST(VarScopeErrorOnExit)
{
  try
  {
    FSM_TOP(top);

    FSM_REGION(region1, top);
    FSM_REGION(region2, top);

    FSM_LOCAL_VAR(int, var, region1);
    FSM_INIT(region1);
    FSM_FINAL(region1);
    FSM_AUTO(region1_INIT, region1_FINAL);

    FSM_INIT(region2);
    FSM_STATE(region2_STATE, region2);
    FSM_FINAL(region2);
    FSM_EXIT(region2_STATE, Action([&](){var.set(1);}));
    FSM_AUTO(region2_INIT, region2_STATE);
    FSM_AUTO(region2_STATE, region2_FINAL);

    top.init();
    top.step();
    top.step();
    top.step();

    ASSERT(false);
  }
  catch (VarDelegate::ScopeError & err)
  {
    ASSERT_EQ(err.path, string() + "top" + pathComponentSeparator + "region1" + pathComponentSeparator + "var");
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

TEST(VarSetGet)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE, top);
    FSM_FINAL(top);

    int readout;

    FSM_AUTO(top_INIT, top_STATE, Action([&](){var.set(1);}));
    FSM_AUTO(top_STATE, top_FINAL, Action([&](){readout = var.get();}));

    top.init();
    top.step();
    top.step();

    ASSERT_EQ(readout, 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarDualSetGet)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);
    FSM_SIGNAL(void, signal, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE1, top);
    FSM_STATE(top_STATE2, top);
    FSM_STATE(top_STATE3, top);
    FSM_FINAL(top);

    int intReadout1;
    int intReadout2;

    FSM_AUTO(top_INIT, top_STATE1, Action([&](){var.set(1);}));
    FSM_AUTO(top_STATE1, top_STATE2, Action([&](){intReadout1 = var.get();}));
    FSM_STEP(top_STATE2, top_STATE3, Trigger(signal), Action([&](){var.set(2);}));
    FSM_AUTO(top_STATE3, top_FINAL, Action([&](){intReadout2 = var.get();}));

    top.init();
    top.step();
    top.step();
    top.step(signal);
    top.step();

    ASSERT_EQ(intReadout1, 1);
    ASSERT_EQ(intReadout2, 2);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(VarTypeError)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(float, var, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE, top);
    FSM_FINAL(top);

    float readout;

    FSM_AUTO(top_INIT, top_STATE, Action([&](){var.set(1);}));
    FSM_AUTO(top_STATE, top_FINAL, Action([&](){readout = var.get();}));

    top.init();
    top.step();
  }
  catch (VarDelegate::TypeError const & err)
  {
    ASSERT_EQ(err.path, string() + "var");
    ASSERT_EQ(err.scopePath, string() + "top" + pathComponentSeparator + "REGION");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif

TEST(SimpleVarSetNxtGet)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);
    FSM_SIGNAL(void, signal, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE, top);
    FSM_FINAL(top);

    int readout;

    FSM_AUTO(top_INIT, top_STATE, Action([&](){var.setNxt(1);}));
    FSM_STEP(top_STATE, top_FINAL, Trigger(signal), Action([&](){readout = var.get();}));

    top.init();
    top.step();
    top.step(signal);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(CascadedVarSetNxtGet)
{
  try
  {
    FSM_TOP(top);

    FSM_VAR(int, var, top);
    FSM_SIGNAL(void, signal1, top);
    FSM_SIGNAL(void, signal2, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE1, top);
    FSM_STATE(top_STATE2, top);
    FSM_FINAL(top);

    int readout1;
    int readout2;

    FSM_AUTO(top_INIT, top_STATE1, Action([&](){var.setNxt(1);}));
    FSM_STEP(top_STATE1, top_STATE2, Trigger(signal1), Action([&](){readout1 = var.get(); var.setNxt(2);}));
    FSM_STEP(top_STATE2, top_FINAL, Trigger(signal2), Action([&](){readout2 = var.get();}));

    top.init();
    top.step();
    top.step(signal1);
    top.step(signal2);

    ASSERT_EQ(readout1, 1);
    ASSERT_EQ(readout2, 2);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(CascadedVarSetSetNxtGet)
{
  try
  {
    FSM_TOP(top);

    FSM_VAR(int, var, top);
    FSM_SIGNAL(void, signal1, top);
    FSM_SIGNAL(void, signal2, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE1, top);
    FSM_STATE(top_STATE2, top);
    FSM_FINAL(top);

    int readout1;
    int readout2;

    FSM_AUTO(top_INIT, top_STATE1, Action([&](){var.set(1);}));
    FSM_STEP(top_STATE1, top_STATE2, Trigger(signal1), Action([&](){readout1 = var.get(); var.setNxt(2);}));
    FSM_STEP(top_STATE2, top_FINAL, Trigger(signal2), Action([&](){readout2 = var.get();}));

    top.init();
    top.step();
    top.step(signal1);
    top.step(signal2);

    ASSERT_EQ(readout1, 1);
    ASSERT_EQ(readout2, 2);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarLeftArrow)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_FINAL, Action([&](){var << 1;}));

    top.init();
    top.step();

    ASSERT_EQ(var.get(), 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarNxtLeftArrow)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top);
    FSM_SIGNAL(void, signal, top);

    FSM_INIT(top);
    FSM_STATE(top_STATE, top);
    FSM_FINAL(top);
    FSM_AUTO(top_INIT, top_STATE, Action([&](){var.nxt << 1;}));
    FSM_STEP(top_STATE, top_FINAL, Trigger(signal));

    top.init();
    top.step();
    top.step(signal);

    ASSERT_EQ(var.get(), 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarExternalInit)
{
  try
  {
    FSM_TOP(top);
    FSM_VAR(int, var, top, 1);
    ASSERT_EQ(var.get(), 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarLocalInitAtCompoundState)
{
  try
  {
    FSM_TOP(top);
    FSM_LOCAL_VAR(int, var, top, 1);

    FSM_INIT(top);

    top.init();
    top.step();

    ASSERT_EQ(var.get(), 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(VarLocalInitAtRegion)
{
  try
  {
    FSM_TOP(top);
    FSM_REGION(top_REGION, top);
    FSM_LOCAL_VAR(int, var, top_REGION, 1);

    FSM_INIT(top_REGION);

    top.init();
    top.step();

    ASSERT_EQ(var.get(), 1);
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
