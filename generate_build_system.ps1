if (Test-Path -Path build) 
{
    "Path exists!"
} 
else 
{
    mkdir build
}

cmake .\ -B .\build