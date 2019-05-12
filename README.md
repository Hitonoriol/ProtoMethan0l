<pre>
------------------------------------------
Methan0l interpreted programming language.
		FEB 2019		  
------------------------------------------
Command line args:
methan0l [program source]

Operators and symbols:
^ - argument list delimiter (e.g. function^var,"text",123;)
$ - 1) when used in out^ function (e.g. out^$;), creates a new line,
    2) when used on variables and strings (e.g var$" foo "$sqrt(42);), acts like a concatenation operator.
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

! Block operators (times, if/else, while) and function definitions must have '#' symbol at the beginning.

#times (counter, [int var|constant|math expression])[
	operators;
]
Example:
#times (i, 100)[
	out^sin(i)," ",cos(i),$;
]

! if and while operators accept only variables, (==|<|>|!=), (and|or).

#if ( (var1 =|<|>|!= var2) [&,|] (...) ...)[
	...
] #else [
	...
]

#while ( (var1 =|<|>|!= var2) [&,|] (...) )[
	...
]

Example:
#while ((i<10) & (!flag))[
	doSomething^;
	+^i;
]

Return from function:
#(func:arg, arg1)[
	return^arg+arg1;
]

round^variable|expression, precision; -- does not  change the variable directly, returns the rounded value.
Example:
a % 150.7984;
out^"Integer: ", (round^a,0), $;

! If you want to include a return of one function as argument into another brackets () are mandatory.

Substring function:
(start[,length])^variable|expression
Example:
out^((3,3)^"onetwothree")  <-- prints "two"

String length:
len^var|expr;

String replace:
replace^source, old, new;

Floating point precision:
prec^int;

Random integer:
rnd^min, max;

Increment/decrement:
+^var;
-^var;

Arrays:
Arrays have dynamic size (when the element which exceeds current bounds is inserted, array size increases too):
([n]^arrayname) % expr;   <-- sets nth array element to expr
out^arrayname;  <-- outputs current array size (array name -> reserved readonly variable which contains its size)
out^([x,y,z]^arrayname);  <-- multidimensional arrays allowed

</pre>
