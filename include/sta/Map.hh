// OpenSTA, Static Timing Analyzer
// Copyright (c) 2024, Parallax Software, Inc.
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

#pragma once  // cdli

#include <map>
#include <algorithm>

namespace sta {

// Add convenience functions around STL container.
template <class KEY, class VALUE, class CMP = std::less<KEY> >
class Map : public std::map<KEY, VALUE, CMP>  // cdli
{
public:
  Map() :
    std::map<KEY, VALUE, CMP>()  // cdli
  {
  }
  explicit Map(const CMP &cmp) :
    std::map<KEY, VALUE, CMP>(cmp)  // cdli
  {
  }

  // Find out if key is in the set.
  bool
  hasKey(const KEY key) const  // cdli
  {
    return this->find(key) != this->end();
  }

  // Find the value corresponding to key.
  VALUE
  findKey(const KEY key) const  // cdli
  {
    auto find_iter = this->find(key);
    if (find_iter != this->end())
      return find_iter->second;
    else
      return nullptr;
  }
  void
  findKey(const KEY key,
	  // Return Values.
	  VALUE &value,
	  bool &exists) const  // cdli
  {
    auto find_iter = this->find(key);
    if (find_iter != this->end()) {
      value = find_iter->second;
      exists = true;
    }
    else
      exists = false;
  }
  void
  findKey(const KEY &key,
	  // Return Values.
	  KEY &map_key,
	  VALUE &value,
	  bool &exists) const  // cdli
  {
    auto find_iter = this->find(key);
    if (find_iter != this->end()) {
      map_key = find_iter->first;
      value = find_iter->second;
      exists = true;
    }
    else
      exists = false;
  }

  void
  insert(const KEY &key,
	 VALUE value)  // cdli
  {
    this->operator[](key) = value;
  }

  void
  deleteContents()  // cdli
  {
    Iterator iter(this);
    while (iter.hasNext())
      delete iter.next();
  }

  void
  deleteKeysContents()  // cdli
  {
    for (const auto [key, value] : this) {
      delete key;
      delete value;
    }
  }

  void
  deleteArrayContents()  // cdli
  {
    Iterator iter(this);
    while (iter.hasNext())
      delete [] iter.next();
  }

  void
  deleteContentsClear()  // cdli
  {
    deleteContents();
    std::map<KEY, VALUE, CMP>::clear();
  }

  // Java style container itererator
  //  Map::Iterator<string *, Value, stringLess> iter(map);
  //  while (iter.hasNext()) {
  //    Value *v = iter.next();
  //  }
  class Iterator  // cdli
  {
  public:
    Iterator() : container_(nullptr) {}  // cdli
    explicit Iterator(std::map<KEY, VALUE, CMP> *container) :
      container_(container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    explicit Iterator(std::map<KEY, VALUE, CMP> &container) :
      container_(&container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    void init(std::map<KEY, VALUE, CMP> *container)  // cdli
    { container_ = container; if (container_ != nullptr) iter_=container_->begin();}
    void init(std::map<KEY, VALUE, CMP> &container)  // cdli
    { container_ = &container; if (container_ != nullptr) iter_=container_->begin();}
    bool hasNext() { return container_ != nullptr && iter_ != container_->end(); }  // cdli
    VALUE next() { return iter_++->second; }  // cdli
    void next(KEY &key,
	      VALUE &value)  // cdli
    { key = iter_->first; value = iter_->second; iter_++; }
    std::map<KEY, VALUE, CMP> *container() { return container_; }  // cdli

  private:
    std::map<KEY, VALUE, CMP> *container_;  // cdli
    typename std::map<KEY, VALUE, CMP>::iterator iter_;  // cdli
  };

  class ConstIterator  // cdli
  {
  public:
    ConstIterator() : container_(nullptr) {}  // cdli
    explicit ConstIterator(const std::map<KEY, VALUE, CMP> *container) :
      container_(container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    explicit ConstIterator(const std::map<KEY, VALUE, CMP> &container) :
      container_(&container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    void init(const std::map<KEY, VALUE, CMP> *container)  // cdli
    { container_ = container; if (container_ != nullptr) iter_=container_->begin();}
    void init(const std::map<KEY, VALUE, CMP> &container)  // cdli
    { container_ = &container; if (container_ != nullptr) iter_=container_->begin();}
    bool hasNext() { return container_ != nullptr && iter_ != container_->end(); }  // cdli
    VALUE next() { return iter_++->second; }  // cdli
    void next(KEY &key,
	      VALUE &value)  // cdli
    { key = iter_->first; value = iter_->second; iter_++; }
    const std::map<KEY, VALUE, CMP> *container() { return container_; }  // cdli

  private:
    const std::map<KEY, VALUE, CMP> *container_;  // cdli
    typename std::map<KEY, VALUE, CMP>::const_iterator iter_;  // cdli
  };
};

} // namespace
