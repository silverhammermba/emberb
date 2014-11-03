require 'minitest/autorun'
require './gmp'

describe GMP::Integer do
  describe "when initialized" do
    it "must accept a Fixnum" do
      x = 3
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end

    it "must accept a Bignum" do
      x = 99999999999999999999999999999999
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end

    it "must accept a String" do
      x = '87678392019238123109238'
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end
  end
end
