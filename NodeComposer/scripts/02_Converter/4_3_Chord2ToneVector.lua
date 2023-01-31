--4_3_Chord2ToneVector
--Chord
--vector<Tone>

function Main(x)
    local tones={}
    for i=1,#(x[2]) do
        tones[#tones+1]={x[1],x[2][i],x[3],x[4]}
    end
    return tones
end