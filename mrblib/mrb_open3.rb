module Open3
  # @param [Array<String>] - command to execute
  # @return [String, String, Process::Status] - stdout, stderr, status
  def capture3(*cmd)
    out_r, out_w = IO.pipe
    err_r, err_w = IO.pipe
    pid = spawn(*cmd, { 'out' => out_w.to_i, 'err' => err_w.to_i })

    out_w.close
    err_w.close

    stdout = ''
    stderr = ''

    out_reader = Thread.new(out_r.to_i, stdout) { |fd, result| io = IO.new(fd); result << io.read; io.close }
    err_reader = Thread.new(err_r.to_i, stderr) { |fd, result| io = IO.new(fd); result << io.read; io.close }

    out_reader.join
    err_reader.join

    _, status = Process.waitpid2(pid)
    [stdout, stderr, status]
  end
  module_function :capture3
end
