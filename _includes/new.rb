class Class
  def new *args, &blk
    obj = allocate
    obj.initialize(*args, &blk)
    obj
  end

  def allocate
    # create and return an empty instance
  end
end
