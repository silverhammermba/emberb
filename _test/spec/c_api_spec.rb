require 'c_api'

include RSpec

describe CAPI do
  context "when doing nothing" do
    it "runs fine" do
      expect(CAPI.run_c("void ruby_main() rb_eval_string_protect(\"puts 'Hello, world!'\", NULL);}")).to eq ""
    end
  end
end
