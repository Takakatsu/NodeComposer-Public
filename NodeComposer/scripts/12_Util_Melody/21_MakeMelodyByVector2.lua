--21_MakeMelodyByVector2
--int,vector<int>,vector<int>,Tone
--Melody

function Main(t,keys,lens,tone)
    local m={}
    for i=1,math.min(#keys,#lens) do
        if keys[i]>=0 then
            m[#m+1]={(i-1)*t,{tone[1],keys[i],tone[3],t*lens[i]}}
        end
    end
    return m
end