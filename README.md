# compiler
Custom LL(1) language 

______________________ 
A basic Language developed by (badcafe)Sam , (baskanos)Brian, and (Xyphold)Brendan
 
 <hr style="height:2px;color:blue">

F♭ keywords: BOF
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

* BOF: Begins the program.

____________________________

F♭ keywords: EOF
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

* EOF: Ends the program.

------------------------------

F♭ keywords: int
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

* int type: as the declaration of the type Integer

--------------------------------------

F♭ keywords: fake
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

* fake type: as the declaration of the type Real
--------------------------------------

F♭ keywords: str
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

* str type: as the declaration of the type String
--------------------------------------

F♭ keywords: bool
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

* bool type: as the declaration of the type Boolean

<h3>Syntax</h3><hr style="height:2px;color:gray">

yay : true<br>
nay : false<br>

<h3>Explanation</h3><hr style="height:2px;color:gray">
________________
The boolean is denoated with with yay or nay and can be used in conditional statments. 
--------------------------------------

F♭ keywords: fin
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

*fin &lt;int&gt; : This will prompt the user to enter a value.

<h3>Syntax</h3><hr style="height:2px;color:gray">

fin &lt;int&gt;,&lt;int&gt;;

<h3>Explanation</h3><hr style="height:2px;color:gray">

This allows the user of a program to enter their own values.

Examples
________________

:::PROGRAM:::<br>
int x;<br>
int y;<br>
fout "Enter two values";<br>
fin x,y;<br>
fout x,y;<br>
:::CONSOLE:::<br>
Enter two values 10 20<br> 
10 20<br>

Notes
________________

fin uses white spaces as delimiters.
--------------------------------------

F♭ keywords: fout
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

*fout &lt;int&gt;||&lt;str&gt;||&lt;fake&gt;||&lt;""&gt;: outputs to console

<h3>Explanation</h3><hr style="height:2px;color:gray">

fout outputs the variable/Literal that its given. 

Examples
________________

:::PROGRAM:::<br>
srt x;<br>
x = "helloworld";<br>
fout x,x;<br>
fout "hi";<br>
:::CONSOLE:::<br>
helloworldhelloworldhi<br>

Notes
________________

Note that fout doesn't new line.<br>
--------------------------------------

F♭ keywords: foutln
------------------------------

<h3>Usage</h3>

*foutln &lt;int&gt;||&lt;fake&gt;||&lt;str&gt;||&lt;""&gt; : outputs enter var starting with new line.<br>


<h3>Explanation</h3>

foutln outputs the entered literal/variable to the console.

Examples
________________
:::PROGRAM:::<br>
str x;<br>
x = "helloworld";<br>
foutln x;<br>
foutln x;<br>
:::CONSOLE:::<br>
<br>
helloworld<br>
helloworld<br>

Notes
________________
fout prints the new line before the statement.
--------------------------------------

F♭ keywords: fif
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

*fif (condition)

<h3>Explanation</h3><hr style="height:2px;color:gray">

fif is the tradisional if statement. if the condition is true it runs the code.

Examples
________________
:::PROGRAM:::<br>
fif (1 == 2)<br>
    fout "help";<br>
felse<br>
    fout "correct";<br>
fifend<br>
:::CONSOLE:::<br>
correct<br>

Notes
--------------------------------------

F♭ keywords: 
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

<h3>Syntax</h3><hr style="height:2px;color:gray">

<h3>Explanation</h3><hr style="height:2px;color:gray">

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
