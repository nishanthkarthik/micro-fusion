$filepath = Read-Host 'Enter the location of file';
$targetfilepath = Read-Host 'Enter the path for result (with backslash at end)'

$writer = [System.IO.StreamWriter] (($targetfilepath + 'SerialLogResult'))

$inarray = [IO.File]::ReadAllLines($filepath);
[System.Collections.ArrayList] $inarraymod = $inarray;

for ($i = 0; $i -le $inarraymod.Count; ++$i)
{
    if ($inarraymod[$i] -ne $null)
    {
        if ($inarraymod[$i].Equals("FIFO overflow!"))
        {
            echo ('Position ' + $i.ToString())
            $inarraymod.RemoveAt($i);
            $inarraymod.RemoveAt($i-1);
        }
    }
}

for ($j = 0; $j -le $inarraymod.Count; ++$j)
{
    $writer.WriteLine($inarraymod[$j]);
}
echo ('Result file written at ' + $targetfilepath)
$writer.Close();
echo 'Writer closed'