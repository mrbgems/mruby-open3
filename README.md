# mruby-open3 [![Build Status](https://travis-ci.org/k0kubun/mruby-open3.svg?branch=master)](https://travis-ci.org/k0kubun/mruby-open3)

Open3 for mruby

## Installation

```ruby
MRuby::Build.new do |conf|
  # ...
  conf.gem mgem: 'mruby-open3'
end
```

## Usage

Currently only `Open3.capture3` is supported.

```rb
Open3.capture3("ruby", "-e", "$stdout.puts 'out'; $stderr.puts 'err'; exit 2")
#=> ["out\n", "err\n", #<Process::Status: pid=3903,exited(2)>]
```

## License

MIT License
