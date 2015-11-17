# catpass

This is the template to use for assignments of the Code Analysis and Transformation class at Northwestern

To build:
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Your pass has been now compiled and it's available here: build/catpass/libCatPass.so

To apply your pass to a bitcode (i.e., LLVM IR):
    $ opt -load build/catpass/libCatPass.* -CAT -disable-output bitcode_to_analyze.bc
    or
    $ clang -Xclang -load -Xclang build/catpass/libCatPass.* program_to_analyse.c
