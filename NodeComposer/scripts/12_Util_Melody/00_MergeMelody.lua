--00_MergeMelody
--Melody-Melody
--Melody

function Main(m1, m2)
    for i = 1, #m2 do
        m1[#m1 + 1] = { m2[i][1], m2[i][2] }
    end
    return m1
end
