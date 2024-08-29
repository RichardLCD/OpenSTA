%{

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

#include "util/FlexDisableRegister.hh"
#include "Debug.hh"
#include "VerilogNamespace.hh"
#include "verilog/VerilogReaderPvt.hh"
#include "VerilogParse.hh"

#define YY_NO_INPUT

int verilog_line = 1;  // cdli
static std::string string_buf;  // cdli

void
verilogFlushBuffer()  // cdli
{
  YY_FLUSH_BUFFER;
}

%}

/* %option debug */
%option noyywrap
%option nounput
%option never-interactive

%x COMMENT
%x QSTRING

SIGN	"+"|"-"
UNSIGNED_NUMBER [0-9][0-9_]*
BLANK	[ \t\r]
EOL	\r?\n
ID_ESCAPED_TOKEN \\[^ \t\r\n]+[\r\n\t ]
ID_ALPHA_TOKEN [A-Za-z_][A-Za-z0-9_$]*
ID_TOKEN {ID_ESCAPED_TOKEN}|{ID_ALPHA_TOKEN}

%%

^[ \t]*`.*{EOL} { /* Macro definition. */  // cdli
	sta::verilog_reader->incrLine();
	}

"//"[^\n]*{EOL} { /* Single line comment. */  // cdli
	sta::verilog_reader->incrLine();
	}

"/*"	{ BEGIN COMMENT; }  // cdli
<COMMENT>{  // cdli
.  // cdli

{EOL}	{ sta::verilog_reader->incrLine(); }  // cdli

"*/"	{ BEGIN INITIAL; }  // cdli

<<EOF>> {  // cdli
	VerilogParse_error("unterminated comment");
	BEGIN(INITIAL);
	yyterminate();
	}
}

{SIGN}?{UNSIGNED_NUMBER}?"'"[bB][01_xz]+ {  // cdli
  VerilogParse_lval.constant = sta::stringCopy(VerilogLex_text);
  return CONSTANT;
}

{SIGN}?{UNSIGNED_NUMBER}?"'"[oO][0-7_xz]+ {  // cdli
  VerilogParse_lval.constant = sta::stringCopy(VerilogLex_text);
  return CONSTANT;
}

{SIGN}?{UNSIGNED_NUMBER}?"'"[dD][0-9_]+ {  // cdli
  VerilogParse_lval.constant = sta::stringCopy(VerilogLex_text);
  return CONSTANT;
}

{SIGN}?{UNSIGNED_NUMBER}?"'"[hH][0-9a-fA-F_xz]+ {  // cdli
  VerilogParse_lval.constant = sta::stringCopy(VerilogLex_text);
  return CONSTANT;
}

{SIGN}?[0-9]+ {  // cdli
  VerilogParse_lval.ival = atol(VerilogLex_text);
  return INT;
}

":"|"."|"{"|"}"|"["|"]"|","|"*"|";"|"="|"-"|"+"|"|"|"("|")" {  // cdli
  return ((int) VerilogLex_text[0]);
}

"(*" { return ATTRIBUTE_OPEN; }  // cdli
"*)" { return ATTRIBUTE_CLOSED; }  // cdli
assign { return ASSIGN; }  // cdli
endmodule { return ENDMODULE; }  // cdli
inout { return INOUT; }  // cdli
input { return INPUT; }  // cdli
module { return MODULE; }  // cdli
output { return OUTPUT; }  // cdli
parameter { return PARAMETER; }  // cdli
defparam { return DEFPARAM; }  // cdli
reg { return REG; }  // cdli
supply0 { return SUPPLY0; }  // cdli
supply1 { return SUPPLY1; }  // cdli
tri { return TRI; }  // cdli
wand { return WAND; }  // cdli
wire { return WIRE; }  // cdli
wor { return WOR; }  // cdli

{ID_TOKEN}("."{ID_TOKEN})* {  // cdli
	VerilogParse_lval.string = sta::stringCopy(VerilogLex_text);
	return ID;
}

{EOL}	{ sta::verilog_reader->incrLine(); }  // cdli

{BLANK}	{ /* ignore blanks */ }  // cdli

\"	{  // cdli
	string_buf.erase();
	BEGIN(QSTRING);
	}

<QSTRING>\" {  // cdli
	BEGIN(INITIAL);
	VerilogParse_lval.string = sta::stringCopy(string_buf.c_str());
	return STRING;
	}

<QSTRING>{EOL} {  // cdli
	VerilogParse_error("unterminated string constant");
	BEGIN(INITIAL);
	VerilogParse_lval.string = sta::stringCopy(string_buf.c_str());
	return STRING;
	}

<QSTRING>\\{EOL} {  // cdli
	/* Line continuation. */
	sta::verilog_reader->incrLine();
	}

<QSTRING>[^\r\n\"]+ {  // cdli
	/* Anything return or double quote */
	string_buf += VerilogLex_text;
	}

<QSTRING><<EOF>> {  // cdli
	VerilogParse_error("unterminated string constant");
	BEGIN(INITIAL);
	yyterminate();
	}

	/* Send out of bound characters to parser. */
.	{ return (int) VerilogLex_text[0]; }  // cdli

%%
