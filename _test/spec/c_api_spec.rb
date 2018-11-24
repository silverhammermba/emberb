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

    it "returns a value" do
      expect(CAPI.run_c(<<-SOURCE
        int state;
        VALUE result = rb_eval_string_protect("Object", &state);

        printf("%d", result == rb_cObject);
        SOURCE
      ).out).to eq ?1
    end

    it "raises exceptions when not protected" do
      message = "Failed in a good way"

      expect(CAPI.run_c_blocks(<<-SOURCE
        VALUE should_raise(VALUE obj) {
          return rb_eval_string("foobar");
        }
        void ruby_main() {
          int state;
          rb_protect(should_raise, Qnil, &state);

          if (state) { printf("#{message}"); }
        }
      SOURCE
      ).out).to eq message
    end
  end

  describe "VALUEs" do
    it "can be type-checked with RB_TYPE_P" do
      expect(CAPI.run_c(<<-SOURCE
        if (!RB_TYPE_P(rb_cObject, T_CLASS)) { printf("failed"); }
      SOURCE
      ).out).to eq ""
    end

    it "can be type-checked dangerously with Check_Type" do
      message = "Failed in a good way"

      expect(CAPI.run_c_blocks(<<-SOURCE
        VALUE raise_unless_true(VALUE obj) {
          Check_Type(obj, T_TRUE);
          return Qnil;
        }
        void ruby_main() {
          int state;
          rb_protect(raise_unless_true, Qfalse, &state);

          if (state) { printf("#{message}"); }
        }
      SOURCE
      ).out).to eq message
    end

    it "can type-check for subclasses" do
      expect(CAPI.run_c(<<-SOURCE
        int state;
        VALUE obj = rb_eval_string_protect("class MyArray < Array; end; MyArray.new", &state);
        if (state || !RB_TYPE_P(obj, T_ARRAY)) { printf("failed"); }
      SOURCE
      ).out).to eq ""
    end

    it "can't type check using T_OBJECT" do
      expect(CAPI.run_c(<<-SOURCE
        int state;
        VALUE obj = rb_eval_string_protect("class MyArray < Array; end; MyArray.new", &state);
        if (state || RB_TYPE_P(obj, T_OBJECT)) { printf("failed"); }
      SOURCE
      ).out).to eq ""
    end

    it "can type check with T_OBJECT for custom classes" do
      expect(CAPI.run_c(<<-SOURCE
        int state;
        VALUE obj = rb_eval_string_protect("class Foo; end; class Bar < Foo; end; Bar.new", &state);
        if (state || !RB_TYPE_P(obj, T_OBJECT)) { printf("failed"); }
      SOURCE
      ).out).to eq ""
    end
  end
end
