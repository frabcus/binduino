A scraper and two Arduino programs to work together.


scraper.js
==========

For use on ScraperWiki to scrape Liverpool bin collection times. 

It generates a small file which has simple instructions as to whether my bin
needs emptying tomorrow morning or not. send (below) reads that file. 

The scraper is written in Node.js.


send
====

Connects an Arduino to the Internet using an Ethernet shield. I plug this one
in in my living room, where the router is.

Polls, using HTTP, the file on ScraperWiki that scraper.js (above) makes.

Sends that state over a simple 434Mhz RF receiver, using the VirtualWire
protocol for error prevention so it works through the walls of my house. Note
that this is broadcast only - it's a one way protocol.


receive
=======

Reads the signal from the corresponding RF receiver. Lights a light
appropriately. This one goes on an Arduino next to the bin, and have some EL
wire to light it up.


