--00_YourScale
--int-vector<int>
--vector<int>

function Main(root,scale)
    for i=1,#scale do
        scale[i]=scale[i]+root
    end
    return scale
end