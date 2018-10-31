require 'c_api'

include RSpec

describe CAPI do
  context "when not testing API functions" do
    it "runs fine with no input" do
      expect(CAPI.run_c("").out).to eq ""
    end

    it "raises on invalid input" do
      expect { CAPI.run_c("{") }.to raise_error CAPI::CompilerError
    end
  end
end
