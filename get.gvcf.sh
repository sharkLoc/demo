ref=/genomes/goat/ref
vcftools=/Software/Soft_ALL/vcftools-0.1.13/bin/vcftools
gatk=/Software/Soft_ALL/gatk-4.1.1.0/gatk
out=`pwd`

for ind in `ls -d $bam/*/*bam`
do
    sp=`echo $ind|awk -F / '{print $(NF-1)}'`
    mkdir -p $out/$sp
    for i in `seq 1 29`
    do	
		mkdir -p $out/$sp/chr$i
		for  j in `seq 1 4`
		do
	    	if [ -e $ref/chr${i}_${j}.intervals ];then
			echo "date
$gatk --java-options -Xmx4G HaplotypeCaller -R $ref/goat.fa -I $ind -L $ref/chr${i}_${j}.intervals -O $out/$sp/chr$i/chr${i}_${j}.g.vcf.gz --emit-ref-confidence GVCF
				date
				qstat -j \$JOB_ID|grep usage ">$out/$sp/chr$i/${sp}.chr${i}_${j}.sh

		echo "cd $out/$sp/chr$i && qsub -cwd -l vf=4G,p=1 -P s2pnh -q s2pnh.q -binding linear:1 ${sp}.chr${i}_${j}.sh">>$out/$sp/qsub.${sp}.sh
	    fi 
   		done
    done
    echo "cd $out/$sp && sh qsub.${sp}.sh " >>$out/qsub.each.sh	
done
