============================
FABRIS PROGRAMMING LANGUAGE
============================


Introduction
========

Fabris is a concatenative, stack based language inspired by Forth.
It is designed to be small, fast and beginner friendly.

Current Fabris version is unstable and is intended only for experimental use.

The name comes from first leters of main components of Fabris VM:
  - F - float stack
  - A - allocator
  - B - base pointers
  - R - return stack
  - I - instruction pointer
  - S - stack

Similarity with the name of Italian fencing master Salvator Fabris
is not a coincidence.


Basic Syntax
============

Comments
--------

Stream Comments
~~~~~~~~~~~~~~~

Line Comments
~~~~~~~~~~~~~


Instructions
============


Stack Manipulation
------------------

  ======== ========== ========================================================
  name     effect     comments
  ======== ========== ========================================================
  swap     (ab--ba)   swap the two top stack items
  dup      (a--aa)    duplicate the top stack item
  over     (ab--aba)  push the second item on top
  drop     (a--)      discard the top item
  nip      (ab--b)    discard the second item
  tuck     (ab--bab)  insert copy of top item before second item
  rot      (abc--bca) rotate the third item to the top
  unrot    (abc--cab) unrotate the top to the third item
  depth    (--n)      push number of items on stack
  tor      (a--)      take the top item of and push it onto the return stack
  fromr    (--x)      take the top item of return stack and push it on top
  ======== ========== ========================================================



Basic Arithmetic
----------------

  ======== ========= ========================================================
  name     effect    comments
  ======== ========= ========================================================
  add      (ab--x)   add two top items (a+b)
  sub      (ab--x)   subtract top item from second item (a-b)
  mul      (ab--x)   multiply two top items (a*b)
  div      (ab--x)   divide second item by top item (a/b)
  mod      (ab--x)   reminder of dividing second item by top item (a%b)
  inc      (a--x)    increment the top item (a+1)
  dec      (a--x)    decrement the top item (a-1)
  abs      (a--x)    return absolute value (abs(a))
  neg      (a--x)    change the sign (-a)
  ======== ========= ========================================================


Comparators
-----------

  ======== ========== ========================================================
  name     effect     comments
  ======== ========== ========================================================
  zero     (a--ax)    check if a == 0
  negative (a--ax)    check if a < 0
  positive (a--ax)    check if a > 0
  lt       (ab--abx)  check if a < b
  le       (ab--abx)  check if a <= b
  gt       (ab--abx)  check if a > b
  ge       (ab--abx)  check if a >= b
  eq       (ab--abx)  check if a == b
  ne       (ab--abx)  check if a <> b
  ======== ========== ========================================================

Logic
-----

  ===== ======== ========================================================
  name  effect   comments
  ===== ======== ========================================================
  and   (ab--x)  and two top items (a&b)
  or    (ab--x)  or two top items (a|b)
  xor   (ab--x)  xor two top items (a^b)
  shl   (ab--x)  shift a left by b bits (a<<b)
  shr   (ab--x)  shift a right by b bits (a>>b)
  ushr  (ab--x)  shift unsigned a right by b bits (a>>b)
  inv   (a--x)   invert all bits (~a)
  not   (a--x)   logical negation (!a)
  ===== ======== ========================================================


Other
-----

  ======= ======== ===============================================================
  name    effect   comments
  ======= ======== ===============================================================
  nop     (--)     do nothig
  clock   (--x)    returns number of microseconds since the program was launched
  halt    (--)     stops program execution
  emit    (a--)    prints single character
  print   (an--)   prints n characters at address a
  trace   (--)     prints information about VM state - stack, ip, ...
  dot     (a--a)   prints top item as number followed by space
  udot    (a--a)   prints top item as unsigned number followed by space
  xdot    (a--a)   prints top item as hexadecimal number followed by a space
  argc    (--x)    returns number of program arguments
  argv    (a--xn)  returns address and length of argument number a
  ======= ======== ===============================================================


More Arithmetic
---------------

  ========= ========= ============================================================
  name      effect    comments
  ========= ========= ============================================================
  min       (ab--x)   return lower value
  max       (ab--x)   return greater value
  limit     (xab--y)  limit value of x (aka clamp), if x<a then a, if x>b then b
  divmul    (abc--x)  ... (a/b*c)
  muldiv    (abc--x)  ... (a*b/c)
  muldivmod (abc--xr) ... (a*b/c, a*b%c)
  divmod    (ab--xr)  ... (a/b, a%b)
  ========= ========= ============================================================



Control Flow
============


Conditional execution
---------------------

if ... then ... end
if ... then ... else ... end


Counted loop
------------

... times ... loop
... times ... break ... loop


Universal loop
--------------

do ... loop
do ... break ... loop


Variables
---------

var X
into X


Words
-----

def X ... ret
ref X

