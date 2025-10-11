if (Test-Path -Path build) 
{
    "Path exists!"
} 
else 
{
    mkdir build
}

cmake -G "Visual Studio 17 2022" .\build