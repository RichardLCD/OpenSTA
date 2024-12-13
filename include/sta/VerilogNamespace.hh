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

#include <string>  // cdli

namespace sta {  // cdli

using std::string;  // cdli

string
instanceVerilogName(const char *sta_name,
		    const char escape);  // cdli
string
netVerilogName(const char *sta_name,
	       const char escape);  // cdli
string
portVerilogName(const char *sta_name,
		const char escape);  // cdli

string
moduleVerilogToSta(const char *sta_name);  // cdli
string
instanceVerilogToSta(const char *sta_name);  // cdli
string
netVerilogToSta(const char *sta_name);  // cdli
string
portVerilogToSta(const char *sta_name);  // cdli

} // namespace
