# makeviz
A tool to visualize makefile dependencies with graphviz

## How to use
This version of makeviz is made with *graphviz version 2.38.0*, and requires that the *dot* executable be available on the path.

It is tested with the *gnumake* makesystem and compatible makefiles.

It assumes that the make binary is named gnumake, and that there is a *grep* and *gcc* implementation on the path.

To generate the graph of its own makefile, run

    make

To generate the graph of another makefile, run

    make file=path/to/makefile

This is the result of a few hours of hacking stuff together, and is just a proof-of-concept prototype.
