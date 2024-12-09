# TLDR and Computer Illiterate/(Boomer)

The app will change a PDF into text so you can have your way with it.

You can get the program by downloading the file listed below.

https://github.com/Cabji/flip-gui/blob/8ef9df2c084397fa329c3915aa31cc79e3ee22df/flip-gui-0.1.zip

# Overview

This is a project to make the flip-cpp project into a GUI program.

C++ with wxWidgets, UI being built using wxFormBuilder. The aim is to write cross-platform code.

# Program

flip-gui (or just, Flip) allow you to input a PDF file, run it through some processing, and extract whatever data you want from the PDF file in plain text format usually this would be in comma separated value (CSV) format. Currently (Oct 2024) the project has the core features that it command line counter part has excluding the planned features listed below in Project Plans.

Flip, by itself is not very useful unless you have a _template_ file configured for the PDF you are processing. I have ranted on about the details of making templates over in the flip (PHP) project repo right here on my github, so go check that out if you want to know more. When this project reaches a mature enough stage I will possibly make some videos detailing the best ways to create templates.

# Building

Use CMake. currently the provided CMake and config files are for my person machine which is Windows 11, using msys2 UCRT64 for the C++ STL, and wWidgets built statically on the local machine.
If you want to build this project, you'll have to alter the CMakeLists.txt file to point to where the relevant wxWidgets folders for your platform are and configure VSCode Cmake addon to know where needed includes are.

# Project Plans

Here are some features I'd like to add eventually:

✔ Template Manager: add, remove, edit & validate template files

✔ Core PDF processing functionality (copy & paste from flip-cpp)

✔ Side-by-side Data Comparison: view the processed data side by side at each step of processing

✔ Stepped Data Processing: view the processed data after each regex is applied (filthy feature creep is happening)

✔ Output: output to CSV file on disk

- Automation Support: add command line switches & flip-gui "automation" configuration support for fully automated PDF processing
- Advanced Output: output into various formats locally (saved to file on disk), and maybe even output directly to networked locations/servers

# Known Bugs

1. ~~Template Editor wxChoice for choosing template file doesn't reflect update in Main Frame's template wxChoice when new template file is added via T.E. (Template Editor)~~
2. ~~Template Editor doesn't auto select newly created template in wxChoice widget, nor open it in text editing widget ready for editing.~~
3. Possible bug in Data Viewer when reusing it multiple times (need to reproduce and accurately describe) (Data Viewer)
4. Template Editor bug - template file editor auto-save polling doesn't trigger if new template file is added and edited