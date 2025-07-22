Archi La Simu 2
=====

<center><b><i>A Web navigator based Von-Neuman Architecture simulator</i></b></center>
<center><b><i>Apr. 2025 - Mikhael Myara, Arnaud Virazel, University of Montpellier</i></b></center>
<center><b><i>Licence GPLv3</i></b></center>

----


### This Git
The project is given as code source as well as webassembly binaries for self hosting.
It is a raw copy of my work folder on my computer. I have no time to manage pull requests or collaborative work for new versions. Feel free to fork the project if you want to do so.

### Self Hosting
Simply make a raw copy of the contents of `archilasimu\web` in any sub-folder hosted by a web-server.

## Code Organization
The code is a Dear ImGUI project that has been configured to work with webassembly / emscripten. The em++ version I use is :
```bash
$ em++ --version
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 3.1.6 ()
Copyright (C) 2014 the Emscripten authors (see AUTHORS.txt)
This is free and open source software under the MIT license.
```
I added my own code in the `archilasimu` subfolder. This folder also contains a `Makefile`. The project can be build by calling `make`, then `make serve`to have a local web server running ArchiLaSimu: `http://127.0.0.1:8000`




