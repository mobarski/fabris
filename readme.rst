============================
FABRIS PROGRAMMING LANGUAGE
============================


Introduction
============

Fabris is a `stack-oriented`_, `concatenative`_ language designed to be compact_,
simple_ and efficient_. Fabris is inspired by Forth, Joy, DSSP, Python and Unix.

.. _stack-oriented: https://en.wikipedia.org/wiki/Stack-oriented_programming_language
.. _concatenative: https://en.wikipedia.org/wiki/Concatenative_programming_language

Current Fabris version is unstable and is intended only for experimental use.

The name comes from first leters of main components of Fabris VM:
(F)rame pointer, (A)llocators, (B)ase pointers, (R)eturn stack, (I)nstruction pointer, (S)tack.

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

    if 10 equal then "equals ten" print end
	
Conditional statement with alternative::

    if 10 or-more then "ten or more" else "nine or less" end

Conditional statement with multiple branches::

    10 or-more then "ten or more"   else
     8 or-less then "eight or less" else
	            "exactly nine"  end

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
    exterminate exterminate exterminate

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
    
    [ " world" print ] [ "hello" print ] call call -- prints "hello world"

Recurrency::

    def print-stack
        if depth then dot drop print-stack end ret

List reduction::

    [ 1 3 5 7 9 ] list [ add ] fold -- sum all elements

Include module::

    use string
    do each-word
	print
    loop

Top-down programming::

    def c  b b add ret
    def b  a a add ret
    def a  1 ret
    
    a dot -- prints 4


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

..	TODO
	r -> kopiuje gorny element r do s
	rdrop -> odrzuca element z r
	? jakies inne nazwy na operacje na r

  ======== =========== ============================================================ =====
  name     effect      comments                                                     core 
  ======== =========== ============================================================ =====
  swap     (ab--ba)    swap the two top stack items                                 yes 
  dup      (a--aa)     duplicate the top stack item                                 yes 
  drop     (a--)       discard the top item                                         yes 
  over     (ab--aba)   push the second item on top                                      
  nip      (ab--b)     discard the second item                                          
  tuck     (ab--bab)   insert copy of top item before second item                       
  rot      (abc--bca)  rotate the third item to the top                                 
  unrot    (abc--cab)  unrotate the top to the third item                               
  depth    (--n)       push number of items on stack                                yes 
  tor      (a--)(=a)   move the top item to the return stack                        yes 
  tos      (--x)(a=)   move the top item of return stack to stack                   yes 
  mark     (--)(=n)    mark stack location (push stack depth to return stack)           
  count    (--x)(n=)   push number of items after the mark, unmark stack
  cut      (?--)(n=)   drop items after marked stack location  
  yank     (--a)(ab=b) remove second item from return stack and place it on stack       
  ======== =========== ============================================================ =====

Basic Arithmetic
----------------

  ======== ========= ======================================================== =====
  name     effect    comments                                                 core
  ======== ========= ======================================================== =====
  add      (ab--x)   add two top items (a+b)                                  yes
  sub      (ab--x)   subtract top item from second item (a-b)
  mul      (ab--x)   multiply two top items (a*b)
  div      (ab--x)   divide second item by top item (a/b)
  mod      (ab--x)   reminder of dividing second item by top item (a%b)
  inc      (a--x)    increment the top item (a+1)
  dec      (a--x)    decrement the top item (a-1)
  abs      (a--x)    return absolute value (abs(a))
  neg      (a--x)    change the sign (-a)                                     yes
  ======== ========= ======================================================== =====


Comparators
-----------

  ======== ========== ======================================================== =====
  name     effect     comments                                                 core
  ======== ========== ======================================================== =====
  zero     (a--ax)    check if a == 0                                          yes 
  minus    (a--ax)    check if a < 0                                           yes 
  plus     (a--ax)    check if a > 0                                           
  less     (ab--ax)   check if a < b                                           
  or-less  (ab--ax)   check if a <= b
  more     (ab--ax)   check if a > b                                           
  or-more  (ab--ax)   check if a >= b
  equal    (ab--ax)   check if a == b                                          
  within   (nab--nx)  check if a <= n <= b                                     
  ======== ========== ======================================================== =====


Logic
-----

  ===== ======== ============================================== =====
  name  effect   comments                                       core
  ===== ======== ============================================== =====
  and   (ab--x)  and two top items (a&b)                        yes
  or    (ab--x)  or two top items (a|b)                         yes
  xor   (ab--x)  xor two top items (a^b)                        ?
  shl   (ab--x)  shift a left by b bits (a<<b)
  shr   (ab--x)  shift a right by b bits (a>>b)
  ushr  (ab--x)  shift unsigned a right by b bits (a>>b)
  inv   (a--x)   invert all bits (~a)
  not   (a--x)   logical negation (!a)                          yes
  ===== ======== ============================================== =====


