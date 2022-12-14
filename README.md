Description
==========
ProcessMonitor is a simple process monitoring application written in C++ using the Qt library.
It constantly monitors the list of processes defined in the config file and starts them if
they aren't running. It also maintains one process window on the foreground. It dynamically monitors
the config file and starts newly added processes, or stops starting processes that are removed.
It also dynamically updates the foreground process. The application lacks this feature on Windows,
as the means to determine the process name in Windows differs. A multiplatform function can be
utilized to extend this feature to Windows as well.<br>
As required, all code is commented, written in C++, but developed using my preferred IDE QtCreator,
Although it is fully compatible with visual studio code with the qt tools extension. 

Crossplatform Capability
=======================
ProcessMonitor is designed to use crossplatform Qt libraries, so it inherently can be compiled in
any platform that supports Qt, i.e. Linux, Windows, Mac, and even Android.

Dependencies
============
Other than the Qt libraries, wmctrl should be installed on Linux for managing the foreground process.

Compiling
=========
The best practice is to use QtCreator to compile and execute it. It can be also opened and compiled using Visual Studio Code. In order to do that Qt Tools for
Visual Studio Code must be installed: https://marketplace.visualstudio.com/items?itemName=tonka3000.qtvsctools

Testing
=======
The implemented methods are too simple to make automated testing for them, and they also require interaction
with external processes. Therefore the functionality is only manually tested.

Usage
=====
A config file should be written for it named "config" and placed in the current directory. Each line represents
the command of the process to be executed. The parameters simply follow the command. To indicate which process
is the foreground process a * is added to the end of the corresponding process line.
For example:

vim<br>
gimp*<br>
thunderbird -mail<br>

In this example three processes exist, two of which are graphical. gimp is executed on the foreground and
thunderbird is executed with the -mail parameter. All three are monitored and whichever crashes or closes
will be restarted.

Logging
=======
ProcessMonitor already logs to stderr, in order to log it into a file run it like the following:

$ ProcessMonitor 2> log &
