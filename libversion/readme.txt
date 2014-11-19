In this folder are all files you need for a .lib build. To build it, please add all (three) files to a LIB-project and build it.

If you use an linux system there are no LIB, but .a-files for same reason. 
So, if you want to compile it for linux, make sure you compile it to an .a-file.

Why and how use an LIB-file?

A LIB-file is very similar to an .obj file. It is compiled, but not linked. To use it you can link it in your project. 
Normally you can add such files under linker option "additional dependencies" or similars. If you had link it you can include
the pulished header file (GAUSSIAN.h). It contains all needed declaration. Is this done you can call the function like any
other function you use. BTW: In this way the linker add the standart ANSI functions like "printf", "scanf" etc.
