## What this folder contains?

In this folder are all files you need for a .lib build. To build it, please add all (three) files to a LIB-project and build it.

If you use an linux system there are no LIB, but .so-files for same reason. 
So, if you want to compile it for linux, make sure you compile it to an .so-file.

###Why and how use an LIB-file?

A LIB-file is very similar to an .obj file. It is compiled, but not linked. To use it you can link it in your project. 
Normally you can add such files under linker option "additional dependencies" or similars. If you had link it you can include
the pulished header file (GAUSSIAN.h). It contains all needed declaration. Is this done you can call the function like any
other function you use. BTW: In this way the linker add the standart ANSI functions like "printf", "scanf" etc. A step-by-step handout how to include the LIB into your project is in the [wiki side to the LIB-Version](https://github.com/LeonardKoenig/GaussianElimination/wiki/Algorithmus-in-eingenen-Projekten) ([here](https://github.com/LeonardKoenig/GaussianElimination/wiki/Algorithmus-in-eingenen-Projekten#wie-kann-ich-die-lib-datei-hinzuf%C3%BCgen)).

###Code

main.cpp
 - implementation of gaussian elimination function
 - implementation of background functions

GAUSSIAN.h
 - "public" declarations for using the LIB (release file)

background.h
 - declarations of background functions
 - functions here are static, so you can't use them if you use the lib (to prevent name conflicts)
