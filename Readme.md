This is my version of Picobit, a small scheme system designed to run on small microcontrollers. I made the portability of PICOBIT to STM32F100RB.

Original description and documentation:

It consists of

* a bytecode compiler which compiles Scheme source code to bytecode.
  The bytecode compiler is run using Racket, usually on a
  PC/workstation.

* a bytecode interpreter which can be either compiled to run on a
  microcontroller, or to be run on some common operating systems, at
  least GNU/Linux (Windows support hasn't been tested in a while,
  though, and especially the networking part might not work out of the
  box). It is written in (mostly) portable C.

See also:
  * A paper describing PICOBIT has been presented to IFL 2009:
    http://www.ccs.neu.edu/home/stamourv/papers/picobit.pdf
    Slides from the presentation:
    http://www.ccs.neu.edu/home/stamourv/slides/picobit-ifl09.pdf

  * S3 (Small Scheme Stack) : A Scheme TCP/IP Stack Targeting Small
    Embedded Applications
    http://www.ccs.neu.edu/home/stamourv/papers/s3.pdf
    Slides from the presentation:
    http://www.ccs.neu.edu/home/stamourv/slides/s3-sw08.pdf

 PICOBIT is a descendant of the BIT and PICBIT systems. You can find
 papers describing these systems at:
    http://w3.ift.ulaval.ca/~dadub100/


