$defs = "";
$fmt = 0;
sub reva {
	return $_ if($defs =~ /REV[CD]/ || $fmt);
	if(/bmark([01])\s+([a-z0-9_]+)\s*,\s*([a-z0-9_]+)/) {
		$ret = sprintf("\tisolate%d %s,mark\n\tbranch %s,true\n", $1, $2, $3);
	}	elsif(/rtnmark([01])\s+([a-z0-9_]+)\s*/) {
		$ret = sprintf("\tisolate%d %s,mark\n\trtn true\n", $1, $2);
	} else {
		$ret = $_;
	}
	return $ret if($defs =~ /REVB/);
	if(/mul32\s+([a-z]+)\s*,\s*([a-z0-9_]+)\s*,\s*([a-z]+)/) {
		$ret = sprintf("\tmult %s,%s\n\tnop 19\n\tproduct %s\n", $2, $1, $3);
	}	elsif(/imul32\s+([a-z]+)\s*,\s*([a-z]+)/) {
		$ret = sprintf("\timult %s\n\tnop 19\n\tproduct %s\n", $1, $2);
	} else {
		$ret = $_;
	}
	return $ret;
}

sub parseif {
	my($fname) = @_;
	open file, $fname;
	@f = <file>;
	close file;
	open file, ">$fname";
	@valid = (1);
	for($i = 0;$i < @f;$i++) { 
		$_ = $f[$i];
		$defs .= $1 . " " if(/^define\s+(\w+)\s*/);
		if(/^ifdef\s+(\w+)\s*/) {
			push @valid, ($defs =~ /$1 / ? 1 : 0) & $valid[$#valid];
		} elsif(/^ifndef\s+(\w+)\s*/) {
			push @valid, ($defs =~ /$1 / ? 0 : 1) & $valid[$#valid];
		} elsif(/^else/) {
			printf "%s\nelse without if at line %d\n", $f[$i - 3], $i if($#valid == 0);
			$valid[$#valid] = (1 - $valid[$#valid]) & $valid[$#valid - 1];
		} elsif(/^endif/) {
			printf "%s\nendif without if at line %d\n", $_, $i if($#valid == 0);
			pop @valid;
		} elsif($valid[$#valid]) {
			if(/^include\s+(.+)\s*/) {
				open file2, "program/$1";
				@f2 = <file2>;
				close file2;
				splice(@f, $i + 1, 0, @f2);
			} else {
				next if(/^\s*bpatch\s+/ && $defs !~ /ROMCODE/);
				next if(/^\s*set[01] mark_ext_patch/ && $defs !~ /ROMCODE/);
				print file reva($_);
			}
		}
	}
	close file;
}

parseif("output/bt_program23.meta");
$fmt = 1;
parseif("output/bt_format.meta");


