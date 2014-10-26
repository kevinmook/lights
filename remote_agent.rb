#!/usr/bin/env ruby
require 'bundler'
Bundler.require

$LOAD_PATH << File.expand_path(File.join(File.dirname(__FILE__), 'lib'))
require 'remote_agent'

if __FILE__ == $0
  RemoteAgent.new().run
end
