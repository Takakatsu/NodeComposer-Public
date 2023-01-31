--20_GetMelodyTime
--Melody
--int

function Main(m)
    local t = 0
    for i = 1, #m do
        t = math.max(t, m[i][1] + m[i][2][4])
    end
    return t
end