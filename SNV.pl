use strict;
sub usage{
	die"Usage: perl $0 <vcf(.gz)|genomatrix(.gz)>",unless(@ARGV==1);
}
usage;
if($ARGV[0]=~/\.gz$/){
	open A,"gzip -dc $ARGV[0]|";
}
else{
	open A,"$ARGV[0]" ;
}
my($AT,$AG,$AC,$TA,$TC,$TG,$GA,$GT,$GC,$CA,$CT,$CG)=(0,0,0,0,0,0,0,0,0,0,0,0);

while(<A>){
	chomp;
	next if /^#/;
	my @line=split;
	my $ref=$line[3];
	my $alt=$line[4];
	if($ref eq "G" && $alt eq "A"){$GA+=1;}
	if($ref eq "G" && $alt eq "T"){$GT+=1;}
	if($ref eq "G" && $alt eq "C"){$GC+=1;}
	if($ref eq "C" && $alt eq "A"){$CA+=1;}
	if($ref eq "C" && $alt eq "T"){$CT+=1;}
	if($ref eq "C" && $alt eq "G"){$CG+=1;}
	if($ref eq "A" && $alt eq "T"){$AT+=1;}
	if($ref eq "A" && $alt eq "G"){$AG+=1;}
	if($ref eq "A" && $alt eq "C"){$AC+=1;}
	if($ref eq "T" && $alt eq "A"){$TA+=1;}
	if($ref eq "T" && $alt eq "G"){$TG+=1;}
	if($ref eq "T" && $alt eq "C"){$TC+=1;}
}
my $transversion=$GT+$GC+$CA+$CG+$AT+$AC+$TA+$TG;
my $transition=$GA+$CT+$AG+$TC;
print "G>A\t$GA\nG>T\t$GT\nG>C\t$GC\n";
print "C>A\t$CA\nC>T\t$CT\nC>G\t$CG\n";
print "A>T\t$AT\nA>G\t$AG\nA>C\t$AC\n";
print "T>A\t$TA\nT>G\t$TG\nT>C\t$TC\n";
print "transversion $transversion\n";
print "transition $transition\n";
close A;
