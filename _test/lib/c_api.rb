require 'open3'
require 'tempfile'

RUBY_VERSION = "2.5"

module CAPI
  class CompilerError < StandardError; end

  def self.run_c code
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

    opts = `pkg-config --cflags --libs ruby-#{RUBY_VERSION}`

    exe = Tempfile.new('capi')

    begin
      exe.close
      puts "Building:"
      out, err, status = Open3.capture3("gcc -o #{exe.path} -xc - #{opts}", stdin_data: code)

      unless status.success?
        raise CompilerError, err
      end

      puts "Running:"
      out, err, status = Open3.capture3(exe.path)
      # TODO what does a segfault look like?
      puts out
      puts err
      p status

      return out
    ensure
      exe.unlink
    end
  end

  # TODO would also be cool to have a method that compiles a lib and then requires it
end
