Archi La Simu 2
=====

<center><b><i>A Web navigator based Von-Neuman Architecture simulator</i></b></center><br\>
<center><i>Apr. 2025 - Mikhael Myara, Arnaud Virazel, University of Montpellier</i></center><br\>
<center><i>Licence GPLv3</i></center>

This software is made of two parts :
 - the ArchiLaSimu simulator, available in this package at `archilasimu/web`. 
 - a Admin section, to manage some preferences (like an "exam mode"), available at `archilasimu/web/admin`. 
----


### This Git
The project is given as code source as well as webassembly binaries + php files for self hosting.
It is a raw copy of my work folder on my computer. I have no time to manage pull requests or collaborative work for new versions. Feel free to fork the project if you want to do so.

### Self Hosting
Simply make a raw copy of the contents of `archilasimu/web` in any sub-folder hosted by a web-server. Then, you can access to :
 - the ArchiLaSimu simulator by typing `http(s)://your-url/your-path/`
 - the Admin section, by typing `http(s)://your-url/your-path/admin/login.php`
**If you only use the simulator, you don't have to do anything more**

If you want to use the Admin section : The admin section is written in PHP, so PHP has to be installed on the server for the Admin section. The default login is admin // password123. You can change this by :
 - starting `http(s)://your-url/your-path/admin/genpass.php`. Type the password you want and copy the corresponding hash-code in the clip board. 
 - then edit the login.php file. You will see in the very begining :
 ```php
 $users = [
    'admin' => '$2y$10$KhQXBETpDHW.KRKOV7RUg./b1QHvp4GFDuzm8TvXFYOz1AZR33Cai' // password = "password123"
];
```
Then replace the 'admin' hash by the one you copied.
You also can add as mny users as you want, by separating them by commas. It simply uses the syntax of a PHP associative array. 


### Code Organization
The code is a Dear ImGUI project that has been configured to work with webassembly / emscripten. The em++ version I use is :
```bash
$ em++ --version
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 3.1.69 ()
Copyright (C) 2014 the Emscripten authors (see AUTHORS.txt)
This is free and open source software under the MIT license.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
$
```
This is the emscripten version that comes by default with Debian Trixie (Linux 6.12.48+deb13-amd64). 
I added my own code in the `archilasimu` subfolder. This folder also contains a `Makefile`. The project can be build by calling `make`, then `make serve`to have a local web server running ArchiLaSimu: `http://127.0.0.1:8000`




