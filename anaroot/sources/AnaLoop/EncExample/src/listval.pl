#!/usr/bin/perl

listval();

sub listval {
    my %analyser_hash = analyser();
    foreach my $key (sort {$a <=> $b} keys %analyser_hash){
	printf "   %-10s %4d\n", $analyser_hash{$key}, $key;
    }
    print "\n";

    print " input Analyser number (less than 0 for all): ";
    my $analyser_num = <STDIN>;
    chomp($analyser_num);
    trim_space($analyser_num);
    if($analyser_num eq ""){
	print "\n";
	exit;
    }

    my @analyser_array;
    if($analyser_num < 0){
	my $i = 0;
	foreach my $key (sort {$a <=> $b} keys %analyser_hash){
	    $analyser_array[$i] = $analyser_hash{$key};
	    ++$i;
	}
    }else{
	my $analyser = $analyser_hash{$analyser_num};
	print "\n";
	if(!$analyser){
	    print "$analyser_num don't exist\n\n";
	    exit;
	}else{
	    $analyser_array[0] = $analyser;
	}
    }

    foreach $analyser (@analyser_array){
	my %wnum_hash = wnum($analyser);
	foreach $key (sort {$a <=> $b} keys %wnum_hash){
	    printf "   %-10s %4d\n", $wnum_hash{$key}, $key;
	}
	print "\n";
    }
}

sub analyser() {

    open(IN,"$ENV{TARTSYS}/sources/AnaLoop/EncExample/include/EAnalyser.hh") || die "$!";

    my $parenthesis = 0;
    my $num_last = -1;
    my %analyser_hash;

    print "\n -- EAnalyser --\n\n";
    while(<IN>){
	if(/\}/){
	    --$parenthesis;
	}
	if($parenthesis == 2){
	    my $str = $_;
	    if($str =~ /\/\//){
		$str = $`;
	    }
	    $str = trim_space($str);
	    $str =~ s/\n//;
	    if($str){
		my $analyser;
		my $num;
		if(/=/){
		    $analyser = $str;
		    $analyser =~ s/(.*)=(.*)/$1/;
		    chomp($analyser);
		    $analyser =~ s/^ *(.*?) *$/$1/;
		    $analyser = trim_space($analyser);
		    $num = $str;
		    $num =~ s/(.*)=(.*),(.*)/$2/;
		    chomp($num);
		    $num = trim_space($num);
		}else{
		    $analyser = $str;
		    $analyser =~ s/(.*),(.*)/$1/;
		    chomp($analyser);
		    $analyser = trim_space($analyser);
		    $num = ++$num_last;
		}
		if($analyser){
		    $analyser_hash{$num}=$analyser;
		    $num_last = $num;
		}
	    }
	}
	if(/\{/){
	    ++$parenthesis;
	}
    }
    close(IN);

    return %analyser_hash;
}

sub wnum() {
    my $analyser = shift;

    open(IN,"$ENV{TARTSYS}/sources/AnaLoop/EncExample/include/EWNum.hh") || die "$!";

    my $flag = 0;
    my $parenthesis = 0;
    my $num_last = -1;
    my %analyser_hash;

    print "\n ************************************************\n";
    while(<IN>){
	if(/\}/){
	    --$parenthesis;
	}
	if($parenthesis == 1){
	    if($_ =~ /namespace *$analyser *\{/){
		$flag = 1;
	    }else{
		$flag = 0;
	    }
	}
	if($parenthesis == 3 && $flag){
	    print $_;
	    my $str = $_;
	    if($str =~ /\/\//){
		$str = $`;
	    }
	    $str = trim_space($str);
	    $str =~ s/\n//;
	    if($str){
		my $analyser;
		my $num;
		if(/=/){
		    $analyser = $str;
		    $analyser =~ s/(.*)=(.*)/$1/;
		    chomp($analyser);
		    $analyser =~ s/^ *(.*?) *$/$1/;
		    $analyser = trim_space($analyser);
		    $num = $str;
		    $num =~ s/(.*)=(.*),(.*)/$2/;
		    chomp($num);
		    $num =~ s/\n//;
		    $num = trim_space($num);
		}else{
		    $analyser = $str;
		    $analyser =~ s/(.*),(.*)/$1/;
		    chomp($analyser);
		    $analyser = trim_space($analyser);
		    $num = ++$num_last;
		}
		if($analyser){
		    $analyser_hash{$num}=$analyser;
		    $num_last = $num;
		}
	    }
	}
	if(/\{/){
	    ++$parenthesis;
	}
    }
    print " ************************************************\n\n";
    print "\n -- EWNum::$analyser --\n\n";
    close(IN);

    return %analyser_hash;
}

sub trim_space {
    my $val = shift;
    $val =~ s/^ *(.*?) *$/$1/;
    return $val;
}
