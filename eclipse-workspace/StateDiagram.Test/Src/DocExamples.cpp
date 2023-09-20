/*
   Copyright 2019-2020 State Diagram Contributors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language startverning permissions and
   limitations under the License.
*/

#include "StateDiagramTestSetup.h"

#include <cassert>
#include <cstdint>

TEST(DocHelloWorld)
{
  try
  {
    FSM_TOP(helloWorld);
    FSM_SIGNAL(void, sayHello, helloWorld);

    FSM_INIT(helloWorld);
    FSM_STATE(aboutToSayHello, helloWorld);
    FSM_FINAL(helloWorld);

    FSM_AUTO(helloWorld_INIT, aboutToSayHello);
    FSM_STEP(aboutToSayHello, helloWorld_FINAL, Trigger(sayHello), Action([]{printf("Hello world!\n");}));

    helloWorld.init();
    helloWorld.step();
    helloWorld.step(sayHello);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DocClassDataMembers)
{
  try
  {
    class MsgPrinterRegion
    {
    private:
      CompoundState const & owner;
      Signal<void> const & trigger;
      string const msg;

    public:
      MsgPrinterRegion(CompoundState const & owner, Signal<void> const & trigger, string const & msg)
      :
        owner(owner)
      , trigger(trigger)
      , msg(msg)
      {
        // TSILE
      }

    private:
      FSM_INIT(owner);
      FSM_CONNECTOR(printMsg, owner);
      FSM_FINAL(owner);
      FSM_AUTO(owner_INIT, printMsg);
      FSM_STEP(printMsg, owner_FINAL, Trigger(trigger), Action([this]{printf("%s\n", msg.c_str());}));
    };

    FSM_TOP(helloWorld);
    FSM_SIGNAL(void, sayHello, helloWorld);
    MsgPrinterRegion helloWorldRegion(helloWorld, sayHello, "Hello world!");

    helloWorld.init();
    helloWorld.step();
    bool const stat{helloWorld.step(sayHello)};
    ASSERT(stat);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DocPrint)
{
  try
  {
    FSM_TOP(M);
    FSM_SIGNAL(string, A, M);
    FSM_SIGNAL(string, B, M);

    FSM_INIT(M);
    FSM_CONNECTOR(S, M);
    FSM_FINAL(M);

    FSM_AUTO(M_INIT, S);
    FSM_STEP(S, M_FINAL);
    S_TO_M_FINAL
      << Trigger(A)
      << Trigger(B)
      << Action([](Event const & signal){printf("%s\n", signal.get<string>().c_str());});

    M.init();
    bool const statA{M.step(A(string("Hello!")))};
    ASSERT(statA);

    M.init();
    bool const statB{M.step(B(string("Good bye!")))};
    ASSERT(statB);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DocNesting)
{
  try
  {
    FSM_TOP(M);

    FSM_INIT(M);
    FSM_STATE(S, M);
    FSM_FINAL(M);
    FSM_AUTO(M_INIT, S);
    FSM_AUTO(S, M_FINAL);

    FSM_LOCAL_SIGNAL(void, A, S);

    FSM_REGION(R1, S);
    FSM_INIT(R1);
    FSM_FINAL(R1);
    FSM_AUTO(R1_INIT, R1_FINAL, Output(A));

    FSM_REGION(R2, S);
    FSM_INIT(R2);
    FSM_FINAL(R2);
    FSM_STEP(R2_INIT, R2_FINAL, Trigger(A));

    M.init();
    M.step();
    ASSERT(M.step());
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DocPrintLoop)
{
  try
  {
    FSM_TOP(M);
    FSM_SIGNAL(string, A, M);
    FSM_SIGNAL(string, B, M);

    FSM_INIT(M);
    FSM_STATE(S, M);
    FSM_FINAL(M);

    FSM_AUTO(M_INIT, S);
    FSM_INTERNAL_STEP(S);
    S_INTERNAL_STEP
      << Max1Flag()
      << Trigger(A)
      << Trigger(B)
      << Action([](Event const& signal){printf("%s\n", signal.get<string>().c_str());});

    M.init();
    M.step(A(string("Hello!")));
    M.step(B(string("Hello again!")));
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DocSequence)
{
  try
  {
    FSM_TOP(M);
    FSM_LOCAL_SIGNAL(string, A, M);

    FSM_INIT(M);
    FSM_CONNECTOR(C, M);
    FSM_FINAL(M);

    FSM_AUTO(M_INIT, C, Output(A(string("test"))));
    FSM_STEP(C, M_FINAL, Trigger(A), Action([&](Event const& E){printf("%s\n", E.get<string>().c_str());}));

    M.init();
    M.step();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

//TEST(Tmp)
//{
//  try
//  {
//    int x1{0};
//    int x2{0};
//
//    FSM_TOP(top);
//    FSM_SIGNAL(void, signal, top);
//
//    FSM_INIT(top);
//    FSM_STATE(state, top);
//
//    FSM_STEP(signal, top_INIT, state);
//    FSM_ENTER(state, Action([&]{x1 = 1;}));
//    FSM_INTERNAL_STEP(signal, state,  Action([&]{printf("hi!\n"); x2 = 1;}), Max1Flag());
//
//    top.init();
//    top.step(signal);
//
//    ASSERT_EQ(x1, 1);
//    ASSERT_EQ(x2, 0);
//  }
//  catch (Error & err)
//  {
//    cout << err.msg(); cout.flush();
//    ASSERT(false);
//  }
//}

//TEST(Tmp)
//{
//  try
//  {
//    bool sawStep{false};
//
//    FSM_TOP(top);
//
//    FSM_INIT(top);
//    FSM_STATE(state1, top);
//    FSM_LOCAL_SIGNAL(void, signal, state1);
//
//    FSM_AUTO(top_INIT, state1);
//
//    FSM_REGION(region1, state1);
//    FSM_REGION(region2, state1);
//
//    FSM_INIT(region1);
//    FSM_FINAL(region1);
//    FSM_AUTO(region1_INIT, region1_FINAL, Output(signal));
//
//    FSM_INIT(region2);
//    FSM_CONNECTOR(state2, region2);
//    FSM_STATE(state3, region2);
//    FSM_AUTO(region2_INIT, state2);
//    FSM_STEP(state2, state3, Trigger(signal), Action([&]{sawStep = true;}));
//
//    top.init();
//    top.step();
//
//    ASSERT(sawStep);
//  }
//  catch (Error & err)
//  {
//    cout << err.msg(); cout.flush();
//    ASSERT(false);
//  }
//}

//TEST(Tmp)
//{
//  try
//  {
//    FSM_TOP(top);
//    FSM_LOCAL_SIGNAL(int, signal, top);
//
//    FSM_REGION(region1, top);
//    FSM_REGION(region2, top);
//
//    FSM_INIT(region1);
//    FSM_FINAL(region1);
//    FSM_AUTO(region1_INIT, region1_FINAL, Output(signal(1)));
//
//    FSM_INIT(region2);
//    FSM_FINAL(region2);
//    FSM_STEP(region2_INIT, region2_FINAL, Trigger(signal), Action([&](Event const& signal){printf("%d\n", signal.get<int>());}));
//
//    top.init();
//    top.step();
//  }
//  catch (Error & err)
//  {
//    cout << err.msg(); cout.flush();
//    ASSERT(false);
//  }
//}

//TEST(Tmp)
//{
//  try
//  {
//    FSM_TOP(top);
//    FSM_SIGNAL(int, signal, top);
//    FSM_SIGNAL(int, signal2, top);
//
//    FSM_INIT(top);
//    FSM_STATE(state, top);
//    FSM_FINAL(top);
//    FSM_AUTO(top_INIT, state, Action([]{printf("outer auto\n");}));
//    FSM_STEP(state, top_FINAL, Trigger(signal), Action([]{printf("outer step\n");}));
//
//    FSM_INIT(state);
//    FSM_STATE(substate, state);
//    FSM_FINAL(state);
//    FSM_AUTO(state_INIT, substate, Action([]{printf("inner auto\n");}));
//    FSM_STEP(substate, state_FINAL, Trigger(signal), Action([]{printf("inner step\n");}));
//
//    top.init();
//    top.step();
//    //ASSERT(!top.step(signal));
//    ASSERT(top.step(signal));
//  }
//  catch (Error & err)
//  {
//    cout << err.msg(); cout.flush();
//    ASSERT(false);
//  }
//}

//TEST(Tmp)
//{
//  try
//  {
//    FSM_TOP(top);
//    FSM_VAR(int, var, top);
//
//    FSM_INIT(top);
//    FSM_CONNECTOR(connector, top);
//    FSM_FINAL(top);
//
//    FSM_AUTO(top_INIT, connector, Action([&]{var << 0;}));
//    FSM_AUTO(connector, top_FINAL, Action([&]{var << 0;}));
//
//    top.init();
//    ASSERT(top.step());
//  }
//  catch (Error & err)
//  {
//    cout << err.msg(); cout.flush();
//    ASSERT(false);
//  }
//}

TEST(Tmp)
{
  try
  {
    FSM_TOP(helloWorld);

    // Void signal used to trigger the state machine
    FSM_SIGNAL(void, sayHello, helloWorld);

    // An initial state directly beneath the top state
    FSM_INIT(helloWorld);

    // An ordinary state also directly beneath the top state
    FSM_STATE(printHelloWorld, helloWorld);

    // A final state directly beneath the top state
    FSM_FINAL(helloWorld);

    // A (triggerless) "auto" transition
    // from the initial state to printHelloWorld
    FSM_AUTO(helloWorld_INIT, printHelloWorld);

    // A (triggered) "step" transition from printHelloWorld to helloWorld_FINAL. The transition
    // carries an action parameterized by a lambda that achieves the desired side effect.
    FSM_STEP(printHelloWorld, helloWorld_FINAL, Trigger(sayHello), Action([]{printf("Hello world!\n");}));

    // Intializing the state machine
    helloWorld.init();

    // Letting the state machine handle the signal that lets it print "Hello world!"
    helloWorld.step();
    helloWorld.step(sayHello);
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

TEST(DocMicrowave)
{
  int16_t constexpr secondsPerMinute{60};
  int16_t constexpr maxMinutesSettable{30};
  int16_t constexpr maxSecondsSettable{secondsPerMinute - 1};
  int16_t constexpr minSecondsSettable{5};
  int16_t constexpr maxSeconds{maxMinutesSettable * secondsPerMinute};
  int16_t constexpr secondsStep{5};

  int16_t constexpr maxWattsSettable{750};
  int16_t constexpr minWattsSettable{150};
  int16_t constexpr wattsStep{150};

  // Mocking low-level API
  bool statDoorShut{true};

  bool sawInit{false};
  bool sawOn{false};
  bool sawOff{false};

  bool sawSoundBeep{false};
  bool sawDisplayDisplayMinutesSeconds{false};
  bool sawDisplayWatts{false};
  bool sawTurnOnDisplay{false};
  bool sawTurnOffDisplay{false};
  bool sawTurnOnMagnetron{false};
  bool sawTurnOffMagnetron{false};
  bool sawTurnOnTurntable{false};
  bool sawTurnOffTurntable{false};
  bool sawTurnOnLight{false};
  bool sawTurnOffLight{false};

  auto doorIsShut{[&](){return statDoorShut;}};
  auto soundBeep{[&]{
    sawSoundBeep = true;
  }};
  const auto turnOnDisplay{[&]{
    sawTurnOnDisplay = true;
  }};
  const auto displayMinutesSeconds{[&](const int16_t minutesSet, const int16_t secondsSet){
    sawDisplayDisplayMinutesSeconds = true;
  }};
  const auto displayWatts{[&](const int16_t watts){
    sawDisplayWatts = true;
  }};
  const auto turnOffDisplay{[&]{
    sawTurnOffDisplay = true;
  }};
  const auto turnOnMagnetron{[&](const int16_t watts){
    sawTurnOnMagnetron = true;
  }};
  const auto turnOffMagnetron{[&]{
    sawTurnOffMagnetron = true;
  }};
  const auto turnOnTurntable{[&]{
    sawTurnOnTurntable = true;
  }};
  const auto turnOffTurntable{[&]{
    sawTurnOffTurntable = true;
  }};
  const auto turnOnLight{[&]{
    sawTurnOnLight = true;
  }};
  const auto turnOffLight{[&]{
    sawTurnOffLight = true;
  }};

  // Microwave state machine
  try
  {
    FSM_TOP(microwave);

    FSM_SIGNAL(void, durationPlus, microwave);
    FSM_SIGNAL(void, durationMinus, microwave);
    FSM_SIGNAL(void, wattsPlus, microwave);
    FSM_SIGNAL(void, wattsMinus, microwave);
    FSM_SIGNAL(void, startStop, microwave);
    FSM_SIGNAL(void, doorOpen, microwave);
    FSM_SIGNAL(void, doorShut, microwave);
    FSM_SIGNAL(void, tick, microwave);

    FSM_INIT(microwave);
    FSM_STATE(standby, microwave);
    FSM_STATE(on, microwave);

    FSM_AUTO(microwave_INIT, standby);
    FSM_STEP(standby, on, Trigger(startStop), Trigger(durationPlus), Trigger(durationMinus), Trigger(wattsPlus), Trigger(wattsMinus), Trigger(doorOpen));
    FSM_STEP(on, standby, Trigger(tick), CompletionFlag());

    FSM_LOCAL_VAR(int16_t, seconds, on);
    FSM_LOCAL_VAR(int16_t, watts, on);
    FSM_LOCAL_SIGNAL(int16_t, secondsNxtSet, on);
    FSM_LOCAL_SIGNAL(int16_t, wattsNxtSet, on);

    FSM_ENTER(on, Action([&]{seconds << minSecondsSettable; watts << minWattsSettable;}));

    InternalStep const on_ON_durationPlus
    {
      on
    , Trigger(durationPlus)
    , Guard([&]{return seconds.get() > 0;})
    , Output([&]()->LocalSignal<int16_t> const&{
        int const valSecondsNxt{[&]{
          if (seconds.get() < secondsPerMinute)
          {
            int const rawValSecondsNxt{seconds.get() + secondsStep};
            return rawValSecondsNxt - (rawValSecondsNxt % secondsStep);
          }
          int const rawValSecondsNxt{seconds.get() + secondsPerMinute};
          return ((rawValSecondsNxt <= maxSecondsSettable) ? (rawValSecondsNxt - (rawValSecondsNxt % secondsPerMinute)) : maxSecondsSettable);
        }()};
        seconds.nxt << static_cast<int16_t>(valSecondsNxt);
        return secondsNxtSet(static_cast<int16_t>(valSecondsNxt));
      })
    , Max1Flag()
    };
    InternalStep const on_ON_durationMinus
    {
      on
    , Trigger(durationMinus)
    , Guard([&]{return seconds.get() > 0;})
    , Output([&]()->LocalSignal<int16_t> const&{
        int const valSecondsNxt{[&]{
          if (seconds.get() <= secondsPerMinute)
          {
            if (seconds.get() <= secondsStep)
            {
              return static_cast<int>(seconds.get());
            }
            if ((seconds.get() % secondsStep) == 0)
            {
              return seconds.get() - secondsStep;
            }
            return seconds.get() - (seconds.get() % secondsStep);
          }
          else
          {
            if ((seconds.get() % secondsPerMinute) == 0)
            {
              return seconds.get() - secondsPerMinute;
            }
            return seconds.get() - (seconds.get() % secondsPerMinute);
          }
        }()};
        seconds.nxt << static_cast<int16_t>(valSecondsNxt);
        return secondsNxtSet(static_cast<int16_t>(valSecondsNxt));
      })
    , Max1Flag()
    };
    InternalStep const on_ON_wattsPlus
    {
      on
    , Trigger(wattsPlus)
    , Guard([&]{return seconds.get() > 0;})
    , Output([&]()->LocalSignal<int16_t> const&{
        int const rawValWattsNxt{watts.get() + wattsStep};
        int const valWattsNxt{(rawValWattsNxt <= maxWattsSettable) ? rawValWattsNxt : maxWattsSettable};
        watts.nxt << static_cast<int16_t>(valWattsNxt);
        return wattsNxtSet(static_cast<int16_t>(valWattsNxt));
      })
    , Max1Flag()
    };
    InternalStep const on_ON_wattsMinus
    {
      on
    , Trigger(wattsMinus)
    , Guard([&]{return seconds.get() > 0;})
    , Output([&]()->LocalSignal<int16_t> const&{
        int const rawValWattsNxt{watts.get() - wattsStep};
        int const valWattsNxt{(rawValWattsNxt >= minWattsSettable) ? rawValWattsNxt : minWattsSettable};
        watts.nxt << static_cast<int16_t>(valWattsNxt);
        return wattsNxtSet(static_cast<int16_t>(valWattsNxt));
      })
    , Max1Flag()
    };

    FSM_LOCAL_SIGNAL(void, start, on);
    FSM_LOCAL_SIGNAL(void, stop, on);
    FSM_LOCAL_SIGNAL(void, exit, on);

    FSM_REGION(cookingChamber, on);
    FSM_INIT(cookingChamber);
    FSM_STATE(stopped, cookingChamber);
    FSM_STATE(paused, cookingChamber);
    FSM_STATE(cooking, cookingChamber);
    FSM_FINAL(cookingChamber);

    auto startCooking([&]{turnOnTurntable(); turnOnMagnetron(watts.get());});
    auto stopCooking([&]{turnOffMagnetron(); turnOffTurntable();});

    FSM_AUTO(cookingChamber_INIT, stopped);
    FSM_STEP
    (
      stopped
    , cooking
    , Trigger(startStop)
    , Guard([&]{return doorIsShut() && (seconds.get() >= 1);})
    , Action([&]{startCooking();}), Output(start)
    );
    FSM_STEP
    (
      stopped
    , cookingChamber_FINAL
    , Trigger(startStop)
    , Guard([&]{return seconds.get() == 0;})
    , Action([&]{soundBeep();})
    , Output(exit)
    );
    FSM_STEP
    (
      paused
    , cooking
    , Trigger(startStop)
    , Guard([&]{return doorIsShut() && (seconds.get() >= 1);})
    , Action([&]{startCooking();})
    , Output(start)
    );
    FSM_STEP
    (
      paused
    , cookingChamber_FINAL
    , Trigger(startStop)
    , Guard([&]{return seconds.get() == 0;})
    , Action([&]{soundBeep();}), Output(exit)
    );
    Step const cooking_TO_paused_BY_startStop
    (
      cooking
    , paused
    , Trigger(startStop)
    , Action([&]{stopCooking();})
    , Output(stop)
    );
    Step const cooking_TO_paused_BY_doorOpen
    (
      cooking
    , paused
    , Trigger(doorOpen)
    , Action([&]{stopCooking();})
    );
    FSM_INTERNAL_STEP
    (
      cooking
    , Trigger(tick)
    , Guard([&]{return seconds.get() >= 2;})
    , Output([&]()->LocalSignal<int16_t> const&{
        int16_t const valSecondsNxt{seconds.get() - 1};
        seconds.nxt << valSecondsNxt;
        return secondsNxtSet(valSecondsNxt);
      })
    , Max1Flag()
    );
    FSM_STEP
    (
      cooking
    , cookingChamber_FINAL
    , Trigger(tick)
    , Guard([&]{return seconds.get() <= 1;})
    , Action([&]{seconds.nxt << static_cast<int16_t>(seconds.get() - 1); stopCooking(); soundBeep();})
    , Output(exit)
    );

    FSM_REGION(light, on);
    FSM_INIT(light);
    FSM_STATE(lightOff, light);
    FSM_STATE(lightOn, light);
    FSM_FINAL(light);

    FSM_STEP(light_INIT, lightOn, Trigger(start), Trigger(doorOpen), Action([&]{turnOnLight();}));
    FSM_STEP(light_INIT, light_FINAL, Trigger(exit));
    FSM_STEP(lightOff, lightOn, Trigger(start), Trigger(doorOpen), Action([&]{turnOnLight();}));
    FSM_STEP(lightOff, light_FINAL, Trigger(exit));
    FSM_STEP(lightOn, lightOff, Trigger(stop), Trigger(doorShut), Action([&]{turnOffLight();}));
    FSM_STEP(lightOn, light_FINAL, Trigger(exit), Action([&]{turnOffLight();}));

    FSM_REGION(display, on);
    FSM_INIT(display);
    FSM_STATE(displayOn, display);
    FSM_FINAL(display);

    FSM_AUTO(display_INIT, displayOn);
    FSM_ENTER
    (
      displayOn
    , Action([&]{turnOnDisplay(); displayMinutesSeconds(seconds.get() / secondsPerMinute, seconds.get() % secondsPerMinute); displayWatts(watts.get());})
    );
    InternalStep const displayOn_INTERNAL_ON_secondsNxtSet
    (
      displayOn
    , Trigger(secondsNxtSet)
    , Action([&](Event const& secondsNxtSet){displayMinutesSeconds(secondsNxtSet.get<int16_t>() / secondsPerMinute, secondsNxtSet.get<int16_t>() % secondsPerMinute);})
    , Max1Flag()
    );
    InternalStep const displayOn_INTERNAL_ON_wattsNxtSet
    (
      displayOn
    , Trigger(wattsNxtSet)
    , Action([&](Event const& wattsNxtSet){displayWatts(wattsNxtSet.get<int16_t>());})
    , Max1Flag()
    );
    FSM_EXIT(displayOn, Action([&]{turnOffDisplay();}));
    FSM_STEP(displayOn, display_FINAL, Trigger(exit));

    // Tests
    microwave.init();

    auto const shortRun{[&]{
      // Pressing StartStop button without having set the duration does nothing
      microwave.step(startStop);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      // Notching the duration up twice
      microwave.step(durationPlus);
      microwave.step(durationPlus);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      // Notching the duration down three times
      microwave.step(durationMinus);
      microwave.step(durationMinus);
      microwave.step(durationMinus);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      // Notching the duration up
      microwave.step(durationPlus);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(startStop);
      ASSERT_RESET(sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT_RESET(sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT_RESET(sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(startStop);
      ASSERT(!sawTurnOnTurntable);
      ASSERT_RESET(sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT_RESET(sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT_RESET(sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(startStop);
      ASSERT_RESET(sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT_RESET(sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT_RESET(sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(doorOpen);
      statDoorShut = false;
      ASSERT(!sawTurnOnTurntable);
      ASSERT_RESET(sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT_RESET(sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(startStop);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(doorShut);
      statDoorShut = true;
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT_RESET(sawTurnOffLight);

      microwave.step(startStop);
      ASSERT_RESET(sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT_RESET(sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT_RESET(sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT(!sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT(!sawTurnOffMagnetron);
      ASSERT(!sawSoundBeep);
      ASSERT(!sawTurnOnLight);
      ASSERT(!sawTurnOffLight);

      microwave.step(tick);
      ASSERT(!sawTurnOnTurntable);
      ASSERT_RESET(sawTurnOffTurntable);
      ASSERT(!sawTurnOnMagnetron);
      ASSERT_RESET(sawTurnOffMagnetron);
      ASSERT_RESET(sawSoundBeep);
//      ASSERT(!sawTurnOnDisplay);
//      ASSERT_RESET(sawTurnOffDisplay);
      ASSERT(!sawTurnOnLight);
      ASSERT_RESET(sawTurnOffLight);
    }};
    shortRun();
    shortRun();
  }
  catch (Error & err)
  {
    cout << err.msg(); cout.flush();
    ASSERT(false);
  }
}

