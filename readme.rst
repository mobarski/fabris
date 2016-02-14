============================
FABRIS PROGRAMMING LANGUAGE
============================


Introduction
========

Fabris is a `stack-oriented`_, `concatenative`_ language designed to be compact,
fast_ and beginner friendly.

.. _stack-oriented: https://en.wikipedia.org/wiki/Stack-oriented_programming_language
.. _concatenative: https://en.wikipedia.org/wiki/Concatenative_programming_language

Current Fabris version is unstable and is intended only for experimental use.

The name comes from first leters of main components of Fabris VM:
(F)ile descriptors, (A)llocators, (B)ase stack, (R)eturn stack, (I)nstruction pointer, (S)tack.

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

    test 40 2 add 42 ok
    test 10 dup inc 11 ok 10 ok

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
  dup2     (ab-abab)  duplicate two top stack items
  over     (ab--aba)  push the second item on top
  drop     (a--)      discard the top item
  drop2    (ab--)     discard two top items
  drop4    (abcd--)   discard four top items
  nip      (ab--b)    discard the second item
  tuck     (ab--bab)  insert copy of top item before second item
  rot      (abc--bca) rotate the third item to the top
  unrot    (abc--cab) unrotate the top to the third item
  depth    (--n)      push number of items on stack
  tor      (a--)      take the top item of and push it onto the return stack
  fromr    (--x)      take the top item of return stack and push it on top
  reverse  (?n--?)    reverse order of n top stack items
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

String Manipulation
-------------------

  ========= ===========  ===================================================================
  name      effect       comments
  ========= ===========  ===================================================================
  hash      (an--x)      return hash value for given string (x65599 algorithm)
  split     (an--rxfy)   return first word from a string and the rest of the string
  strip     (an--bx)     return string without leading and trailing whitespaces
  lstrip    (an--bx)     return string without leading whitespaces
  rstrip    (an--bx)     return string without trailing whitespaces
  substr    (ankc--anbc) return substring of c characters starting at b
  begins    (anbm--anx)  return true if an string begins with bm string
  ends      (anbm--anx)  return true if an string ends with bm string
  contains  (anbm--anx)    return true if an string contains bm string
  index     (anbm--anx)  return index of bm string within an string, or -1
  arein     (anbm--anx)    return true if an string contains any character from bm string
  haschar   (anc--anx)     return true if an string contains character c
  char      (ani--anx)   return character at index i in given string
  upper     (an--an)       destructive change to lowercase
  lower     (an--an)       destructive change to uppercase
  ========= ===========  ===================================================================

Performance
===========
.. _fast:

Different dispatching techniques results in different efficiency depending
on the CPU architecture [1]_.

Fabris offers multiple dispatching strategies in the single VM.

  ============ == ==== ====== ==== ====== ======= ====== ===== ====== ===== ======
  benchmark     N goto switch call direct repl.sw c.call c.inl python  ENV  VM cfg
  ============ == ==== ====== ==== ====== ======= ====== ===== ====== ===== ======
  nested-loops 16  508    862  990    391     518    489  464   15313  E.1    C.0
  nested-loops 16  398    882  934    287     546    400  369       .  E.1    C.1
  ============ == ==== ====== ==== ====== ======= ====== ===== ====== ===== ======

Programs are based on Benchmark Tests from http://dada.perl.it/shootout/.

Times are given in milliseconds for best of 5 runs. More benchmarks and results coming soon.

Environment:
  - E.1 - Intel Atom N570 1.66 @ 1.0 GHz, gcc 4.8.4, -O3 -fomit-frame-pointer

VM config:
  - C.0 - default config
  - C.1 - sp on ESI register, ip on EDI register


Related articles:

.. [1] http://www.complang.tuwien.ac.at/forth/threading/
.. [2] http://www.complang.tuwien.ac.at/forth/threaded-code.html
.. [3] http://realityforge.org/code/virtual-machines/2011/05/19/interpreters.html
.. [4] https://en.wikipedia.org/wiki/Threaded_code