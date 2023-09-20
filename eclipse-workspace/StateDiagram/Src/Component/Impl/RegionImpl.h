/*   
   (c) Copyright 2019-2021 State Diagram Contributors

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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_REGIONIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_REGIONIMPL_H_

#ifndef STATE_DIAGRAM_STRINGLESS
#include <set>
#endif // STATE_DIAGRAM_STRINGLESS

#include "StackSeq/StackSeq_all.h"
#include "Util/ForwardList.hpp"
#include "CompoundStateImpl.h"
#include "ExecStat.h"
#include "LocalScope.h"
#include "SubComponent.hpp"
#include "SubStateImpl.h"

namespace state_diagram
{

class InitStateImpl;
class StateImpl;

class RegionImpl
:
  public SubComponent<CompoundStateImpl>
, public LocalScope
{
private:
  InitStateImpl * m_initState;
  ForwardList<SubStateImpl * const> m_subStates;
#ifndef STATE_DIAGRAM_STRINGLESS
  set<string> m_subStateNames;
#endif // STATE_DIAGRAM_STRINGLESS

  SubStateImpl * m_current;

public:
  RegionImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundStateImpl * const parent);
  RegionImpl(RegionImpl const &) = delete;

  void operator=(RegionImpl const &) = delete;

#ifndef STATE_DIAGRAM_STRINGLESS
  using NamePathImpl::path;
#endif // STATE_DIAGRAM_STRINGLESS

  CompoundStateImpl * parentCompoundState() const;
  StateImpl * parentState() const;

  void insertInitState(STATE_DIAGRAM_STRING_PARAM_COMMA(name) InitStateImpl * const initState);
  void insertSubState(STATE_DIAGRAM_STRING_PARAM_COMMA(name) SubStateImpl * const subState);

  bool isParentOf(SubStateImpl const * const subState) const;

  unique_ptr<StackSeq::Construct const>
  makeStackSeqConstruct
  (
    TargetStateImpl * const target
  , RegionImpl * const sourceRegion
  );

  void init();
  void shallowInit();
  void finalize() const;
  bool hasTerminated() const;
  void descendToTarget(StackSeq::Construct const * const stackSeq);
  void setAsCurrent(SubStateImpl * const target);
  ExecStat exec();
  void reload() const;

  bool hasAsCurrent(SubStateImpl const * const subState) const;

private:
  void forEachSubState(function<void (SubStateImpl * const)> const & f) const;
  void forEachCSubState(function<void (SubStateImpl const * const)> const & f) const;

#ifndef STATE_DIAGRAM_STRINGLESS
  void throwSignalNameClashError(string const & signalName) const override;
  void throwVarNameClashError(string const & varName) const override;
#endif // STATE_DIAGRAM_STRINGLESS
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_REGIONIMPL_H_
