param (
    [Parameter(Mandatory=$true)][String]$day,
    [Parameter(Mandatory=$true)][String]$part
)
$common = './common/cpp'
$src = "./$day/cpp/$part"
$origin = pwd

echo "cleaning and copying standard files"
cp "$common\CMakePresets.json" $src

echo "Building: $src"
cd $src
cmake --preset release 
cmake --build --preset release


echo "Running: $src"
echo "example"
.\$day\cpp\$part\build\release\Release\aoc.exe .\$day\example.txt
echo "main"
.\$day\cpp\$part\build\release\Release\aoc.exe .\$day\input.txt
cd $origin
