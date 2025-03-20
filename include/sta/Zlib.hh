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

// The zlib package is optional.
// Define stdio based macros if it is missing.

#include "StaConfig.hh"  // ZLIB  // cdli

#ifdef ZLIB_FOUND  // cdli

#include <zlib.h>  // cdli
#include "util/gzstream.hh"  // cdli

#else // ZLIB_FOUND  // cdli

#include <cstdio>  // cdli
#include <fstream>  // cdli

#define gzFile FILE*  // cdli
#define gzopen fopen  // cdli
#define gzclose fclose  // cdli
#define gzgets(stream,s,size) fgets(s,size,stream)  // cdli
#define gzprintf fprintf  // cdli
#define Z_NULL nullptr  // cdli

namespace gzstream {  // cdli
typedef std::ifstream igzstream;  // cdli
}

#endif // ZLIB_FOUND
