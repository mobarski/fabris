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
(F)rame pointer, (A)llocators, (B)ase pointers, (R)eturn stack, (I)nstruction pointer, (S)tack.

Similarity with the name of Italian fencing master Salvator Fabris
is not a coincidence.


..	INSPIRACJA
	forth,dssp,joy,factor,python,lisp,unix

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
        scan-for-enemies
        if enemy then
	    break end
        patrol-area
    loop
    exterminate

New function definition::

    def square dup mul ret

New function definition with named parameters::

    def energy of h m v as
        m v v mul mul 2 div -- kinetic energy
        h m g mul mul -- potential energy
        add ret

Testing::

    test 40 2 add 42 ok
    test 10 dup inc 11 ok 10 ok

Lambda expressions::
    
    [" world" print] [ "hello" print ] call call -- prints "hello world"

Recurrency::

    def print-stack
        if depth then dot drop print-stack end ret

List reduction::

    [1 3 5 7 9] list [add] fold -- sum all elements

Functions
=========

..	TODO
	ile konsumuja komparatory? 0 / 1 / 2 ? moze if zaznacza stos a then dropuje?
	frame pointer i zmienne lokalne
	map i fold z joy
	dot vs peek
	var vs into vs to vs set vs save
	local vs global vs const vs state vs static
	zmiana nazwy tor/fromr:
	-> tor/tos, stor/rtos, rput/rget, putr/getr, bury/dig,
	-> cut/paste, store/restore, plant/dig, poke,prod/dig,
	-> jut,dab,pat/?, lay/raise, lay/pick, keep,save/

Stack Manipulation
------------------

  ======== =========== ============================================================
  name     effect      comments
  ======== =========== ============================================================
  swap     (ab--ba)    swap the two top stack items
  dup      (a--aa)     duplicate the top stack item
  drop     (a--)       discard the top item
  over     (ab--aba)   push the second item on top
  nip      (ab--b)     discard the second item
  tuck     (ab--bab)   insert copy of top item before second item
  rot      (abc--bca)  rotate the third item to the top
  unrot    (abc--cab)  unrotate the top to the third item
  depth    (--n)       push number of items on stack
  tor      (a--)(=a)   take the top item of and push it onto the return stack
  tos      (--x)(a=)   take the top item of return stack and push it on stack
  mark     (--)(=n)    mark stack location (push stack pointer to return stack)
  count    (--x)(n=)   push number of items after the mark, unmark stack
  yank     (--a)(ab=b) remove second item from return stack and place it on stack
  ======== =========== ============================================================

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
  minus    (a--ax)    check if a < 0
  plus     (a--ax)    check if a > 0
  less     (ab--abx)  check if a < b
  more     (ab--abx)  check if a > b
  equal    (ab--abx)  check if a == b
  within   (nab--nx)  check if a <= n < b
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


Input/Output
------------

  ======= ======== ======================================================================
  name    effect   comments
  ======= ======== ======================================================================
  emit    (c--)    write single character to standard output
  take    (--c)      read single character from standard input
  print   (an--)   prints n characters at address a
  argc    (--x)    returns number of program arguments
  argv    (a--xn)  returns address and length of argument number a
  dot     (a--a)   prints top item as number followed by space
  udot    (a--a)   prints top item as unsigned number followed by space
  xdot    (a--a)   prints top item as hexadecimal number followed by a space
  write   (anf--)    write n characters at address a to file with descriptor f
  ======= ======== ======================================================================


Other
-----

  ======= ======== ======================================================================
  name    effect   comments
  ======= ======== ======================================================================
  nop     (--)     do nothig
  clock   (--x)    returns number of microseconds since the program was launched
  halt    (--)     stops program execution
  call    (f--)    call function referenced by f
  ok      (ab--)   do nothing if two top items are equal, halt and print error otherwise
  trace   (--)     prints information about VM state - stack, ip, ...
  sprint  (--)     prints stack
  ======= ======== ======================================================================

