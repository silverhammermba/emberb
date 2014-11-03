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

    it "must accept a GMP::Integer" do
      x = GMP::Integer.new(4)
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end

    it "must accept a String" do
      x = '87678392019238123109238'
      GMP::Integer.new(x).to_s.must_equal x.to_s
    end

    it "must raise if the base is invalid" do
      # only accept base with strings
      proc { GMP::Integer.new(3, 10) }.must_raise TypeError
      # only accept fixnum bases
      proc { GMP::Integer.new('3', 1.5) }.must_raise TypeError
      # wrong base range
      proc { GMP::Integer.new('3', 64) }.must_raise RangeError
      # invalid base for string
      proc { GMP::Integer.new('3', 2) }.must_raise ArgumentError
      # good
      GMP::Integer.new('3', 9)
    end
  end

  describe "when converting to String" do
    it "must raise if the base is invalid" do
      x = GMP::Integer.new(3)

      # base must be Fixnum
      proc { x.to_s(1.5) }.must_raise TypeError
      # wrong base range
      proc { x.to_s(64) }.must_raise RangeError
      # good
      x.to_s(2)
    end
  end
end
