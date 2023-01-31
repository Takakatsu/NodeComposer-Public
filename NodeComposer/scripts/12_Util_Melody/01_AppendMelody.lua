--01_AppendMelody
--Melody-Melody
--Melody

function Main(m1, m2)
    local t = 0
    for i = 1, #m1 do
        t = math.max(t, m1[i][1] + m1[i][2][4])
    end
    for i = 1, #m2 do
        m1[#m1 + 1] = { t + m2[i][1], m2[i][2] }
    end
    return m1
end
