--20_MakeMelodyByVector
--int,vector<int>,Tone
--Melody

function Main(t,keys,tone)
    local m={}
    for i=1,#keys do
        if keys[i]>=0 then
            m[#m+1]={(i-1)*t,{tone[1],keys[i],tone[3],t}}
        end
    end
    return m
end