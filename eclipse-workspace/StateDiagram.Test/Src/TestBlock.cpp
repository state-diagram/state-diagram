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

TEST(Block)
{
  try
  {
    bool sawStep1{false};
    bool sawStep2{false};

    FSM_TOP(top);

    FSM_SIGNAL(void, signal, top);

    FSM_INIT(top);
    FSM_STATE(state, top);
    FSM_FINAL(top);

    FSM_STEP(top_INIT, state, Trigger(signal), Action{[&]{sawStep1 = true;}});
    FSM_STEP(state, top_FINAL, Trigger(signal), Action{[&]{sawStep2 = true;}});

    top.init();
    ASSERT(!sawStep1);
    ASSERT(!sawStep2);

    top.step(signal);
    ASSERT(sawStep1);
    ASSERT(!sawStep2);
    sawStep1 = false;

    top.step(signal);
    ASSERT(!sawStep1);
    ASSERT(sawStep2);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
