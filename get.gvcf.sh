gatk=/software/biosoft/gatk-4.0.3.0/gatk
gvcf=/work2019/meat/04.gatk/gvcf
outdir=`pwd`
ref=/work2019/ref/pigRef

for i in `seq 1 18` X Y
do
	mkdir -p $outdir/chr$i
	
	for j in `ls $gvcf/*/*/chr$i.g.vcf.gz`
	do
		echo "$j" >> $outdir/chr$i/chr${i}.gvcf.list
	done
	
	echo "date
		$gatk --java-options \"-Xmx20G -Djava.io.tmpdir=$outdir/chr$i/tmp1\" CombineGVCFs -R $ref/chr${i}.fa -V $outdir/chr$i/chr${i}.gvcf.list -O $outdir/chr$i/chr${i}.combine.vcf.gz
		date
		echo \"gatk CombineGVCFs Done\"
		qstat -j \$JOB_ID|grep usage ">$outdir/chr$i/chr${i}.combine.sh

	echo "date
		$gatk --java-options \"-Xmx20G -Djava.io.tmpdir=$outdir/chr$i/tmp2\" GenotypeGVCFs -R $ref/chr${i}.fa -V $outdir/chr$i/chr${i}.combine.vcf.gz -O $outdir/chr$i/chr${i}.raw.vcf.gz
		echo \"gatk GenotypeGVCFs Done\"
		date
		qstat -j \$JOB_ID|grep usage" > $outdir/chr$i/chr${i}.vcf.sh

	echo "date
		$gatk --java-options \"-Xmx12G\" SelectVariants -R $ref/chr${i}.fa -V $outdir/chr$i/chr${i}.raw.vcf.gz -select-type SNP  -O $outdir/chr$i/chr${i}.raw.vcf.SNP.gz
		echo \"gatk SelectVariants:snp Done\"
	
		snpFilt=\"QD < 2.0 || ReadPosRankSum < -8.0  || FS > 60.0 ||  MQ < 40.0 || SOR > 3.0 || MQRankSum < -12.5 || QUAL < 30\"
		$gatk VariantFiltration -R $ref/chr${i}.fa -V $outdir/chr$i/chr${i}.raw.vcf.SNP.gz -filter \"\$snpFilt\"  --filter-name \"Filter\" -O $outdir/chr$i/chr${i}.raw.vcf.SNP.mark.gz
		echo \"gatk mark Done\"
		date
		qstat -j \$JOB_ID|grep usage" >$outdir/chr$i/chr${i}.select.mark.sh

	echo "cd $outdir/chr$i/ && qsub -cwd -l vf=10G,p=4 -P s2pnh chr${i}.combine.sh" >> $outdir/qsub.combine.sh
	echo "cd $outdir/chr$i/ && qsub -cwd -l vf=8G,p=1 -P s2pnh chr${i}.vcf.sh" >> $outdir/qsub.vcf.sh
	echo "cd $outdir/chr$i/ && qsub -cwd -l vf=2G,p=1 -P s2pnh chr${i}.select.mark.sh">>$outdir/qsub.select.mark.sh
done
