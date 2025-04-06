$fileName = $args[0]
$outDirectory =  ".\..\..\bin"
$resolvedOutDirectory = Resolve-Path $outDirectory

"Starting Shader compile for '$fileName'"

if (Test-Path -Path $outDirectory)
{
    "Output Path Found at '$resolvedOutDirectory'"
} 
else 
{
    "Path not found. Creating new at '$resolvedOutDirectory'"
    mkdir $outDirectory
}

$csoPath = "$outDirectory\v_$fileName.cso"
$asmPath = "$outDirectory\v_$fileName.asm"

#These options will generate pdb output. These are symbols loaded by PIX to debug the shaders.
#-Zi -Fd

dxc ".\..\hlsl\$($fileName)_vs.hlsl" -T vs_6_0 -E VS -Fo $csoPath -Fc $asmPath -Od -Zi -Fd "$outDirectory\"

"$csoPath"
"$asmPath"

$csoPath = "$outDirectory\p_$fileName.cso"
$asmPath = "$outDirectory\p_$fileName.asm"

dxc ".\..\hlsl\$($fileName)_ps.hlsl" -T ps_6_0 -E PS -Fo $csoPath -Fc $asmPath -Od -Zi -Fd "$outDirectory\"

"$csoPath"
"$asmPath"