Input/Output
------------

  ======= ======== ================================================================ =====
  name    effect   comments                                                         core
  ======= ======== ================================================================ =====
  emit    (c--)    write single character to standard output                        yes
  take    (--c)      read single character from standard input                      opt.
  untake  (c--)      unread single character from standard input                    opt.
  print   (an--)   prints n characters at address a
  argc    (--x)    returns number of program arguments                              opt.
  argv    (a--xn)  returns address and length of argument number a                  opt.
  dot     (a--a)   prints top item as number followed by space
  udot    (a--a)   prints top item as unsigned number followed by space
  xdot    (a--a)   prints top item as hexadecimal number followed by a space
  write   (anf--)    write n characters at address a to file with descriptor f
  ======= ======== ================================================================ =====

Control/Flow
------------

  ======= ========= ================================================================ =====
  name    effect    comments                                                         core
  ======= ========= ================================================================ =====
  def X   (--)      define new word X                                                yes
  ret     (--)      return from definition                                           yes
  macro X (--)      define new macro X                                               yes
  mend    (--)      end macro definition                                             yes
  then    (x--)     execute following code if x is not zero                          yes
  else    (--)      branch for the then word (optional)                              yes
  end     (--)      finish then/else sequence                                        yes
  do      (=x)      start of unconditioned loop                                      yes
  break   (x=)      break out of the current loop                                    yes
  loop    (--)      repeat loop                                                      yes
  times   (x--)(=i) start of counted loop                                            yes
  "X"     (--an)    start/end of a string, places addres and length on the stack     yes
  [       (--)      start of anonymous code block                                    yes
  ]       (--r)     end of anonymous code block, put reference to it on the stack    yes
  call    (r--)     call code block referenced by r                                  yes
  'X'     (--c)     start/end of a char                                              yes
  X       (--x)     place integer X in the stack                                     yes
  use X   (--)      use module X                                                     yes
  dyn X   (--)      declare word X as dynamic, that can change at the runtime        yes
  ref X   (--r)     put reference to word X on the stack                             yes
  as X    (r--)     redefine dynamic word X as code reference r                      yes
  ======= ========= ================================================================ =====

Other
-----

  ======= ======== ================================================================ =====
  name    effect   comments                                                         core
  ======= ======== ================================================================ =====
  nop     (--)     do nothig
  clock   (--x)    returns number of microseconds since the program was launched    opt.
  halt    (--)     stops program execution                                          yes
  peek    (a--c)   get character (unsigned) from address a                          opt.
  poke    (ca--)   set character at addres a to c                                   opt.
  ok      (ab--)   halt and print error if two top items are not equal
  trace   (--)     prints information about VM state - stack, ip, ...
  sprint  (--)     prints stack
  ======= ======== ================================================================ =====

String Manipulation
-------------------

  ========= ============ =========================================================== =====
  name      effect       comments                                                    core
  ========= ============ =========================================================== =====
  hash      (an--x)      return hash value for given string (x65599 algorithm)
  split     (an--rxfy)   return first word from a string and the rest of the string
  strip     (an--bx)     return string without leading and trailing whitespaces
  lstrip    (an--bx)     return string without leading whitespaces
  rstrip    (an--bx)     return string without trailing whitespaces
  substr    (ankc--anbc) return substring of c characters starting at b
  index     (anbm--anx)  return index of bm string within an string, or -1
  char      (ani--anx)   return character at index i in given string                 yes
  upper     (an--an)       destructive change to lowercase
  lower     (an--an)       destructive change to uppercase
  ========= ============ =========================================================== =====


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
  reverse  (?n--?n)     reverse order of n top stack items
  reverse2 (?n--?n)       reverse order of n top stack pairs
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


.. _efficient:

Performance
===========

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


.. _compact:

Minimalism
==========

One of the design goals of Fabris is to be compact. That is why the language is divided
into core words and extension words. Fabris implementation needs only to natively handle
core words to provide ability to compile any Fabris program as every extension word
can be writen in Fabris using only core words. In practice most standard Fabris words
are natively implemented because they are designed to be easily implemented in C.

Fabris core words:
  - 12 control flow words: def, ret, then, else, end, [, ], call, ", do, break, loop
  - 6 stack manipulation words: swap, dup, drop, tos, tor, depth
  - 7 arithmetic and logic words: add, neg, zero, minus, and, or, not
  - 3 other words: emit, char, halt
  - 4 optional words: clock, take, argc, argv


.. _simple:

Simplicity
==========

