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

TEST(TriggerOnAutoTransitionError)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, global, top);

    FSM_INIT(top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, top_FINAL);

    top_INIT_TO_top_FINAL.add(Trigger(global));

    ASSERT(false);
  }
  catch (Transition::Spec::TriggerSpecOnTriggerlessTransitionError const & err)
  {
    ASSERT_EQ(err.transitionTypeIndicator, "auto");
    ASSERT_EQ(err.originPath, string() + "top" + pathComponentSeparator + STATE_DIAGRAM_DEFAULT_NAME_OF_REGION + pathComponentSeparator + STATE_DIAGRAM_NAME_OF_INITIAL_STATE);

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TriggeredActionOnAutoTransitionError)
{
  try
  {
    FSM_TOP(top);

    FSM_INIT(top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, top_FINAL);

    top_INIT_TO_top_FINAL.add(Action([](Event const &){;}));

    ASSERT(false);
  }
  catch (Transition::Spec::TriggeredSpecOnTriggerlessTransitionError const & err)
  {
    ASSERT_EQ(err.specTypeIndicator, "action");
    ASSERT_EQ(err.transitionTypeIndicator, "auto");
    ASSERT_EQ(err.originPath, string() + "top" + pathComponentSeparator + STATE_DIAGRAM_DEFAULT_NAME_OF_REGION + pathComponentSeparator + STATE_DIAGRAM_NAME_OF_INITIAL_STATE);

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TriggeredGuardOnEnterTransitionError)
{
  try
  {
    FSM_TOP(top);

    FSM_STATE(state, top);
    FSM_ENTER(state);

    state_ENTER.add(Guard([](Event const &){return true;}));

    ASSERT(false);
  }
  catch (Transition::Spec::TriggeredSpecOnTriggerlessTransitionError const & err)
  {
    ASSERT_EQ(err.specTypeIndicator, "guard");
    ASSERT_EQ(err.transitionTypeIndicator, "enter");
    ASSERT_EQ(err.originPath, string() + "top" + pathComponentSeparator + STATE_DIAGRAM_DEFAULT_NAME_OF_REGION + pathComponentSeparator + "state");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TriggeredOutputOnExitTransitionError)
{
  try
  {
    FSM_TOP(top);

    FSM_STATE(state, top);
    FSM_EXIT(state);

    FSM_LOCAL_SIGNAL(void, local, state);

    state_EXIT.add(Output([&](Event const &) -> LocalEvent const & {return local;}));

    ASSERT(false);
  }
  catch (Transition::Spec::TriggeredSpecOnTriggerlessTransitionError const & err)
  {
    ASSERT_EQ(err.specTypeIndicator, "output");
    ASSERT_EQ(err.transitionTypeIndicator, "exit");
    ASSERT_EQ(err.originPath, string() + "top" + pathComponentSeparator + STATE_DIAGRAM_DEFAULT_NAME_OF_REGION + pathComponentSeparator + "state");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}



