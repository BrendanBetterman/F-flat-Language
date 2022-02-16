# compiler
Custom LL(1) language 

______________________ 
A basic Language developed by (badcafe)Sam , (baskanos)Brian, and (Xyphold)Brendan
 

<Language Name> keywords: BOF
------------------------------

Usage
________________

* BOF: Begins the program.

<Language Name> keywords: EOF
------------------------------

Usage
________________

* EOF: Ends the program.

<Language Name> keywords: int
------------------------------

Usage
________________

* int type: as the declaration of the type Integer

<Language Name> keywords: fake
------------------------------

Usage
________________

* fake type: as the declaration of the type Real

<Language Name> keywords: str
------------------------------

Usage
________________

* str type: as the declaration of the type String

<Language Name> keywords: bool
------------------------------

Usage
________________

* bool type: as the declaration of the type Boolean

Syntax
________________

yay : true

nay : false

Explanation
________________
The boolean is denoated with with yay or nay and can be used in conditional statments. 

<Language Name> keywords: fin
------------------------------

Usage
________________

*fin <int> : This will prompt the user to enter a value.

Syntax
________________

fin <int>,<int>;

Explanation
________________

This allows the user of a program to enter their own values.

Examples
________________

:::PROGRAM:::
int x;
int y;
fout "Enter two values";
fin x,y;
fout x,y;
:::CONSOLE:::
Enter two values 10 20 
10 20

Notes
________________

fin uses white spaces as delimiters.

<Language Name> keywords: fout
------------------------------

Usage
________________

*fout <int>||<str>||<fake>||<"">: outputs to console

Explanation
________________

fout outputs the variable/Literal that its given. 

Examples
________________

:::PROGRAM:::
srt x;
x = "helloworld";
fout x,x;
fout "hi";
:::CONSOLE:::
helloworldhelloworldhi

Notes
________________

Note that fout doesn't new line.

<Language Name> keywords: foutln
------------------------------

Usage
________________

*foutln <int>||<fake>||<str>||<""> : outputs enter var starting with new line.


Explanation
________________
foutln outputs the entered literal/variable to the console.

Examples
________________
:::PROGRAM:::
str x;
x = "helloworld";
foutln x;
foutln x;
:::CONSOLE:::

helloworld
helloworld

Notes
________________
fout prints the new line before the statement.

<Language Name> keywords: fif
------------------------------

Usage
________________
*fif (condition)

Explanation
________________
fif is the tradisional if statement. if the condition is true it runs the code.

Examples
________________
:::PROGRAM:::
fif (1 == 2)
    fout "help";
felse
    fout "correct";
fifend
:::CONSOLE:::
correct

Notes
________________

<Language Name> keywords: 
------------------------------

Usage
________________

Syntax
________________

Explanation
________________

Functions
________________

Examples
________________

Notes
________________


______________________
Single line comments: //

Multi-line comments:  /* */
 
Primitive-Data-Types 
----------------------
 
Type ------- Identifier
-----------------------

Char		(Doesn't exist)

Byte		(Doesn't exist)

Short		(Doesn't exist)

Long		(Doesn't exist)

Non-Primitive-Data-Types
_________________________

Type -------	Identifier

String		str

Array		[]
