This is a simple windowed demo with the fantastic GTK2 widgets framework, instead of low level libX control.
Using this template you can quickly implement a fixed size window application for a robot console or a machine HMI.

![alt text](http://url/to/img.png)

Please see Screenshot.png to see how the test window looks.

System preparation:
apt-get install gtk2.0
apt-get install gtk2.0-dev

The default build command and binary is for Debian x86 
For the Raspberry Pi, simply replace /usr/lib/x86_64-linux-gnu for /usr/lib/arm-linux-gnueabihf
and adjust the Geany project base directory to match your project location.

The build command needs a bunch of library references, but that's been done, look at the project file or main.cpp.

Also just as a comparison, a minimal window in Xlib uses 5 MBytes of RAM, while a minimal Gtk2 window uses 15 MBytes.
Gtk2 seems a nice compromise between size and performance for many applications.

Enjoy!!
