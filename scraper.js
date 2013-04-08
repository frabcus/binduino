#!/usr/bin/node

var fs = require('fs')
var sqlite3 = require('sqlite3')
var async = require('async')
var http = require('http')
var request = require('request')
var jsdom = require('jsdom')
var assert = require('assert')
var moment = require('moment')
var events = require('events')
var util = require('util')

// Setup database
var db = new sqlite3.Database('scraperwiki.sqlite')
db.run("CREATE TABLE IF NOT EXISTS bin_dates (type TEXT, next TEXT, PRIMARY KEY (type))")

// Setup events framework
var ee = new events.EventEmitter()

// Error handler
var handle_error = function(message) {
  console.log("Error:", message)
  request.post("https://x.scraperwiki.com/api/status", {'form': {'type':'error', 'message': "" + message} })
  process.exit(1)
}
// Catch all uncaught exceptions
process.on('uncaughtException', handle_error)

// Write out the file which the Arduino's read - it sets:
// - light off
// R recycling light on
var write_bin_light_state = function(next_recycling) {
  // should the bin light be lit?
  next_recycling = moment(next_recycling, "YYYY-MM-DD")
  var end_range = next_recycling.clone().add("hours", 12) // stop midday of the day
  var start_range = next_recycling.clone().subtract("hours", 12) // start midday the day before
  var now = moment()

  //now = moment("2013-02-25 10:00:00")
  //console.log("faked now!", now.format())
  console.log("light bin range", +start_range, +end_range, +now)

  var light_bin = "-"
  if (start_range <= now && now <= end_range) {
    light_bin = "R"
  }
  var fs = require('fs');
  fs.writeFile("http/light.state", light_bin, function(err) {
      if (err) {
          return handle_error(err)
      } else {
          console.log("Set light.state to", light_bin)
      }
  }); 
}
ee.on('newNextRecycling', write_bin_light_state) 

// Read one row of data from table
var parse_row = function($, row, type) {
  row = row.map(function(i, e) {
    return $(e).text().trim()
  }).get()

  // XXX could change this to just use moment(row[0]) which calls Date.parse, esp.
  // if they change the formatting a lot
  var next_date = moment(row[0], "dddd D MMMM YYYY").format("YYYY-MM-DD")

  console.log(type, next_date)
  db.run("INSERT OR REPLACE INTO bin_dates (type, next) values (?, ?)", type, next_date)

  return next_date
}

// Read one whole page - which has a table of recycling collections for a house
var parse_page = function (err, response, body) {
  if (err) {
    return handle_error(err)
  } 
  if (response.statusCode != 200) {
    return handle_error("HTTP error: " + response.statusCode)
  }

  // parse using jQuery via jsdom
  jsdom.env({
    html: body,
    scripts: [ 'http://code.jquery.com/jquery-1.5.min.js' ]
  }, function (err, window) {
    if (err) {
      return handle_error(err)
    }
    var $ = window.jQuery

    // check header is as we expect
    var header = $('.bins thead th').map(function(i, e) {
      return $(e).text().trim()
    }).get()
    console.log(header)
    assert.deepEqual(header, [ '', 'Next date', '2nd date', 'Third date' ])

    // there's a th as well which we ignore for now
    var next_refuse = parse_row($, $('.Refuse td'), 'refuse')
    var next_recycling = parse_row($, $('.Recycling td'), 'recycling')

    request.post("https://x.scraperwiki.com/api/status", {'form': {'type':'ok'} })
    ee.emit('newNextRecycling', next_recycling)
  })
}

// Get the page with the data on from Liverpool Council
var get_bin_collections = function(err,data) {
  if (err) {
    return handle_error(err)
  }
  var json = JSON.parse(data)
  var postcode = json['postcode']
  var house = json['house']

  console.log("Postcode:", postcode, "House:", house)

  // Get the Liverpool council web page
  var url = "http://liverpool.gov.uk/bins-and-recycling/bin-collection-dates-and-times/results.aspx?housenumber=" + house + "&postcode=" + postcode + "&btnSend="
  console.log("Getting URL:", url)
  request(url, parse_page)
}

// Read JSON file with postcode and house name
var postcode, house
fs.readFile('postcode.txt', 'utf8', get_bin_collections)

