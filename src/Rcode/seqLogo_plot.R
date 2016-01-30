
#setwd("/home/luzhao/barcode_single_end/barcode_bench/data")
#source("/home/luzhao/barcode_single_end/barcode_single_end/Rcode/positionWeight.R")
#source("/home/luzhao/barcode_single_end/barcode_single_end/Rcode/low_quality_cluster_extractor.R")

library(seqLogo)
plotSeqLogo <- function(single_frequency) {
  pwm <- apply(single_frequency, 2, proportion)
  pwm <- makePWM(pwm) 
  seqLogo(pwm)
}

proportion <- function(x){
  rs <- sum(x);
  return(x / rs);
}

getSecondLargest <- function(x) {
  n <- length(x)
  return(sort(x, partial = n-1)[n-1])
}

findAbnormalCluster <- function(frequency_file) {
	if(!file.exists(frequency_file)) {
		return
	}
	prefix = strsplit(frequency_file, split = '[.]')[[1]][1]
	output = paste(prefix, "_entropy.csv", sep = "")
        print(c(prefix, output))
	entropy_process(frequency_file, output)
	
	# Load the entropy result got the previous step
	clusters = read.table(output, header = TRUE, sep = ",", stringsAsFactors = FALSE)

	frequency_table = read.table(frequency_file, header = TRUE, sep = ",", stringsAsFactors=FALSE)
	col_size = ncol(clusters)
	probs = c(0, 0.1, 0.5)
	
	# Identify those low quality clusters based on the cluster size and the entropy information.
	attach(clusters)
	
       	size_quantile = quantile(size, prob = probs) 
	max_entropy = apply(clusters[,3:col_size], 1, max)
	clusters = cbind(clusters, max_entropy)

	low_quality_cluster = clusters[which(clusters$size > size_quantile[2] & max_entropy > 0.5),] 

	if (nrow(low_quality_cluster) > 0) { 

		pdf(paste(prefix, ".pdf", sep = ""))
		barcode = c()
		for(i in 1:nrow(low_quality_cluster)) {
			barcode = c(barcode, low_quality_cluster$barcode[i])
			plotSeqLogo(frequency_table[which(frequency_table$Cluster.ID == low_quality_cluster$barcode[i]),])
  
		}
		dev.off()
		write.table(barcode, paste(prefix, "_abnormal_cluster_id.csv", sep = ""), sep = ',', row.names = FALSE, col.name = FALSE)
	}
}

prefix = "single_end_data"
bp_suffix = "bp_frequency.csv"
frequency_files = paste(prefix, seq(16, 16, 1), bp_suffix, sep = "_")
print(frequency_files)
cluster_suffix = "cluster.csv"
cluster_files = paste(prefix, seq(16, 16, 1), cluster_suffix, sep ="_")
abnormal_suffix = "bp_frequency_abnormal_cluster_id.csv"

abnormal_cluster_id_files = paste(prefix, seq(16, 16, 1), abnormal_suffix, sep = "_")

outputfiles = paste("abnormal_cluster", seq(16, 16, 1), sep = "_")

files = cbind(abnormal_cluster_id_files, cluster_files, frequency_files, outputfiles)
Sys.time()
apply(files, 1, dump_abnormal_cluster)
Sys.time()
