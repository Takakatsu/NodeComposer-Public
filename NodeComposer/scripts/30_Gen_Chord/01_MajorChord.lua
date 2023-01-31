--01_MajorChord
--Tone
--Chord

function Main(tone)
    return {tone[1],{tone[2],tone[2]+4,tone[2]+7},tone[3],tone[4]}
end