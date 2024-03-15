param (
    [Parameter(Mandatory=$true)][String]$day,
    [Parameter(Mandatory=$true)][String]$part
)
$common = "./common/cpp"
$src = "./$day/cpp/$part"
$origin = pwd

echo "cleaning and copying standard files"
cp "$common\CMakePresets.json" $src
cp "$common\.clang-format" $src
cp "$common\.clang-tidy" $src

echo "Building: $src"
cd $src
cmake --preset release 
cmake --build --preset release
cd $origin

$exe = "$src/build/release/Release/aoc.exe"
echo "Running: $exe"
echo "example:"
Start-Process -Wait -NoNewWindow -FilePath $exe -ArgumentList "./$day/example.txt"  
echo ""
echo "main:"
Start-Process -Wait -NoNewWindow -FilePath $exe -ArgumentList "./$day/input.txt"  
