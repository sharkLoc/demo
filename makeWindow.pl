use strict;

open A,"$ARGV[0]"; #chrlen info

my $window=$ARGV[1]; # window length

while(<A>){
	chomp;
	my($chr,$len)=split;
	my $n=1;
	for(my $k=1;$k<=$len;$k+=$window){
		my $tmp=$k+$window-1;
		if($tmp>$len){$tmp=$len;}
		my $out=$chr."\t".$k."\t".$tmp;
		open OU,">>all.bed";
		print OU "$out\n"; 
		$n+=1;
	}
}
