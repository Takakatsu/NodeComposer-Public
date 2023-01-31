--02_JoinMelody
--Melody-Melody-int
--Melody

function Main(m1, m2, t)
    for i = 1, #m2 do
        m1[#m1 + 1] = { t + m2[i][1], m2[i][2] }
    end
    return m1
end
