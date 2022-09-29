#!/usr/bin/env ruby
require("gpib")

gpib = Gpib.new(20)
gpib.send("*IDN?")
res = gpib.receive
p res

