param (
    [Parameter(Mandatory=$true)][String]$day,
    [Parameter(Mandatory=$true)][String]$part
)
$common = "./common/cpp"
$src = "./$day/cpp/$part"
$origin = pwd

echo "cleaning and copying standard files"
cp "$common\CMakePresets.json" $src

echo "Building: $src"
cd $src
cmake --preset release 
cmake --build --preset release
cd $origin

$exe = "$src/build/release/Release/aoc.exe"
echo "Running: $exe"
echo "example:"
Start-Process -FilePath $exe -ArgumentList ['./$day/example.txt'] -Wait
echo "main:"
Start-Process -FilePath $exe -ArgumentList ['./$day/input.txt'] -Wait
