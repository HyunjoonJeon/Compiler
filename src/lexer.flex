%option noyywrap

%{
// Avoid error "error: `fileno' was not declared in this scope"
extern "C" int fileno(FILE *stream);

#include "maths_parser.tab.hpp"
%}

%%
[*]             { return T_TIMES; }
[/]             { return T_DIVIDE; }
[+]             { return T_PLUS; }
[-]             { return T_MINUS; }
[%]             { return T_MODULO; }

[>]             { return T_GREATERTHAN; }
[<]             { return T_LESSTHAN; }
[>][=]          { return T_GREATERTHANEQUAL; }
[<][=]          { return T_LESSTHANEQUAL; }
[=][=]          { return T_EQUALTO; }
[!][=]          { return T_NOTEQUALTO; }

[&][&]          { return T_LOGICAND; }
[|][|]          { return T_LOGICOR; }
[!]             { return T_NOT; }

[?]             { return T_QUESTION}
[:]             { return T_COLON}

[=]             { return T_ASSIGN; }
[;]             { return T_SEMICOLON; }
[,]             { reutrn T_COMMA; }

[{]             { return T_LBRACE; }
[}]             { return T_RBRACE; }
[(]             { return T_LBRACKET; }
[)]             { return T_RBRACKET; }

int             { return T_INT; }
return          { return T_RETURN; }
while           { return T_WHILE; }
if              { return T_IF; }
else            { return T_ELSE; }

[0-9]+([.][0-9]*)?             { yylval.number=strtod(yytext, 0); return T_NUMBER; }
[a-zA-Z_][a-zA-Z_\d]*          { yylval.string=new std::string(yytext); return T_VARIABLE; }

[ \t\r\n]+		{;}

.               { fprintf(stderr, "Invalid token\n"); exit(1); }
%%

void yyerror (char const *s)
{
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}