Two programs, for two separate Ardino's, to work together.

Send
====

Connects the Arduino to the Internet using an Ehternet shield. I plug this one
in in my living room, where the router is.

Polls, using HTTP, a particular file on ScraperWiki, which has simple
instructions as to whether my bin needs emptying tomorrow morning or not.

Sends that sate over a simple 434Mhz RF receiver, using the VirtualWire
protocol so it works through the walls of my house.


Receive
=======

Reads the signal from the corresponding RF receiver. Lights a light appropriately.

