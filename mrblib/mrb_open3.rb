module Open3
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
    until remaining_ios.empty?
      readable_ios, = IO.select(remaining_ios)
      readable_ios.each do |io|
        begin
          loop do
            begin
              out = io.read_nonblock
              if io == out_r
                stdout << out
              else
                stderr << out
              end
            rescue Errno::EAGAIN
              break
            end
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
