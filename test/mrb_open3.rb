##
## Open3 Test
##

assert("Open3#hello") do
  t = Open3.new "hello"
  assert_equal("hello", t.hello)
end

assert("Open3#bye") do
  t = Open3.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("Open3.hi") do
  assert_equal("hi!!", Open3.hi)
end
