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

#pragma once

#include <limits>

#include "ObjectId.hh"
#include "Set.hh"
#include "Vector.hh"
#include "MinMax.hh"
#include "Transition.hh"

namespace sta {

// Class declarations for pointer references.
class Graph;  // cdli
class Vertex;  // cdli
class Edge;  // cdli
class VertexIterator;
class VertexInEdgeIterator;
class VertexOutEdgeIterator;
class GraphLoop;
class VertexSet;

typedef ObjectId VertexId;
typedef ObjectId EdgeId;
typedef ObjectId ArcId;
typedef Vector<Vertex*> VertexSeq;  // cdli
typedef Vector<Edge*> EdgeSeq;
typedef Set<Edge*> EdgeSet;
typedef int Level;  // cdli
typedef int DcalcAPIndex;
typedef int TagGroupIndex;
typedef Vector<GraphLoop*> GraphLoopSeq;

static constexpr int level_max = std::numeric_limits<Level>::max();  // cdli

// 16,777,215 tags
static const int tag_group_index_bits = 24;
static const TagGroupIndex tag_group_index_max = (1<<tag_group_index_bits)-1;
static const int slew_annotated_bits = MinMax::index_count * RiseFall::index_count;

// Bit shifts used to mark vertices in a Bfs queue.
enum class BfsIndex { dcalc, arrival, required, other, bits };  // cdli

} // namespace
