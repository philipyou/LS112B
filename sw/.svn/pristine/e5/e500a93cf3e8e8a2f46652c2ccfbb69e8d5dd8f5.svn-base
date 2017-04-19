$path=$ARGV[0];
open file, $path;
@f = <file>;
close file;
open file, ">$path";
$path =~ s/\/.*//;
open file1, ">$path/memmap.format";
$xaddr = 0x4000;
$paddr = 0x4c00;
$oaddr = $saddr = 0;
for($i = 0;$i < @f;$i++) { 
	$_ = $f[$i];
	$start = 1 if(/^\s*memalloc\s*\(\s*$/);
	$start = 2 if(/^\s*xmemalloc\s*\(\s*$/);
	$start = 3 if(/^\s*pmemalloc\s*\(\s*$/);
	$start = 4 if(/^\s*omemalloc\s*\(\s*$/);
	if($start) {
		if(/^\s*([0-9]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/) {
			if($start == 1) {
				$str = sprintf "0x%04x %s%s\n", $addr, $2, $3;
				$addr += $1;
				$bstr .= $str;
				$sstr .= $str;
			} elsif($start == 2) {
				$str = sprintf "0x%04x %s%s\n", $xaddr, $2, $3;
				$xaddr += $1;
				$xstr .= $str;
			} elsif($start == 3) {
				$str = sprintf "0x%04x %s%s\n", $paddr, $2, $3;
				$paddr += $1;
				$xstr .= $str;
			} else {
				push @ostr, $_;
			}
		}
		if(/^\s*\)\s*$/) {
			$start = 0;
			push @omalloc,[@ostr] if(@ostr > 0);
			@ostr = ();
		}
	} else {
		$sstr .= $_;
	}
}

for($i = 0;$i <= $#omalloc;$i++) {
	for($j = 0, $oaddr = $xaddr;$j <= $#{$omalloc[$i]};$j++) {
		if($omalloc[$i][$j] =~ /^\s*([0-9]+)\s+([a-zA-Z0-9_]+)\s*(.*)$/) {
			$str = sprintf "0x%04x %s%s\n", $oaddr, $2, $3;
			$oaddr += $1;
			$xstr .= $str;
		}
	}
	$xend = $oaddr if($oaddr > $xend);
}
print file $xstr, $sstr;
print file1 $bstr, $xstr;
close file;
close file1;
printf "\nLast allocated address is %04x\n", $addr;
printf "\nLast allocated xmem address is %04x\n", $xend;   
printf "\nLast allocated pmem address is %04x\n", $paddr;