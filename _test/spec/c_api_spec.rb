require 'c_api'

include RSpec

describe CAPI do
  describe "setup" do
    it "works with no code" do
      expect(CAPI.run_c("").out).to eq ""
    end

    it "works with setup/finalize" do
      expect(CAPI.run_c_main(<<-SOURCE
        int main(int argc, char* argv[])
        {
          if (ruby_setup())
          {
            printf("Failed");
          }
          else
          {
            ruby_finalize();
          }

          return 0;
        }
      SOURCE
      ).out).to eq ""
    end

    it "raises when code is invalid" do
      expect { CAPI.run_c("{") }.to raise_error CAPI::CompilerError
    end

    it "raises when code causes segfault" do
      expect { CAPI.run_c("ruby_cleanup(0)") }.to raise_error CAPI::RuntimeError
    end
  end

  describe "eval" do
    it "works with puts" do
      expect(CAPI.run_c(<<-SOURCE
        int state;
        rb_eval_string_protect("puts 'Hello, world!'", &state);

        if (state) { printf("Error"); }
        SOURCE
      ).out).to eq "Hello, world!\n"
    end

    it "fails with invalid code" do
      message = "Failed in a good way"

      expect(CAPI.run_c(<<-SOURCE
        int state;
        rb_eval_string_protect("foobar", &state);

        if (state) { printf("#{message}"); }
        SOURCE
      ).out).to eq message
    end
  end
end
