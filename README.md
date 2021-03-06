# compiler
<h1 align="center">F♭</h1>
<h2 align="center">Toy LL(1) language</h2> 
--------



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

<h3>Examples</h3><hr style="height:2px;color:gray">
:::PROGRAM:::<br>
int x;<br>
int y;<br>
fout "Enter two values";<br>
fin x,y;<br>
fout x,y;<br>
:::CONSOLE:::<br>
Enter two values 10 20<br> 
10 20<br>

<h3>Notes</h3><hr style="height:2px;color:gray">

fin uses white spaces as delimiters.
--------------------------------------

F♭ keywords: fout
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

*fout &lt;int&gt;||&lt;str&gt;||&lt;fake&gt;||&lt;""&gt;: outputs to console

<h3>Explanation</h3><hr style="height:2px;color:gray">

fout outputs the variable/Literal that its given. 

<h3>Examples</h3><hr style="height:2px;color:gray">

:::PROGRAM:::<br>
srt x;<br>
x = "helloworld";<br>
fout x,x;<br>
fout "hi";<br>
:::CONSOLE:::<br>
helloworldhelloworldhi<br>

<h3>Notes</h3><hr style="height:2px;color:gray">
________________

Note that fout doesn't new line.<br>
--------------------------------------

F♭ keywords: foutln
------------------------------

<h3>Usage</h3>

*foutln &lt;int&gt;||&lt;fake&gt;||&lt;str&gt;||&lt;""&gt; : outputs enter var starting with new line.<br>


<h3>Explanation</h3>

foutln outputs the entered literal/variable to the console.

<h3>Examples</h3><hr style="height:2px;color:gray">
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

<h3>Notes</h3><hr style="height:2px;color:gray">
________________
fout prints the new line before the statement.
--------------------------------------

F♭ keywords: fif
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

*fif (condition)

<h3>Explanation</h3><hr style="height:2px;color:gray">

fif is the tradisional if statement. if the condition is true it runs the code.

<h3>Examples</h3><hr style="height:2px;color:gray">
:::PROGRAM:::<br>
fif (1 == 2)<br>
    fout "help";<br>
felse<br>
    fout "correct";<br>
fifend<br>
:::CONSOLE:::<br>
correct<br>

<h3>Notes</h3><hr style="height:2px;color:gray">
--------------------------------------

F♭ keywords: for
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">
*for index; condition; increment;<br>
*endfor;<br>

<h3>Explanation</h3><hr style="height:2px;color:gray">
The for loop increments the program until the condition is met.

<h3>Examples</h3><hr style="height:2px;color:gray">
:::PROGRAM:::<br>
int i;<br>
for i =0; i&lt;3 i++<br>
    fout "helloworld";<br>
    fout i;<br>
endfor<br>
:::CONSOLE:::<br>
helloworld0;<br>
helloworld1;<br>
helloworld2;<br>

F♭ keywords: while
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">
*while condition;<br>
*endwhile;<br>

<h3>Explanation</h3><hr style="height:2px;color:gray">
The while loop increments the program until the condition is met.

<h3>Examples</h3><hr style="height:2px;color:gray">
:::PROGRAM:::<br>
int i = 0;<br>
while i&lt;3 <br>
    fout "helloworld";<br>
    fout i;<br>
    i++;
endwhile<br>
:::CONSOLE:::<br>
helloworld0;<br>
helloworld1;<br>
helloworld2;<br>

F♭ keywords: 
------------------------------

<h3>Usage</h3><hr style="height:2px;color:gray">

<h3>Syntax</h3><hr style="height:2px;color:gray">

<h3>Explanation</h3><hr style="height:2px;color:gray">

<h3>Functions</h3><hr style="height:2px;color:gray">

<h3>Examples</h3><hr style="height:2px;color:gray">

<h3>Notes</h3><hr style="height:2px;color:gray">


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
