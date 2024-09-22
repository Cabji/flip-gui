# Overview

This is a project to make the flip-cpp project into a GUI program.

C++ with wxWidgets, UI being built using wxFormBuilder. The aim is to write cross-platform code.

# Building

Use CMake. currently the provided CMake and config files are for my person machine which is Windows 11, using msys2 UCRT64 for the C++ STL, and wWidgets built statically on the local machine.
If you want to build this project, you'll have to alter the CMakeLists.txt file to point to where the relevant wxWidgets folders for your platform are and configure VSCode Cmake addon to know where needed includes are.

# Project Plans

Here are some features I'd like to add eventually:

✔ Template Manager: add, remove, edit & validate template files

- Core PDF processing functionality (copy & paste from flip-cpp (haw haw yeah that will work! /s))
- Side-by-side Data Comparison: view the processed data side by side at each step of processing
- Advanced Output: output into various formats locally (saved to file on disk), and maybe even output directly to networked locations/servers
