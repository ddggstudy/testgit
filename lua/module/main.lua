local c = require("complex")

local c1 = c.new(100, 200)
local c2 = c.new(1000, 2000)
local subc = c.sub(c1, c2)
print(subc.r.."aa"..subc.i)

