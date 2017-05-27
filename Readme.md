PICOBIT
------

This is my version of Picobit, a small Scheme System designed to run on small microcontrollers. I made the portability of PICOBIT to STM32F100RB.

It consists of 2 parts, a scheme compiler and a VM.

  * The scheme compiler was written in Racket Language, it usually run
  on a PC/workstation. Its function is compile the scheme code to
  bytecode.

  * The Virtual Machine is a bytecode interpreter. In this case it is
  compiled to run on a microcontroller(STM32F1-value line). The VM is
  written in (mostly) portable C.

  Demos and infos: [[picobit][http://medicina.tips/picobit/picobit.html]]

Original description and documentation:
------

Folked from https://github.com/stamourv/picobit.

History
------

 Marc Feeley originally wrote PICOBIT around 2006.
 Vincent St-Amour took over development in 2008.
 Jim Shargo worked on a port to Racket in 2011.
 The Racket port was completed in June 2011.
 Peter Zotov (whitequark) ported PICOBIT to ARM in August 2011.

Licence
------

 PICOBIT is released under the GPLv3.
