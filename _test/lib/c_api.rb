require 'open3'
require 'tempfile'

BUILD_VERSION = "2.5"

module CAPI
  class Result
    def initialize out, err, status
      @out = out
      @err = err
      @status = status
    end

    attr_reader :out, :err, :status
  end

  class CompilerError < StandardError; end
  class RuntimeError < StandardError; end

  # run a block of C code with the VM already initalized, must include an entry point:
  # void ruby_main(void)
  def self.run_c_blocks code
    code = <<-SOURCE
      #include <ruby.h>
      #include <stdio.h>

      #{code}

      int main(int argc, char* argv[])
      {
        ruby_init();
        ruby_main();
        return ruby_cleanup(0);
      }
    SOURCE

    opts = `pkg-config --cflags --libs ruby-#{BUILD_VERSION}`

    exe = Tempfile.new('capi')

    begin
      exe.close
      out, err, status = Open3.capture3("gcc -o #{exe.path} -xc - #{opts}", stdin_data: code)

      unless status.success?
        raise CompilerError, err
      end

      result = Result.new *Open3.capture3(exe.path)
      # TODO what does a segfault look like?
      unless result.status.success?
        raise RuntimeError, result.err
      end

      return result
    ensure
      exe.unlink
    end
  end

  # run C statements with the Ruby VM initialized, in the context of a top-level function
  def self.run_c source
    run_c_blocks "void ruby_main() { #{source}; }"
  end

  # TODO would also be cool to have a method that compiles a lib and then requires it
end
