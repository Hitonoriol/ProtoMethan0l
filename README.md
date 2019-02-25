<pre>
------------------------------------------
Methan0l interpreted programming language.
		FEB 2019		  
------------------------------------------
Command line args:
methan0l [program source]

Operators and symbols:
^ - argument list delimiter (e.g. function^var,"text",123;)
$ - 1) if used in out^ function (e.g. out^$;), creates a new line,
    2) if used on variables and strings (e.g var$" foo "$sqrt(42);), concats all elements into one string.
% - assignment operator
() - function name brackets
[] - block operator brackets

Available commands:
def^var[% math expression|integer|"string"],var2,var3,...;
Example: def^foo%50, bar%foo*0.05, abc%"text";

out^[variable|integer|"string"|math expression|$-for newline],expr2,expr3,...;
Example: out^"Hello, World!",$,some_variable,$,50*sqrt(800)**35;

in^["Prompt string"],variable,...;
Example: in^"Enter your name: ", name;

variable%["string"|math expression|integer];
myvar%oldvar+sin(45);

userfunc^[arg1,arg2,arg3,...];

times (counter, [int var|constant|math expression])[
	operators;
]
Example:
times (i, 100)[
	out^sin(i)," ",cos(i),$;
]

if (var1|expression ==|<|>|!= var2|expression and|or ...)[
	...
] else [
	...
]
</pre>
