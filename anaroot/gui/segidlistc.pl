#!/usr/bin/perl

$segidlisth=$ARGV[0];
print("converting ".$csvfilename." to ui_segidlist.hh\n");

open(SIDFILE,"<".$segidlisth);
@sids=<SIDFILE>;
close(SIDFILE); 

open(UIFILE,">ui_segidlist.hh");

@names = ("Device","Focalplane","Detector","Module","Scaler");
$defs[64];
$nid = 0;

foreach (@sids){
    chomp;
    if(/^\/\//){
	@v = split(/ /,$_);
	if($v[1] eq $names[$nid]){
	    if($nid > 0){
		print UIFILE "QString ".$names[$nid-1]."Name[64]={";
		for($i=0;$i<64;$i++){
		    print UIFILE "\"".$defs[$i]."\"";
		    if($i<63){
			print UIFILE ",";
		    }
		    else{
			print UIFILE "};\n";
		    }
		}
	    }
	    $nid ++;
	    for($i=0;$i<64;$i++){$defs[$i] = $i.":";}
	    print $v[1]."\n"; 
	}
    }
    if($nid > 0){
	if(/^static/){ 
	    @v = split(/\s+/,$_);
	    $v[5] =~ s/\;//; 
	    #print $v[3]." ".$v[5]."\n"; 
	    $len = length $defs[$v[5]];
	    if($len > 0){$defs[$v[5]] = $defs[$v[5]].",";}
	    $defs[$v[5]] = $defs[$v[5]].$v[3];
	}
    }
}

close(UIFILE);
