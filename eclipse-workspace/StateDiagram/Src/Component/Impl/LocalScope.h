/*   
   (c) Copyright 2019-2021 State Diagram Contributors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LhICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_LOCALSCOPE_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_LOCALSCOPE_H_

#include "state_diagram/state_diagram.h"

#ifndef STATE_DIAGRAM_STRINGLESS
#include <set>
#endif // STATE_DIAGRAM_STRINGLESS

#include "Util/ListAlgorithm.hpp"
#include "NamePathImpl.h"

namespace state_diagram
{

class LocalSignalDelegateImpl;
class LocalVarDelegateImpl;
class TopStateImpl;

class LocalScope
:
  public virtual NamePathImpl
{
protected:
  LocalScope(STATE_DIAGRAM_STRING_PARAM_COMMA(name) LocalScope const * const up, TopStateImpl * const topState);
private:
  LocalScope(LocalScope const &) = delete;

  void operator=(LocalScope const &) = delete;

public:
  void insertLocalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) LocalSignalDelegateImpl * const localSignal);
  bool hasInScope(LocalSignalDelegateImpl const * const localSignal) const;

  void insertLocalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(name) LocalVarDelegateImpl * const localVar);
  bool hasInScope(LocalVarDelegateImpl const * const localVar) const;

  TopStateImpl * const topState;

protected:
  void unsetLocalVars() const;
  void reload() const;

#ifndef STATE_DIAGRAM_STRINGLESS
  virtual
  void
  throwSignalNameClashError
  (
    string const & signalName
  )
  const = 0;
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  virtual
  void
  throwVarNameClashError
  (
    string const & varName
  )
  const = 0;
#endif // STATE_DIAGRAM_STRINGLESS

private:
  LocalScope const * const m_up;

#ifndef STATE_DIAGRAM_STRINGLESS
  set<string> m_localSignalNames;
  set<string> m_localVarNames;
#endif // STATE_DIAGRAM_STRINGLESS
  ForwardList<LocalSignalDelegateImpl * const> m_localSignals;
  ForwardList<LocalVarDelegateImpl * const> m_localVars;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_LOCALSCOPE_H_
