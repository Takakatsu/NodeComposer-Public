--03_GetElements
--vector<bool>|vector<int>|vector<float>|vector<Tone>|vector<Chord>|vector<Melody>-vector<int>
--vector<bool>|vector<int>|vector<float>|vector<Tone>|vector<Chord>|vector<Melody>

function Main(v,i_v)
    local vec={}
    for i=1,#i_v do
        vec[i]=v[i_v[i]]
    end
    return vec
end