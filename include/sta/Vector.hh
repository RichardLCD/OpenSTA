// OpenSTA, Static Timing Analyzer
// Copyright (c) 2025, Parallax Software, Inc.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
// 
// The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.
// 
// Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 
// This notice may not be removed or altered from any source distribution.

#pragma once  // cdli

#include <vector>  // cdli
#include <algorithm>  // cdli

namespace sta {  // cdli

// Add convenience functions around STL container.
template <class OBJ>
class Vector : public std::vector<OBJ>  // cdli
{
public:
  Vector() : std::vector<OBJ>() {}  // cdli
  Vector(size_t n) : std::vector<OBJ>(n) {}  // cdli
  Vector(size_t n, const OBJ &obj) : std::vector<OBJ>(n, obj) {}  // cdli

  // Erase an object from the vector (slow).
  void
  eraseObject(OBJ obj)  // cdli
  {
    auto find_iter = std::find(this->begin(), this->end(), obj);
    if (find_iter != this->end())
      this->erase(find_iter);
  }

  void
  deleteContents()  // cdli
  {
    Iterator iter(this);
    while (iter.hasNext())
      delete iter.next();
  }

  void
  deleteContentsClear()  // cdli
  {
    deleteContents();
    this->clear();
  }

  void
  deleteArrayContentsClear()  // cdli
  {
    Iterator iter(this);
    while (iter.hasNext())
      delete [] iter.next();
    this->clear();
  }

  // Java style container itererator
  //  Vector::Iterator<Object*> iter(vector);
  //  while (iter.hasNext()) {
  //    Object *v = iter.next();
  //  }
  class Iterator  // cdli
  {
  public:
    Iterator() : container_(nullptr) {}  // cdli
    Iterator(std::vector<OBJ> *container) :
      container_(container)  // cdli
    { if (container) iter_ = container->begin(); }
    Iterator(std::vector<OBJ> &container) :
      container_(&container)  // cdli
    { if (container_) iter_ = container_->begin(); }
    void init() { iter_ = container_->begin(); }  // cdli
    void init(std::vector<OBJ> *container)  // cdli
    { container_ = container; if (container_) iter_=container_->begin(); }
    void init(std::vector<OBJ> &container)  // cdli
    { container_ = &container; iter_ = container_->begin(); }
    bool hasNext() { return container_ && iter_ != container_->end(); }  // cdli
    OBJ& next() { return *iter_++; }  // cdli
    std::vector<OBJ> *container() { return container_; }  // cdli

  private:
    std::vector<OBJ> *container_;  // cdli
    typename std::vector<OBJ>::iterator iter_;  // cdli
  };

  class ConstIterator  // cdli
  {
  public:
    ConstIterator() : container_(nullptr) {}  // cdli
    ConstIterator(const std::vector<OBJ> *container) :
      container_(container)  // cdli
    { if (container_) iter_ = container_->begin(); }
    ConstIterator(const std::vector<OBJ> &container) :
      container_(&container)  // cdli
    { iter_ = container_->begin(); }
    void init() { iter_ = container_->begin(); }  // cdli
    void init(const std::vector<OBJ> *container)  // cdli
    { container_ = container; if (container_) iter_=container_->begin();}
    void init(const std::vector<OBJ> &container)  // cdli
    { container_ = &container; if (container_) iter_=container_->begin();}
    bool hasNext() { return container_ && iter_ != container_->end(); }  // cdli
    const OBJ& next() { return *iter_++; }  // cdli
    const std::vector<OBJ> *container() { return container_; }  // cdli

  private:
    const std::vector<OBJ> *container_;  // cdli
    typename std::vector<OBJ>::const_iterator iter_;  // cdli
  };
};

template <class OBJ, class SortCmp>
void
sort(Vector<OBJ> &seq, SortCmp cmp)  // cdli
{
  // For some strange reason std::sort goes off into never never land
  // when optimization is turned on in gcc.
  std::stable_sort(seq.begin(), seq.end(), cmp);
}

template <class OBJ, class SortCmp>
void
sort(Vector<OBJ> *seq, SortCmp cmp)  // cdli
{
  // For some strange reason std::sort goes off into never never land
  // when optimization is turned on in gcc.
  std::stable_sort(seq->begin(), seq->end(), cmp);
}

} // namespace sta
