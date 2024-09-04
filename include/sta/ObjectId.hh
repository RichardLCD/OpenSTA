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

#include <cstdint>

namespace sta {

// ObjectId is block index and object index within the block.
typedef uint32_t ObjectId;  // cdli
// Block index.
typedef uint32_t BlockIdx;  // cdli
// Object index within a block.
typedef uint32_t ObjectIdx;  // cdli

static constexpr int object_id_bits = sizeof(ObjectId) * 8;  // cdli
static constexpr BlockIdx block_idx_null = 0;  // cdli
static constexpr ObjectId object_id_null = 0;  // cdli
static constexpr ObjectIdx object_idx_null = 0;  // cdli

} // Namespace
