%{  // cdli
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

#include <ctype.h>  // cdli
#include <string.h>  // cdli

#include "util/FlexDisableRegister.hh"  // cdli
#include "liberty/LibertyParser.hh"  // cdli
#include "LibertyParse.hh"  // cdli
#include "liberty/LibertyScanner.hh"  // cdli

#undef YY_DECL  // cdli
#define YY_DECL \
int \
sta::LibertyScanner::lex(sta::LibertyParse::semantic_type *const yylval, \
                         sta::LibertyParse::location_type *loc)  // cdli

// update location on matching
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);  // cdli

typedef sta::LibertyParse::token token;  // cdli

%}

/* cdli */
%option c++
%option yyclass="sta::LibertyScanner"
%option prefix="Liberty"
%option noyywrap
%option never-interactive
%option stack
%option yylineno
/* %option debug */

/* cdli */
%x comment
%x qstring

/* cdli */
DIGIT [0-9]
ALPHA [a-zA-Z]
FLOAT [-+]?(({DIGIT}+\.?{DIGIT}*)|(\.{DIGIT}+))([Ee][-+]?{DIGIT}+)?
BLANK [ \t]
BUS_LEFT [\[<]
BUS_RIGHT [\]>]
BUS_SUB {BUS_LEFT}{DIGIT}+{BUS_RIGHT}
BUS_RANGE {BUS_LEFT}{DIGIT}+:{DIGIT}+{BUS_RIGHT}
PIN_NAME ({ALPHA}|_)({ALPHA}|{DIGIT}|_)*!?
BUS_NAME {PIN_NAME}({BUS_SUB}|{BUS_RANGE})
BUS_NAME2 {PIN_NAME}{BUS_SUB}({BUS_SUB}|{BUS_RANGE})
MIXED_NAME {BUS_NAME}_{PIN_NAME}
HNAME ({PIN_NAME}|{BUS_NAME}|{MIXED_NAME})([\/.]({PIN_NAME}|{BUS_NAME}|{MIXED_NAME}))+
/* ocv_table_template(2D_ocv_template) */
/* leakage_power_unit             : 1pW; */
/* default_operating_conditions : slow_100_3.00 ; */
/* revision : 1.0.17; */
/* default_wire_load : xc2v250-5_avg; */
TOKEN ({ALPHA}|{DIGIT}|_)({ALPHA}|{DIGIT}|[._\-])*(:({ALPHA}|{DIGIT}|_)+)?
/* bus_naming_style : %s[%d] ; */
BUS_STYLE "%s"{BUS_LEFT}"%d"{BUS_RIGHT}
PUNCTUATION [,\:;|(){}+*&!'=]
TOKEN_END {PUNCTUATION}|[ \t\r\n]
EOL \r?\n
%%

{PUNCTUATION} { return ((int) yytext[0]); }  // cdli

{FLOAT}{TOKEN_END} {  // cdli
	/* Push back the TOKEN_END character. */
	yyless(yyleng - 1);
	yylval->number = strtod(yytext, nullptr);
	return token::FLOAT;
	}

{ALPHA}({ALPHA}|_|{DIGIT})*{TOKEN_END} {  // cdli
	/* Push back the TOKEN_END character. */
	yyless(yyleng - 1);
	yylval->string = sta::stringCopy(yytext);
	return token::KEYWORD;
	}

{PIN_NAME}{TOKEN_END} |
{BUS_NAME}{TOKEN_END} |
{BUS_NAME2}{TOKEN_END} |
{MIXED_NAME}{TOKEN_END} |
{HNAME}{TOKEN_END} |
{BUS_STYLE}{TOKEN_END} |
{TOKEN}{TOKEN_END} {  // cdli
	/* Push back the TOKEN_END character. */
	yyless(yyleng - 1);
	yylval->string = sta::stringCopy(yytext);
	return token::STRING;
	}

\\?{EOL} { loc->lines(); loc->step(); }  // cdli

"include_file"[ \t]*"(".+")"[ \t]*";"? {  // cdli
        if (includeBegin()) {
          BEGIN(INITIAL);
        }
        }

"/*"	BEGIN(comment);  // cdli

	/* Straight out of the flex man page. */
<comment>[^*\r\n]*		/* eat anything that's not a '*' */  // cdli
<comment>"*"+[^*/\r\n]*		/* eat up '*'s not followed by '/'s */  // cdli
<comment>{EOL}	{ loc->lines(); loc->step(); }  // cdli
<comment>"*"+"/" BEGIN(INITIAL);  // cdli

\"	{  // cdli
	token_.clear();
	BEGIN(qstring);
	}

<qstring>\" {  // cdli
	BEGIN(INITIAL);
	yylval->string = stringCopy(token_.c_str());
	return token::STRING;
	}

<qstring>{EOL} {  // cdli
	error("unterminated string constant");
	BEGIN(INITIAL);
	yylval->string = stringCopy(token_.c_str());
	return token::STRING;
	}

<qstring>\\{EOL} {  // cdli
	/* Line continuation. */
	loc->lines(); loc->step();
	}

<qstring>\\. {  // cdli
	/* Escaped character. */
	token_ += '\\';
	token_ += yytext[1];
	}

<qstring>[^\\\r\n\"]+ {  // cdli
	/* Anything but escape, return or double quote */
	token_ += yytext;
	}

<qstring><<EOF>> {  // cdli
	error("unterminated string constant");
	BEGIN(INITIAL);
	yyterminate();
	}

{BLANK}* {}  // cdli
	/* Send out of bound characters to parser. */
.	{ return (int) yytext[0]; }  // cdli

<<EOF>> { if (stream_prev_)  // cdli
            fileEnd();
          else
            yyterminate();
        }

%%
