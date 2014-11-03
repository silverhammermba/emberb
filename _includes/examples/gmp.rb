require './ext/gmp'

class GMP::Integer
  def to_i
    to_s.to_i
  end

  def == other
    return (self <=> other) == 0 if other.is_a? self.class
    super
  end

  alias :inspect :to_s
end
