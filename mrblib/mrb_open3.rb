module Open3
  # @param [Array<String>] - command to execute
  # @return [String, String, Process::Status] - stdout, status
  def capture2(*cmd)
    stdout, stderr, status = capture3(*cmd)
    $stderr.print(stderr)
    [stdout, status]
  end
  module_function :capture2

  # @param [Array<String>] - command to execute
  # @return [String, String, Process::Status] - stdout, stderr, status
  def capture3(*cmd)
    opts = {}
    if cmd.last.is_a?(Hash)
      opts = cmd.pop.dup
    end
    out_r, out_w = IO.pipe
    err_r, err_w = IO.pipe
    opts[:out] = out_w.to_i
    opts[:err] = err_w.to_i
    pid = spawn(*cmd, opts)

    out_w.close
    err_w.close

    stdout = ''
    stderr = ''

    remaining_ios = [out_r, err_r]
    buf = ''
    until remaining_ios.empty?
      readable_ios, = IO.select(remaining_ios)
      readable_ios.each do |io|
        begin
          io.sysread(1024, buf)
          if io == out_r
            stdout << buf
          else
            stderr << buf
          end
        rescue EOFError
          io.close unless io.closed?
          remaining_ios.delete(io)
        end
      end
    end

    _, status = Process.waitpid2(pid)
    [stdout, stderr, status]
  end
  module_function :capture3
end
