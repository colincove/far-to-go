if (Test-Path -Path build) 
{
    "Path exists!"
} 
else 
{
    mkdir build
}

cmake -G "Visual Studio 18 2026" .\build