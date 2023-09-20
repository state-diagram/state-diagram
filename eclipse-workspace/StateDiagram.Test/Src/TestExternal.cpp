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

TEST(Externals)
{
  try
  {
    FSM_TOP(top);

    FSM_SIGNAL(void, signal, top);
    FSM_VAR(int, var, top);

    FSM_INIT(top);
    FSM_STATE(top_SUBSTATE, top);
    FSM_FINAL(top);

    FSM_AUTO(top_INIT, top_SUBSTATE);
    FSM_STEP(top_SUBSTATE, top_FINAL, Trigger(signal), Action([&]{var.set(1);}));

    top.init();
    top.step();
    bool const stat{top.step(signal)};

    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}
