complex = {}	--global parameter, module name

function complex.new(r, i)
	return {r=r,i=i}
end


--define a const parameter i

complex.i = complex.new(0, 1)

function complex.add(c1, c2)
	return complex.new(c1.r+c2.r, c1.i+c2.i)
end

function complex.sub(c1, c2)
	return complex.new(c1.r-c2.r, c1.i-c2.i)
end


-- return module
return complex
