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

TEST(TriggerScopeError)
{
  try
  {
    FSM_TOP(topState);

    FSM_REGION(region1, topState);
    FSM_REGION(region2, topState);

    FSM_LOCAL_SIGNAL(void, is, region1);
    FSM_STATE(state, region2);

    Step state_TO_state{state, state, Trigger(is)};

    ASSERT(false);
  }
  catch (Step::TriggerScopeError const & err)
  {
    ASSERT_EQ(err.scopePath, string() + "topState" + pathComponentSeparator + "region2");
    ASSERT_EQ(err.signalPath, string() + "topState" + pathComponentSeparator + "region1" + pathComponentSeparator + "is");

    cout << err.msg(); cout.flush();
  }
  catch (Error const & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
