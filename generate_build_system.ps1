if (Test-Path -Path build) 
{
    "Path exists!"
} 
else 
{
    mkdir build
}

cd build
cmake ..\.
cd ..