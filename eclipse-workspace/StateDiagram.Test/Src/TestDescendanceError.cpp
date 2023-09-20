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

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_EXECUTING
TEST(DescendanceError)
{
  try
  {
    FSM_TOP(topState);

    FSM_REGION(region1, topState);
    FSM_REGION(region2, topState);

    FSM_STATE(state1, region1);
    FSM_STATE(state2, region2);

    Auto state1_TO_state2{state1, state2};

    ASSERT(false);
  }
  catch (ExternalTransition::DescendanceError const & err)
  {
    ASSERT_EQ(err.sourcePath, string() + "topState" + pathComponentSeparator + "region1" + pathComponentSeparator + "state1");
    ASSERT_EQ(err.targetPath, string() + "topState" + pathComponentSeparator + "region2" + pathComponentSeparator + "state2");

    cout << err.msg(); cout.flush();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
#endif
