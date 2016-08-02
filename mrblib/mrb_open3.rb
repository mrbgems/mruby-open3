module Open3
  def capture3
    p spawn("ruby", "-e", "$stdout.puts 'out'; $stderr.puts 'err'", {})
  end
  module_function :capture3
end
