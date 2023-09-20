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

#include "ExternalTransitionImpl.h"

#include <cassert>

namespace state_diagram
{

ExternalTransitionImpl
::ExternalTransitionImpl(SourceStateImpl * const _source, TargetStateImpl * const _target)
:
  source{_source}
, target{_target}
, m_stackSeq{makeStackSeqCheckColocality()}
, m_outputScope{computeOutputScope()}
{
  // This space intentionally left empty
}

SubStateImpl const *
ExternalTransitionImpl
::outputScope()
const
{
  return m_outputScope;
}

SourceStateImpl *
ExternalTransitionImpl
::origin()
const
{
  return source;
}

unique_ptr<StackSeq const>
ExternalTransitionImpl
::makeStackSeqCheckColocality()
const
{
  if (source->parentRegion() == target->parentRegion())
  {
    return make_unique<StackSeq::Unwind::Void const>();
  }
  assert (source->parentRegion() != target->parentRegion());
  if (target->isDeepMemberOf(source->parentRegion()))
  {
    return target->parentRegion()->makeStackSeqConstruct(target, source->parentRegion());
  }
  if (source->isDeepMemberOf(target->parentRegion()))
  {
    return make_unique<StackSeq::Unwind::Do const>(target);
  }
  // Co-locality violated if none of the two parent regions is enclosed within the other.
#ifndef STATE_DIAGRAM_STRINGLESS
  throw ExternalTransition::DescendanceError(source->path(), target->path());
#else
  STATE_DIAGRAM_HANDLE_ERROR(ExternalTransition::descendanceError);
#endif // STATE_DIAGRAM_STRINGLESS
}

SubStateImpl *
ExternalTransitionImpl
::computeOutputScope()
const
{
  // It's the more nested parent region that constitutes the output scope.
  if (target->isDeepMemberOf(source->parentRegion()))
  {
    return target;
  }
  if (source->isDeepMemberOf(target->parentRegion()))
  {
    return source;
  }
  assert (false); // Colocality check already carried out by way of generating stack sequence.
}

void
ExternalTransitionImpl
::setCompletionFlag()
{
  auto const guard{Guard([&]{return source->hasTerminated();})};
  add(&guard);
}

ExecStat
ExternalTransitionImpl
::exec()
{
  UnwindCmd const unwindCmd{
    m_stackSeq->accept
    (
      [](StackSeq::Construct const * const ssc)
      {
        ssc->exec();
        return UnwindCmd{};
      }
    , [&](StackSeq::Unwind const * const ssu) -> UnwindCmd
      {
        return
          ssu->accept
          (
            [&]() -> UnwindCmd
            {
              this->target->becomeCurrent();
              return UnwindCmd{};
            }
          , [](StackSeq::Unwind::Do const * const ssud) -> UnwindCmd
            {
              return UnwindCmd{ssud->target};
            }
          );
      }
    )
  };
  return ExecStat{unwindCmd};
}

} // namespace state_diagram
