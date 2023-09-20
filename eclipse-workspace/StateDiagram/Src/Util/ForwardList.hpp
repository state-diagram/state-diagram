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

#ifndef STATE_DIAGRAM_UTIL_FORWARDLIST_HPP_
#define STATE_DIAGRAM_UTIL_FORWARDLIST_HPP_

#include <iterator>
#include <memory>

#include "state_diagram/state_diagram_error.h"

namespace state_diagram
{

using namespace std;

template<typename Item>
class ForwardList
{
private:
  struct Node
  {
    template<class... Args>
    Node(Node * const _next, Args &&... args)
    :
      item{forward<Args>(args)...}
    , next{_next}
    {
      // This space intentionally left empty
    }

    Node(Node const &) = delete;

    void operator=(Node const &) = delete;

    Item item;
    unique_ptr<Node> const next;
  };

  unique_ptr<Node> m_first;

public:
  ForwardList()
  :
    m_first{nullptr}
  {
    // This space intentionally left empty
  }

  ForwardList(ForwardList const &) = delete;

  void operator=(ForwardList const &) = delete;

  template<class... Args>
  void emplace_front(Args &&... args)
  {
    m_first = make_unique<Node>(m_first.release(), forward<Args>(args)...);
  }

  template<bool isConst = false>
  class Iterator
  {
  public:
      using iterator_category = conditional_t<isConst, output_iterator_tag, input_iterator_tag>;
      using difference_type   = ptrdiff_t;
      using value_type        = conditional_t<isConst, Item const, Item>;
      using pointer           = conditional_t<isConst, Item const, Item> *;
      using reference         = conditional_t<isConst, Item const, Item> &;

      Iterator(Node * const _node)
      :
        node(_node)
      {
        // This space intentionally left empty
      }

      Iterator(Iterator const & other) = default;

      Iterator & operator=(Iterator const & other) = default;

      reference
      operator*()
      const
      {
        return node->item;
      }

      pointer
      operator->()
      {
        return &node->item;
      }

      Iterator &
      operator++()
      {
        node = node->next.get();
        return *this;
      }

      Iterator
      operator++(int)
      {
        Iterator const unIncremented{*this};
        ++*this;
        return unIncremented;
      }

      friend
      bool
      operator==(Iterator const & lft, Iterator const & rgt)
      {
        return lft.node == rgt.node;
      };

      friend
      bool
      operator!=(Iterator const & lft, Iterator const & rgt)
      {
        return lft.node != rgt.node;
      };

  private:
      conditional_t<isConst, Node const, Node> * node;
  };

private:
  static bool constexpr isConst{true};
  static bool constexpr isNonConst{false};

public:
  using iterator = Iterator<isNonConst>;
  using const_iterator = Iterator<isConst>;

  iterator
  begin()
  const
  {
    return iterator{m_first.get()};
  }

  iterator
  end()
  const
  {
    return iterator{nullptr};
  }

  const_iterator
  cbegin()
  const
  {
    return const_iterator{m_first.get()};
  }

  const_iterator
  cend()
  const
  {
    return const_iterator{nullptr};
  }

  size_t
  size()
  const noexcept
  {
    size_t szSoFar{0};
    for (Node const * node = m_first.get(); node != nullptr; node = node->next.get())
    {
      ++szSoFar;
    }
    return szSoFar;
  }

  bool
  empty()
  const
  {
    return size() == 0;
  }
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_UTIL_FORWARDLIST_HPP_

