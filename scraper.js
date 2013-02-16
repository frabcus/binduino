#!/usr/bin/node

var fs = require('fs')
var sqlite3 = require('sqlite3')
var async = require('async')

var postcode, house
fs.readFile('postcode.txt', 'utf8', function (err,data) {
  if (err) {
    return console.log(err)
  }
  var json = JSON.parse(data)
  postcode = json['postcode']
  house = json['house']
  console.log("Postcode:", postcode, "House:", house)
})

var db = new sqlite3.Database('scraperwiki.sqlite')
db.run("CREATE TABLE IF NOT EXISTS bin_dates (rubbish_type TEXT, date TEXT)")

var url = "http://liverpool.gov.uk/bins-and-recycling/bin-collection-dates-and-times/results.aspx?housenumber=" + house + "&postcode=" + postcode + "&btnSend="
console.log(url)