String Manipulation
-------------------

  ========= ============ ===================================================================
  name      effect       comments
  ========= ============ ===================================================================
  hash      (an--x)      return hash value for given string (x65599 algorithm)
  split     (an--rxfy)   return first word from a string and the rest of the string
  strip     (an--bx)     return string without leading and trailing whitespaces
  lstrip    (an--bx)     return string without leading whitespaces
  rstrip    (an--bx)     return string without trailing whitespaces
  substr    (ankc--anbc) return substring of c characters starting at b
  index     (anbm--anx)  return index of bm string within an string, or -1
  char      (ani--anx)   return character at index i in given string
  upper     (an--an)       destructive change to lowercase
  lower     (an--an)       destructive change to uppercase
  ========= ============ ===================================================================


String Comparators
------------------

  ========= ============ ===================================================================
  name      effect       comments
  ========= ============ ===================================================================
  begins    (anbm--anx)  return true if an string begins with bm string
  ends      (anbm--anx)  return true if an string ends with bm string
  contains  (anbm--anx)    return true if an string contains bm string
  arein     (anbm--anx)    return true if an string contains any character from bm string
  haschar   (anc--anx)     return true if an string contains character c
  ========= ============ ===================================================================


More Stack Manipulation
-----------------------

  ======== ============ ===========================================================
  name     effect       comments
  ======== ============ ===========================================================
  dup2     (ab--abab)   duplicate top pair
  swap2    (abxy--xyab) swap two pairs
  drop2    (ab--)       drop pair
  pick     (n--x)       pick nth stack item from top (not counting n)
  ndrop    (?n--?)      discard n top items (not counting n)
  reverse  (?n--?)      reverse order of n top stack items
  reverse2 (?n--?)        reverse order of n top stack pairs
  push     (?n--)       push n items from stack to return stack
  revpush  (?n--)         push n items from stack to return stack in reverse order
  pop      (n--?)       pop n items from return stack onto stack
  revpop   (n--?)       pop n items from return stack onto stack in reverse order
  ======== ============ ===========================================================


More Arithmetic
---------------

  ========= ========= ============================================================
  name      effect    comments
  ========= ========= ============================================================
  min       (ab--x)   return lower value
  max       (ab--x)   return greater value
  limit     (xab--y)  limit value of x (aka clamp), if x<a then a, if x>b then b
  divmul    (abc--x)    ... (a/b*c)
  muldiv    (abc--x)    ... (a*b/c)
  muldivmod (abc--xr)   ... (a*b/c, a*b%c)
  divmod    (ab--xr)    ... (a/b, a%b)
  ========= ========= ============================================================


Performance
===========
.. _fast:

Different dispatching techniques results in different efficiency depending
on the CPU architecture [1]_.

Fabris offers multiple dispatching strategies in the single VM.

  ============ == ==== ====== ==== ====== ======= ====== ===== ====== ===== ======
  benchmark     N goto switch call direct repl.sw c.call c.inl python  ENV  VM cfg
  ============ == ==== ====== ==== ====== ======= ====== ===== ====== ===== ======
  nested-loops 16  508    862  990    391     518    489  464   11671  E.1    C.0
  nested-loops 16  398    882  934    287     546    400  369    7142  E.1    C.1
  fibonacci    32  867   1043 1183    665     904    520  485    6037  E.1    C.0
  fibonacci    32  620   1017 1001    501     787    506  401    4524  E.1    C.2
  ============ == ==== ====== ==== ====== ======= ====== ===== ====== ===== ======

Programs are based on Benchmark Tests from http://dada.perl.it/shootout/.

Times are given in milliseconds for best of 5 runs. More benchmarks and results coming soon.

Environment:
  - E.1 - Intel Atom N570 1.66 @ 1.0 GHz, gcc 4.8.4, -O3 -fomit-frame-pointer

VM config:
  - C.0 - Default Fabris config. Python 3.5
  - C.1 - Fabris registers: sp on ESI, ip on EDI. Python 2.7
  - C.2 - Fabris registers: sp on ESI, ip on EDI, rp on EBX. Python 2.7

Related articles:

.. [1] http://www.complang.tuwien.ac.at/forth/threading/
.. [2] http://www.complang.tuwien.ac.at/forth/threaded-code.html
.. [3] http://realityforge.org/code/virtual-machines/2011/05/19/interpreters.html
.. [4] https://en.wikipedia.org/wiki/Threaded_code