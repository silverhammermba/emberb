def think ai, player
  # get my position
  ax, ay = ai.pos

  # get direction that player moved
  dx, dy = player.dir

  # ... movement logic ...
  x = dy
  y = -dx

  # move in this direction
  ai.move x, y
end
