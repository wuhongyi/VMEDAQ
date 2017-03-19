#!/usr/bin/perl
#
# script to analyze spectcl file
# dump xml file which can be easily compared with anaroot xml file

open(SPKFILE,"<".$ARGV[0]);
@lines=<SPKFILE>;
close(SPKFILE); 

# dump params to hash

%params = ();
foreach $l (@lines){
    @c=split(/\ /,$l);
    if($c[0] eq "treevariable"){
	$params{$c[2]} = $c[3];
#	print $c[2]." = ".$c[3]."\n";
    }
}

# make ppac xml

@xmlppacnames = ("F1PPAC-1",
		 "F1PPAC-2",
		 "F2PPAC-1",
		 "F2PPAC-2",
		 "F3PPAC-1A",
		 "F3PPAC-1B",
		 "F3PPAC-2A",
		 "F3PPAC-2B",
		 "F4PPAC",
		 "F5PPAC-1A",
		 "F5PPAC-1B",
		 "F5PPAC-2A",
		 "F5PPAC-2B",
		 "F6PPAC",
		 "F7PPAC-1A",
		 "F7PPAC-1B",
		 "F7PPAC-2A",
		 "F7PPAC-2B",
		 "F8PPAC-1A",
		 "F8PPAC-1B",
		 "F8PPAC-2A",
		 "F8PPAC-2B",
		 "F9PPAC-1A",
		 "F9PPAC-1B",
		 "F9PPAC-2A",
		 "F9PPAC-2B",
		 "F10PPAC-1A",
		 "F10PPAC-1B",
		 "F10PPAC-2A",
		 "F10PPAC-2B",
		 "F11PPAC-1A",
		 "F11PPAC-1B",
		 "F11PPAC-2A",
		 "F11PPAC-2B");

@spkppacnames = ("brips.f1.ppac1",
		 "brips.f1.ppac2",
		 "brips.f2.ppac1",
		 "brips.f2.ppac2",
		 "brips.f3.ppac1.ppacA",
		 "brips.f3.ppac1.ppacB",
		 "brips.f3.ppac2.ppacA",
		 "brips.f3.ppac2.ppacB",
		 "brips.f4.ppac",
		 "brips.f5.ppac1.ppacA",
		 "brips.f5.ppac1.ppacB",
		 "brips.f5.ppac2.ppacA",
		 "brips.f5.ppac2.ppacB",
		 "brips.f6.ppac",
		 "brips.f7.ppac1.ppacA",
		 "brips.f7.ppac1.ppacB",
		 "brips.f7.ppac2.ppacA",
		 "brips.f7.ppac2.ppacB",
		 "brips.f8.ppac1.ppacA",
		 "brips.f8.ppac1.ppacB",
		 "brips.f8.ppac2.ppacA",
		 "brips.f8.ppac2.ppacB",
		 "brips.f9.ppac1.ppacA",
		 "brips.f9.ppac1.ppacB",
		 "brips.f9.ppac2.ppacA",
		 "brips.f9.ppac2.ppacB",
		 "brips.f10.ppac1.ppacA",
		 "brips.f10.ppac1.ppacB",
		 "brips.f10.ppac2.ppacA",
		 "brips.f10.ppac2.ppacB",
		 "brips.f11.ppac1.ppacA",
		 "brips.f11.ppac1.ppacB",
		 "brips.f11.ppac2.ppacA",
		 "brips.f11.ppac2.ppacB");

#15 parameters for ppac
@xmlppacpnames = ("a_ch2ns",
		  "x1_ch2ns",
		  "y1_ch2ns",
		  "x2_ch2ns",
		  "y2_ch2ns",
		  "xfactor",
		  "yfactor",
		  "xoffset",
		  "yoffset",
		  "xns_off",
		  "yns_off",
		  "xpos_off",
		  "ypos_off",
		  "xzpos",
		  "yzpos");

