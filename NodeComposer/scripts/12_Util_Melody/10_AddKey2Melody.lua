--10_AddKey2Melody
--Melody-int
--Melody

function Main(m, t)
    for i = 1, #m do
        m[i] = { m[i][1], { m[i][2][1] + t, m[i][2][2], m[i][2][3] } }
    end
    return m
end
