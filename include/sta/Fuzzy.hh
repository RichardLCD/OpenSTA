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

namespace sta {

// "Fuzzy" floating point comparisons that allow some tolerance.

bool
fuzzyEqual(float v1,
	   float v2);  // cdli
bool
fuzzyZero(float v);  // cdli
bool
fuzzyLess(float v1,
	  float v2);  // cdli
bool
fuzzyLessEqual(float v1,
	       float v2);  // cdli
bool
fuzzyGreater(float v1,
	     float v2);  // cdli
bool
fuzzyGreaterEqual(float v1,
		  float v2);  // cdli
bool
fuzzyInf(float value);  // cdli

} // namespace
