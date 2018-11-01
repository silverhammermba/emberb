require 'c_api'

include RSpec

describe CAPI do
  context "when not testing API functions" do
    it "VM works" do
      expect(CAPI.run_c("").out).to eq ""
    end

    it "VM works with setup/finalize" do
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

    it "invalid input raises" do
      expect { CAPI.run_c("{") }.to raise_error CAPI::CompilerError
    end

    it "runtime errors raise" do
      expect { CAPI.run_c("ruby_cleanup(0)") }.to raise_error CAPI::RuntimeError
    end
  end
end
