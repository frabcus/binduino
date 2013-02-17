#!/usr/bin/node

var fs = require('fs')
var sqlite3 = require('sqlite3')
var async = require('async')
var http = require('http');
var request = require('request');
var jsdom = require('jsdom');
var assert = require('assert');

var parse_row = function(row, type) {

}

// Read JSON file with postcode and house name
var postcode, house
fs.readFile('postcode.txt', 'utf8', function (err,data) {
  if (err) {
    return console.log(err)
  }
  var json = JSON.parse(data)
  postcode = json['postcode']
  house = json['house']

  console.log("Postcode:", postcode, "House:", house)

  // Create the SQLite database if necessary
  var db = new sqlite3.Database('scraperwiki.sqlite')
  db.run("CREATE TABLE IF NOT EXISTS bin_dates (rubbish_type TEXT, date TEXT)")

  // Get the Liverpool council web page
  var url = "http://liverpool.gov.uk/bins-and-recycling/bin-collection-dates-and-times/results.aspx?housenumber=" + house + "&postcode=" + postcode + "&btnSend="
  console.log("Getting URL:", url)
  request(url, function (error, response, body) {
    if (error || response.statusCode != 200) {
      console.log(err)
    } else {
      // Parse the body
      jsdom.env({
        html: body,
        scripts: [ 'http://code.jquery.com/jquery-1.5.min.js' ]
      }, function (err, window) {
        var $ = window.jQuery;

        // check header is as we expect
        var header = $('.bins thead th').map(function(i, e) {
          return $(e).text().trim()
        }).get()
        console.log(header)
        assert.deepEqual(header, [ '', 'Next date', '2nd date', 'Third date' ])
//        console.log($('.Refuse').html());
      }); 

    }
  })

})