@spkppacpnames = ("ch2ns.0",
		  "ch2ns.1",
		  "ch2ns.2",
		  "ch2ns.3",
		  "ch2ns.4",
		  "xns2mm.1",
		  "yns2mm.1",
		  "xns2mm.0",
		  "yns2mm.0",
		  "xnsoffset",
		  "ynsoffset",
		  "xmmoffset",
		  "ymmoffset",
		  "xzpos",
		  "yzpos");

open(XMLFILE,">BigRIPSPPAC.xml");
print XMLFILE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<dataroot>\n";

$cdet = 1;
foreach $det ( @xmlppacnames ) {
    print XMLFILE "<BigRIPSPPAC>\n";
    print XMLFILE "<ID>$cdet</ID>\n";
    print XMLFILE "<NAME>$det</NAME>\n";
    $cp = 0;
    foreach $p ( @xmlppacpnames ) {
	$pp = $spkppacnames[$cdet-1].".".$spkppacpnames[$cp];
	print XMLFILE "<$p>$params{$pp}</$p>\n";
#	print "<$p>$pp</$p>\n";
	$cp ++;
    }
    print XMLFILE "</BigRIPSPPAC>\n";
    $cdet ++;
}

print XMLFILE "<dataroot>\n";

# make plastic xml

@xmlplanames = ("F2pl",
		"F3pl",
		"F5pl-1",
		"F5pl-2",
		"F7pl",
		"F8pl",
		"F11pl-1",
		"F11pl-2",
		"F12pl");

@spkplanames = ("brips.f2.scint",
		"brips.f3.scint",
		"brips.f5.scint",
		"brips.f5.scint2",
		"brips.f7.scint",
		"brips.f8.scint",
		"brips.f11.scint1",
		"brips.f11.scint2",
		"brips.f12.scint1");

#3 parameters for pla
@xmlplapnames = ("tcal_left",
		 "tcal_right",
		 "zpos");

@spkplapnames = ("ch2ns.0",
		 "ch2ns.1",
		 "zpos");

open(XMLFILE,">BigRIPSPlastic.xml");
print XMLFILE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<dataroot>\n";

$cdet = 1;
foreach $det ( @xmlplanames ) {
    print XMLFILE "<BigRIPSPlastic>\n";
    print XMLFILE "<ID>$cdet</ID>\n";
    print XMLFILE "<NAME>$det</NAME>\n";
    $cp = 0;
    foreach $p ( @xmlplapnames ) {
	$pp = $spkplanames[$cdet-1].".".$spkplapnames[$cp];
	print XMLFILE "<$p>$params{$pp}</$p>\n";
#	print "<$p>$pp</$p>\n";
	$cp ++;
    }
    print XMLFILE "</BigRIPSPlastic>\n";
    $cdet ++;
}

print XMLFILE "<dataroot>\n";

# make ic xml

@xmlicnames = ("F2IC",
	       "F3IC",
	       "F7IC",
	       "F11IC");

@spkicnames = ("brips.f2.ic",
	       "brips.f3.ic",
	       "brips.f7.ic",
	       "brips.f11.ic");

@xmlicpnames = ("ch2mev_0",
		"ch2mev_1");

@spkicpnames = ("ch2mev.0",
		 "ch2mev.1");

open(XMLFILE,">BigRIPSIC.xml");
print XMLFILE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<dataroot>\n";

$cdet = 1;
foreach $det ( @xmlicnames ) {
    print XMLFILE "<BigRIPSIC>\n";
    print XMLFILE "<ID>$cdet</ID>\n";
    print XMLFILE "<NAME>$det</NAME>\n";
    $cp = 0;
    foreach $p ( @xmlicpnames ) {
	$pp = $spkicnames[$cdet-1].".".$spkicpnames[$cp];
	print XMLFILE "<$p>$params{$pp}</$p>\n";
#	print "<$p>$pp</$p>\n";
	$cp ++;
    }
    print XMLFILE "</BigRIPSIC>\n";
    $cdet ++;
}

print XMLFILE "<dataroot>\n";

