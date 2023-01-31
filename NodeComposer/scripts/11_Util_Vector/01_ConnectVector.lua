--01_ConnectVector
--vector<bool>|vector<int>|vector<float>|vector<Tone>|vector<Chord>|vector<Melody>-vector<bool>|vector<int>|vector<float>|vector<Tone>|vector<Chord>|vector<Melody>
--vector<bool>|vector<int>|vector<float>|vector<Tone>|vector<Chord>|vector<Melody>

function Main(v1,v2)
    for i=1,#v2 do
        v1[#v1+i] = v2[i]
    end
    return v1
end