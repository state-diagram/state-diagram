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

TEST(Feature)
{
  try
  {
    FSM_TOP(topState);

    FSM_SIGNAL(void, es, topState);

    FSM_REGION(region_1, topState);
    FSM_REGION(region_2, topState);
    FSM_REGION(region_3, topState);

    FSM_LOCAL_SIGNAL(void, is, topState);
    FSM_LOCAL_SIGNAL(void, ie_1, region_1);
    FSM_LOCAL_SIGNAL(void, ie_2, region_2);

    FSM_INIT(region_1);
    FSM_STATE(state_1, region_1);
    FSM_FINAL(region_1);

    Auto region_1_INIT_TO_state_1(region_1_INIT, state_1);
    Auto state_1_TO_region_1_FINAL(state_1, region_1_FINAL, Guard([](){return true;}), Action([](){;}), Output(is));

    state_1_TO_region_1_FINAL.add(Guard([](){return true;}));
    state_1_TO_region_1_FINAL.add(Output(is));
    state_1_TO_region_1_FINAL.add(Output(ie_1));

    FSM_INIT(state_1);
    FSM_STATE(state_1_1, state_1);
    FSM_STATE(state_1_2, state_1);
    FSM_FINAL(state_1);

    Step state_1_1_TO_state_1_2
    {
      state_1_1
    , state_1_2
    , Trigger(es)
    , Guard([](){return true;})
    , Guard([](Event const &){return true;})
    , Action([](){;})
    , Action([](Event const &){;})
    , Output(is)
    , Output([&]()-> LocalEvent const & {return is;})
    , Output([&](Event const &)-> LocalEvent const & {return is;})
    };

    Step state_1_2_TO_state_1_FINAL{state_1_2, state_1_FINAL};
    state_1_2_TO_state_1_FINAL.add(Trigger(es));
    state_1_2_TO_state_1_FINAL.add(Guard([](){return true;}));
    state_1_2_TO_state_1_FINAL.add(Guard([](Event const &){return true;}));
    state_1_2_TO_state_1_FINAL.add(Action([](){;}));
    state_1_2_TO_state_1_FINAL.add(Action([](Event const &){;}));
    state_1_2_TO_state_1_FINAL.add(Output(is));
    state_1_2_TO_state_1_FINAL.add(Output([&](Event const &)->LocalEvent const & {return is;}));
    state_1_2_TO_state_1_FINAL.add(FreezeFlag(FULL));
    state_1_2_TO_state_1_FINAL.add(FreezeFlag(SHALLOW));

    state_1_2_TO_state_1_FINAL << Guard([](){return true;}) << Guard([](){return true;});

    Auto region_1_INIT_TO_state_1_INIT(region_1_INIT, state_1_1);
    Auto pincipal_1_1_TO_region_1_FINAL(state_1_1, region_1_FINAL);
    pincipal_1_1_TO_region_1_FINAL.add(CompletionFlag());

    FSM_INIT(region_2);
    FSM_STATE(state_2_1, region_2);
    FSM_STATE(state_2_2, region_2);
    FSM_FINAL(region_2);

    FSM_INIT(region_3);
    FSM_CONNECTOR(connector_3_1, region_3);
    FSM_CONNECTOR(connector_3_2, region_3);
    FSM_FINAL(region_3);

    FSM_AUTO(region_3_INIT, connector_3_1);
    FSM_AUTO(connector_3_1, connector_3_2);
    FSM_AUTO(connector_3_2, region_3_FINAL);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(MultipleOutput)
{
  try
  {
    FSM_TOP(topState);

    FSM_SIGNAL(void, es, topState);

    FSM_REGION(region_1, topState);
    FSM_REGION(region_2, topState);

    FSM_LOCAL_SIGNAL(void, is, topState);
    FSM_LOCAL_SIGNAL(void, ie_1, region_1);
    FSM_LOCAL_SIGNAL(void, ie_2, region_2);

    FSM_INIT(region_1);
    FSM_STATE(state_1, region_1);
    FSM_FINAL(region_1);

    Auto region_1_INIT_TO_state_1(region_1_INIT, state_1);
    Auto state_1_TO_region_1_FINAL(state_1, region_1_FINAL, Guard([](){return true;}), Action([](){;}), Output(is));

    state_1_TO_region_1_FINAL.add(Guard([](){return true;}));
    state_1_TO_region_1_FINAL.add(Output(is));
    state_1_TO_region_1_FINAL.add(Output(ie_1));

    FSM_INIT(state_1);
    FSM_STATE(state_1_1, state_1);
    FSM_STATE(state_1_2, state_1);
    FSM_FINAL(state_1);

    Step state_1_1_TO_state_1_2
    {
      state_1_1
    , state_1_2
    , Trigger(es)
    , Guard([](){return true;})
    , Guard([](Event const &){return true;})
    , Action([](){;})
    , Action([](Event const &){;})
    , Output(is)
    , Output([&]()-> LocalEvent const & {return is;})
    , Output([&](Event const &)-> LocalEvent const & {return is;})
    };

    Step state_1_2_TO_state_1_FINAL{state_1_2, state_1_FINAL};
    state_1_2_TO_state_1_FINAL.add(Trigger(es));
    state_1_2_TO_state_1_FINAL.add(Guard([](){return true;}));
    state_1_2_TO_state_1_FINAL.add(Guard([](Event const &){return true;}));
    state_1_2_TO_state_1_FINAL.add(Action([](){;}));
    state_1_2_TO_state_1_FINAL.add(Action([](Event const &){;}));
    state_1_2_TO_state_1_FINAL.add(Output(is));
    state_1_2_TO_state_1_FINAL.add(Output([&](Event const &)->LocalEvent const & {return is;}));
    state_1_2_TO_state_1_FINAL.add(FreezeFlag(FULL));
    state_1_2_TO_state_1_FINAL.add(FreezeFlag(SHALLOW));

    Auto region_1_INIT_TO_state_1_INIT(region_1_INIT, state_1_1);
    Auto pincipal_1_1_TO_region_1_FINAL(state_1_1, region_1_FINAL);
    pincipal_1_1_TO_region_1_FINAL.add(CompletionFlag());

    FSM_INIT(region_2);
    FSM_STATE(state_2_1, region_2);
    FSM_STATE(state_2_2, region_2);
    FSM_FINAL(region_2);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(TriggeredSpecs)
{
  try
  {
    FSM_TOP(top);
    FSM_SIGNAL(int, signal1, top);
    FSM_LOCAL_SIGNAL(int, signal2, top);

    FSM_INIT(top);
    FSM_FINAL(top);
    FSM_STEP(top_INIT, top_FINAL, Trigger(signal1));
    top_INIT_TO_top_FINAL <<
      Guard([](Event const & trigger){return trigger.get<int>() == 1;}) <<
      Output([&](Event const & trigger)->LocalEvent const &{return signal2(trigger.get<int>());}) <<
      Action([&](Event const & trigger){printf("%d\n", trigger.get<int>());});

    top.init();
    top.step(signal1(1));
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

