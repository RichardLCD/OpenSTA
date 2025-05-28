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

#include "LibertyLocation.hh"  // cdli
#include "LibertyParse.hh"  // cdli

#ifndef __FLEX_LEXER_H  // cdli
#undef yyFlexLexer  // cdli
#define yyFlexLexer LibertyFlexLexer  // cdli
#include <FlexLexer.h>  // cdli
#endif

namespace sta {  // cdli

class Report;  // cdli
class LibertyParser;  // cdli

class LibertyScanner : public LibertyFlexLexer  // cdli
{
public:
  LibertyScanner(std::istream *stream,
                 const char *filename,
                 LibertyParser *reader,
                 Report *report);  // cdli
  virtual ~LibertyScanner() {}  // cdli

  virtual int lex(LibertyParse::semantic_type *const yylval,
                  LibertyParse::location_type *yylloc);  // cdli
  // YY_DECL defined in LibertyLex.ll
  // Method body created by flex in LibertyLex.cc

  // Get rid of override virtual function warning.
  using FlexLexer::yylex;  // cdli

private:
  bool includeBegin();  // cdli
  void fileEnd();  // cdli
  void error(const char *msg);  // cdli

  std::istream *stream_;  // cdli
  std::string filename_;  // cdli
  LibertyParser *reader_;
  Report *report_;
  std::string token_;

  // Previous lex state for include files.
  std::string filename_prev_;  // cdli
  std::istream *stream_prev_;  // cdli
};

} // namespace
