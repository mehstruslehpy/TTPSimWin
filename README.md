# WHAT THIS IS:
* This is an assembler for the TTP architecture.
  * TTP stands for "Taks Toy Processor" it is a processor design used for an intro to assembly language class that I took.
# TIPS AND USAGE:
* To run do: __./asm &lt;myasmfile&gt;__
  * the output will be placed in the file output.ram.
  * if you have access to the Taks logisim files for his ttp processor then you can run your assembly on those.
  * if you do not then don't worry you can run them just as easily on the emulator I created which you can find at https://github.com/mehstruslehpy/TTPSim
* As an example try running the code under ./TestingPrograms
  type: __./asm TestingPrograms/Add.asm__
# TODO:
* clean up the source code and make it look pretty.
* doxygen docs?.
* test more thorougly.
* nicer error handling.
* clean up debug/logging output.
* fix very minor label/comment bugs.
* fix minor single char instruction matching bugs.
![link to demo gif goes here](https://raw.githubusercontent.com/mehstruslehpy/Documents/master/C%2B%2B/TTPAsm/TTPSimAndAsmDemo.gif)

# WINDOWS NOTES:
* This version of my ttp tools is very much experimental please notify me if you encounter any bugs
* The debug scripts will only work if the system you are using has the correct nix shells and utilities
* You can either run the program via the command prompt using a program like msys or by clicking run inside of codeblocks
* If you choose to run inside of codeblocks set the ram file to run via Project>Set programs' arguments...> then type your filename under Program arguments
* Debugging inside codeblocks works like normal except both the release and debug build targets build with the -g compiler flag
