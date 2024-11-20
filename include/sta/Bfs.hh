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

#include <mutex>

#include "Iterator.hh"
#include "Set.hh"
#include "GraphClass.hh"
#include "VertexVisitor.hh"
#include "StaState.hh"

namespace sta {

class SearchPred;  // cdli
class BfsFwdIterator;  // cdli
class BfsBkwdIterator;  // cdli

// LevelQueue is a vector of vertex vectors indexed by logic level.
typedef Vector<VertexSeq> LevelQueue;  // cdli

// Abstract base class for forward and backward breadth first search iterators.
// Visit all of the vertices at a level before moving to the next.
// Use enqueue to seed the search.
// Use enqueueAdjacentVertices as a vertex is visited to queue the
// fanout vertices filtered by the search predicate.
//
// Vertices are marked as being in the queue by using a flag on
// the vertex indexed by bfs_index.  A unique flag is only needed
// if the BFS in in use when other BFS's are simultaneously in use.
class BfsIterator : public StaState, Iterator<Vertex*>
{
public:
  virtual ~BfsIterator();  // cdli
  // Make sure that the BFS queue is deep enough for the max logic level.
  void ensureSize();
  // Reset to virgin state.
  void clear();
  bool empty() const;  // cdli
  // Enqueue a vertex to search from.
  void enqueue(Vertex *vertex);  // cdli
  // Enqueue vertices adjacent to a vertex.
  void enqueueAdjacentVertices(Vertex *vertex);
  void enqueueAdjacentVertices(Vertex *vertex,
			       SearchPred *search_pred);
  void enqueueAdjacentVertices(Vertex *vertex,
			       Level to_level);
  virtual void enqueueAdjacentVertices(Vertex *vertex,
				       SearchPred *search_pred,
				       Level to_level) = 0;
  bool inQueue(Vertex *vertex);  // cdli
  void checkInQueue(Vertex *vertex);  // cdli
  // Notify iterator that vertex will be deleted.
  void deleteVertexBefore(Vertex *vertex);
  void remove(Vertex *vertex);
  void reportEntries(const Network *network);  // cdli

  virtual bool hasNext();
  bool hasNext(Level to_level);
  virtual Vertex *next();

  // Apply visitor to all vertices in the queue in level order.
  // Returns the number of vertices that are visited.
  virtual int visit(Level to_level,
		    VertexVisitor *visitor);
  // Apply visitor to all vertices in the queue in level order,
  // using threads to parallelize the visits. visitor must be thread safe.
  // Returns the number of vertices that are visited.
  int visitParallel(Level to_level,
		    VertexVisitor *visitor);

protected:
  BfsIterator(BfsIndex bfs_index,
	      Level level_min,
	      Level level_max,
	      SearchPred *search_pred,
	      StaState *sta);
  void init();
  void deleteEntries(Level level);
  virtual bool levelLess(Level level1,
			 Level level2) const = 0;  // cdli
  virtual bool levelLessOrEqual(Level level1,
				Level level2) const = 0;  // cdli
  virtual void incrLevel(Level &level) = 0;  // cdli
  void findNext(Level to_level);
  void deleteEntries();

  BfsIndex bfs_index_;  // cdli
  Level level_min_;  // cdli
  Level level_max_;  // cdli
  SearchPred *search_pred_;  // cdli
  LevelQueue queue_;  // cdli
  std::mutex queue_lock_;  // cdli
  // Min (max) level of queued vertices.
  Level first_level_;  // cdli
  // Max (min) level of queued vertices.
  Level last_level_;  // cdli

  friend class BfsFwdIterator;  // cdli
  friend class BfsBkwdIterator;  // cdli
};

class BfsFwdIterator : public BfsIterator
{
public:
  BfsFwdIterator(BfsIndex bfs_index,
		 SearchPred *search_pred,
		 StaState *sta);
  virtual ~BfsFwdIterator();
  virtual void enqueueAdjacentVertices(Vertex *vertex,
				       SearchPred *search_pred,
				       Level to_level);
  using BfsIterator::enqueueAdjacentVertices;

protected:
  virtual bool levelLessOrEqual(Level level1,
				Level level2) const;  // cdli
  virtual bool levelLess(Level level1,
			 Level level2) const;  // cdli
  virtual void incrLevel(Level &level);  // cdli
};

class BfsBkwdIterator : public BfsIterator
{
public:
  BfsBkwdIterator(BfsIndex bfs_index,
		  SearchPred *search_pred,
		  StaState *sta);
  virtual ~BfsBkwdIterator();
  virtual void enqueueAdjacentVertices(Vertex *vertex,
				       SearchPred *search_pred,
				       Level to_level);
  using BfsIterator::enqueueAdjacentVertices;

protected:
  virtual bool levelLessOrEqual(Level level1,
				Level level2) const;  // cdli
  virtual bool levelLess(Level level1,
			 Level level2) const;  // cdli
  virtual void incrLevel(Level &level);  // cdli
};

} // namespace
