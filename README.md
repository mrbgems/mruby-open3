# mruby-open3   [![Build Status](https://travis-ci.org/k0kubun/mruby-open3.svg?branch=master)](https://travis-ci.org/k0kubun/mruby-open3)
Open3 class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'k0kubun/mruby-open3'
end
```
## example
```ruby
p Open3.hi
#=> "hi!!"
t = Open3.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
