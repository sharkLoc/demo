dir=/GATK/1.gvcf
out=`pwd`
ref=/genomes/goat/ref
vcftools=/Software/Soft_ALL/vcftools-0.1.13/bin/vcftools
gatk=/Software/Soft_ALL/gatk-4.1.1.0/gatk
snpfilter="QD < 2.0 || ReadPosRankSum < -8.0  || FS > 60.0 ||  MQ < 40.0 || SOR > 3.0 || MQRankSum < -12.5 || QUAL < 30"

for chr in `seq 1 29`
do
    mkdir -p $out/chr$chr
    #######################################  region   gvcf 2 vcf file ###########################################
for i in `ls -d $dir/*`
do
    if [ -d $i ];then	
	for j in `ls -d $i/*`
	do
	    if [ -d $j ];then
		for n in `seq 1 4`
		do
		    if [ -e $j/chr${chr}_${n}.g.vcf.gz ];then
			ls $j/chr${chr}_${n}.g.vcf.gz >>$out/chr$chr/chr${chr}_${n}.gvcf.list
		    fi
		    if [ -e $j/chr${chr}_${n}.g.vcf.gz ];then
			mkdir -p $out/chr$chr/tmp/chr${chr}_${n}
			echo "date
$gatk --java-options \"-Xmx4G -Djava.io.tmpdir=$out/chr$chr/tmp/chr${chr}_${n}\" CombineGVCFs -R $ref/goat.fa -V $out/chr$chr/chr${chr}_${n}.gvcf.list -O $out/chr$chr/chr${chr}_${n}.combine.g.vcf.
gz 
date
$gatk --java-options \"-Xmx4G -Djava.io.tmpdir=$out/chr$chr/tmp/chr${chr}_${n}\" GenotypeGVCFs -R $ref/goat.fa -V $out/chr$chr/chr${chr}_${n}.combine.g.vcf.gz -O $out/chr$chr/chr${chr}_${n}.raw.vc
f.gz
date
qstat -j \$JOB_ID|grep usage">$out/chr$chr/chr${chr}_${n}.vcf.sh

		    fi
		done
	    fi
	done	
    fi
done
    ###############################################  merge raw vcf ################################################
    for reg in `seq 1 5`
    do
	if [ -e $out/chr$chr/chr${chr}_${reg}.vcf.sh ];then
	    echo "$out/chr$chr/chr${chr}_${reg}.raw.vcf.gz" >> $out/chr$chr/chr${chr}.vcf.list
	fi	
    done
    echo "date
$gatk  --java-options \"-Xmx4G -Djava.io.tmpdir=$out/chr$chr/tmp\" MergeVcfs -R $ref/goat.fa -I $out/chr$chr/chr${chr}.vcf.list -O chr${chr}.GenotypeGVCFs.vcf.gz 
date
qstat -j \$JOB_ID|grep usage">$out/chr$chr/chr${chr}.vcf.merge.sh
    echo "cd $out/chr$chr && qsub -cwd -l vf=4G,p=1 -P s2pnh chr${chr}.vcf.merge.sh">>$out/qsub.vcfMerge.sh
    ###############           select snp and mark  #############################################################
    echo "date
$gatk --java-options \"-Xmx4G\"  SelectVariants -R $ref/goat.fa -V $out/chr$chr/chr${chr}.GenotypeGVCFs.vcf.gz -select-type SNP -O $out/chr$chr/chr${chr}.SNP.gz
echo \" select snp done !\"
$gatk VariantFiltration -R $ref/goat.fa -V $out/chr$chr/chr${chr}.SNP.gz -filter \"$snpfilter\" --filter-name \"Filter\" -O $out/chr$chr/chr${chr}.SNP.mark.gz
echo \"mark snp done\"
$vcftools --gzvcf $out/chr$chr/chr${chr}.SNP.mark.gz --remove-indels --recode --recode-INFO-all --remove-filtered-all --stdout |gzip >$out/chr$chr/chr${chr}.SNP.clean.gz
date
qstat -j \$JOB_ID|grep usage" >$out/chr$chr/chr${chr}.select.mark.sh
    echo "cd $out/chr$chr  && qsub -cwd -l vf=4G,p=1 -P s2pnh chr${chr}.select.mark.sh">>$out/qsub.snp.select.mark.sh 

    ###################################### merge fina clean vcf  ################################################
done
################################### qsub gvcf 2 vcf #############################################################
for sh in `ls $out/*/*vcf.sh`
do
	shell=`echo $sh|awk -F / '{print $NF}'`
	chr=`echo $sh|awk -F / '{print $(NF-1)}'`
	echo "cd $out/$chr && qsub -cwd -l vf=4G,p=1 -P s2pnh $shell">>$out/qsub.gvcf2vcf.sh
done
