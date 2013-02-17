A scraper and two Arduino programs to work together.


scraper.js
==========

For use on ScraperWiki to scrape Liverpool bin collection times, and generate
a small file 
which has simple
instructions as to whether my bin needs emptying tomorrow morning or not

that Send reads. The scraper is written in Node.


Send
====

Connects an Arduino to the Internet using an Ehternet shield. I plug this one
in in my living room, where the router is.

Polls, using HTTP, a particular file on ScraperWiki, which has simple
instructions as to whether my bin needs emptying tomorrow morning or not.

Sends that sate over a simple 434Mhz RF receiver, using the VirtualWire
protocol so it works through the walls of my house.


Receive
=======

Reads the signal from the corresponding RF receiver. Lights a light appropriately.


