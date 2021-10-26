# myls
Implements a version of the ls command with similar behavior.

A source file, myls.c, implements the following logic:

When compiled and run with no arguments, it lists all non-hidden files in the current directory. 

When supplied with a list of arguments, it behaves as ls does: 
  for all non-directories,  the files are listed; 
  for all directories,  the contents are listed; 
  error if a file doesn’t exist. 
   

The ls program also accepts the -a and -l optional arguments. 
When provided with the -a option, the program  lists all files in the directories given, including hidden files.
When provided with the -l option, the program lists detailed information about each file, one file per line, just as the standard ls program does.
