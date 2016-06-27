//Mini java lexical analyzer
%%

%class Lexer
%implements MiniJavaTokens
%unicode
%int
%debug

%{
StringBuffer string=new StringBuffer();
int token;
Object lexVal;

int column(){
return yycolumn;
}
int line(){
	return yyline;
}
%}
CommentContent = ([^* ] | \*+ [^\/\*])*
LineTerminator = \n|\r\n|\r
Space = {LineTerminator}|[ \t\f]
Identifier = [:jletter:][:jletterdigit:]*
IntegerLiteral = 0|[1-9][0-9]*
Comment = {LineComment}|{MultiLineComment}
LineComment = "//" {CommentContent}[LineTerminator]
MultiLineComment = "/*" ~"*/"|"/**" {CommentContent}"*/"
InputCharacter = [^\r\n]
StringLiteral=\"((\\.|\n)|[^\\\"\n])*\"

%state STRING
%%
<YYINITIAL>{
	"public" {return token=t_public;}
	"class" {return token=t_class;}
	"if" {return token=t_if;}
	"while" {return token=t_while;}
	"this" {return token=t_this;}
	"new" {return token=t_new;}
	"static" {return token=t_static;}
	"boolean" {return token=t_boolean;}
	"System.out.println" {return token=SystemOutPrintln;}
	"int" {return token=t_int;}
	"else" { return token=t_else;}
	"void" {return token=t_void;}
	"return" {return token=t_return;}
	"String" {lexVal=new String(yytext());
			return token=t_String;
			}
}
<YYINITIAL>{
"main" {return token=t_main;}
{Identifier} {
			lexVal=new String(yytext());
			return token=IDENTIFIER;
			}
{StringLiteral} {string.append(yytext());
				return token=STRING_LITERAL;
			}
}
<STRING>{
	"true" {return token=t_true;}
	"false" {return token=t_false;}
	"length" {return token=length;}
	"extends" {return token=t_extends;}
}
	\. {return token='.';}
	, {return token=',';}
	; {return token=';' ;}
	//"||" {return token=OR;}
	\< {return token='<';}
	\> {return token='>';}
	\+ {return token='+';}
-  {return token='-';}
= {return token='=';}
\/ {return token='/';}
\* {return token='*';}
\} {return token='}' ;}
\{ {return token='{';}
\( {return token='(';}
\) {return token=')';}
\[ {return token='[';}
\] {return token=']';}


{IntegerLiteral} {
				lexVal=Integer.parseInt(yytext());
				return token=INTEGER_LITERAL;}
{Space} {/*ignore spaces*/}
{Comment} {/*ignore comment*/}
<<EOF>> {return token=ENDINPUT;}
[^] 			{String err=new String(yytext());
				throw new Error("Unrecognized token: "+err);}			
