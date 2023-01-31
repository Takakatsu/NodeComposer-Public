--00_Append2Vector
--vector<bool>|vector<int>|vector<float>|vector<Tone>|vector<Chord>|vector<Melody>-bool|int|float|Tone|Chord|Melody
--vector<bool>|vector<int>|vector<float>|vector<Tone>|vector<Chord>|vector<Melody>

function Main(v,x)
    v[#v+1] = x
    return v
end