# Overview

This is a project to make the flip-cpp project into a GUI program.

C++ with wxWidgets, UI being built using wxFormBuilder. The aim is to write cross-platform code.

# Program

flip-gui (or just, Flip) allow you to input a PDF file, run it through some processing, and extract whatever data you want from the PDF file in plain text format usually this would be in comma separated value (CSV) format. Currently (Oct 2024) the project has the core features that it command line counter part has excluding the planned features listed below in Project Plans.

Flip, by itself is not very useful unless you have a *template* file configured for the PDF you are processing. I have ranted on about the details of making templates over in the flip-cpp project repo right here on my github, so go check that out if you want to know more. When this project reaches a mature enough stage I will possibly make some videos detailing the best ways to create templates.

# Building

Use CMake. currently the provided CMake and config files are for my person machine which is Windows 11, using msys2 UCRT64 for the C++ STL, and wWidgets built statically on the local machine.
If you want to build this project, you'll have to alter the CMakeLists.txt file to point to where the relevant wxWidgets folders for your platform are and configure VSCode Cmake addon to know where needed includes are.

# Project Plans

Here are some features I'd like to add eventually:

✔ Template Manager: add, remove, edit & validate template files
✔ Core PDF processing functionality (copy & paste from flip-cpp)
✔ Side-by-side Data Comparison: view the processed data side by side at each step of processing
- Output: output to CSV file on disk
- Automation Support: add command line switches & flip-gui "automation" configuration support for fully automated PDF processing
- Advanced Output: output into various formats locally (saved to file on disk), and maybe even output directly to networked locations/servers
  
