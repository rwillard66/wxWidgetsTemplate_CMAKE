This directory contains a template for creating wxWidgets desktop applications 
for Linux, OSX, and Windows.

The directory structure is as follows:

(wxWidgets_application_template_CMAKE) --
                                        | CMakeLists.txt
                                        | README.txt
                                        |
                                        |-- (src)
                                        |       | main.cpp
                                        |       | images.h
                                        |
                                        |-- (res)
                                        |       | forms.fbp
                                        |       | mywxproject.png
                                        |       |-- (cpp)
                                        |
                                        |-- (build-linux)
                                        |       | 
                                        |
                                        |-- (build-osx)
                                        |       | 
                                        |
                                        |-- (build-win)
                                        |       | 
                                        |
                                        |-- (nbproject)
                                                | <<nbproject files>>


forms.fbp:        Blank wxFormBuilder document with paths entered.
mywxproject.png:  Default image for icon.
CMakeLists.txt:   CMake file. Contains instruction to create Makefile for
                  application compilation. To generate Makefile, enter the
                  build directory of the target OS you are compiling on, and
                  run 'cmake ..'
                  Note: on windows, run 'cmake -G "MinGW Makefiles" ..'
REAME.txt:        This file.
main.cpp:         Template for main. Includes instructions for adding wx
                  Form class.
images.h:         Prototype file for including images in the application. 
		          Full directions for use are included at the top of the 
                  file.

Distribution
============
The CMake file generates package building scripts in the target Makefiles. To 
create a package, run 'make package' in the target build folder. This will
generate the following:

Linux - A .deb and .changes file are created that can be added to the repository
OSX   - A .app bundle is created. This can then be added to a .dmg distribution 
        by following the instructions in the Sagatech wiki.
Windows - Creates NSIS install package. 
