============================
FABRIS PROGRAMMING LANGUAGE
============================


Introduction
========

Fabris is a `stack-oriented`_, `concatenative`_ language designed to be compact,
fast and beginner friendly.

.. _stack-oriented: https://en.wikipedia.org/wiki/Stack-oriented_programming_language
.. _concatenative: https://en.wikipedia.org/wiki/Concatenative_programming_language

Current Fabris version is unstable and is intended only for experimental use.

The name comes from first leters of main components of Fabris VM:
(F)loat stack, (A)llocator, (B)ase pointers, (R)eturn stack, (I)nstruction pointer, (S)tack.

Similarity with the name of Italian fencing master Salvator Fabris
is not a coincidence.


Basic Syntax
============

Basic expressions::

    "the answer is " print 40 2 add dot

Line comment::

    "hello world" print -- the rest of line is ignored
    -- this line is also ignored
	
Stream comment::

    4 10 mul (this is a comment) 1 1 add add ( this also )

Conditional statement::

    if 10 eq then "equals ten" print end
	
Conditional statement with alternative::

    if 10 ge then "ten or more" else "nine or less" end

Counted loop::

    3 times
        "Hastur! " print
    loop

Universal loop::

    do
        dup getc
        if zero then drop drop break end
        emit
    loop

New function definition::

    def square dup mul end

New function definition with named parameters::

    def energy of h m v as
        m v v mul mul 2 div -- kinetic energy
        h m g mul mul -- potential energy
        add end

Testing::

    test 40 2 add 42 eq ok
    test 10 dup inc 11 eq ok 10 eq ok

Lambda expressions::
    
    [" world" print] [ "hello" print ] call call -- prints "hello world"

Recurrency::

    def print-stack
        if depth then dot print-stack end end


Functions
=========


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
  reverse  (*n--*)    reverse order of n top stack items
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

  ======= ======== ======================================================================
  name    effect   comments
  ======= ======== ======================================================================
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
  hash    (an--x)  return hash value for given string (x65599 algorithm)
  call    (f--)    call function referenced by f
  ok      (ab--)   do nothing if two top items are equal, halt and print error otherwise 
  ======= ======== ======================================================================


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

