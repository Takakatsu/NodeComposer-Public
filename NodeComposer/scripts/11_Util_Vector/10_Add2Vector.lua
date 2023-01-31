--10_Add2Vector
--vector<int>|vector<float>-int|float
--vector<int>|vector<float>

function Main(v,x)
    for i=1,#v do
        v[i]=v[i]+x
    end
    return v
end