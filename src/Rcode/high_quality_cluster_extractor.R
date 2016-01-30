setwd("/home/luzhao/barcode_single_end/barcode_bench/data")


# The first element is the entropy table 
# The second element is the cluster table
# The third is the frequency table
# The last one is the ouprefix
dump_high_quality_cluster <- function(files, size, entropy_threshold) {
	
	print(files)
	# Extract those clusters that its largest entropy is less than the 
	# the specified threshold and dump those entropy information to file
	entropy_table = read.table(files[1], sep = ",", header = TRUE, stringsAsFactors = FALSE)
	large_clusters = entropy_table[which(entropy_table$size >= size),]		
	max_entropy = apply(large_clusters[, -c(1,2)], 1, max)
	qualified_clusters_by_entropy = large_clusters[max_entropy <= entropy_threshold,]
	write.table(qualified_clusters_by_entropy, 
		    paste(files[4],"entropy.csv", sep = "_"),
		    sep = ",", row.names = FALSE, col.names = TRUE)		
	
	# Extract the cluster information and frequency information.
	clusters = read.table(files[2], sep = ",", header = TRUE, stringsAsFactors = FALSE)
	attach(clusters)
	qualified_clusters = clusters[which(Cluster.ID %in% qualified_clusters_by_entropy$barcode),]
	write.table(qualified_clusters,
		    paste(files[4], "clusters.csv", sep = "_"),
		    sep = ",", row.names = FALSE, col.names = TRUE)
	detach(clusters)
	
	# Extract the bp frequency information.	
	frequency_table = read.table(files[3], sep = ",", header = TRUE)

	attach(frequency_table)
	
	qualified_frequency_table = frequency_table[which(Cluster.ID %in% qualified_clusters_by_entropy$barcode),]
	detach(frequency_table)
	write.table(qualified_frequency_table,
		    paste(files[4], "frequency.csv", sep = "_"),
		    sep = ",", row.names = FALSE, col.names = TRUE)
}


prefix = "single_end_data"
bp_suffix = "bp_frequency.csv"
frequency_files = paste(prefix, seq(16, 28, 1), bp_suffix, sep = "_")
print(frequency_files)
cluster_suffix = "cluster.csv"
cluster_files = paste(prefix, seq(16, 28, 1), cluster_suffix, sep ="_")
entropy_suffix = "bp_frequency_entropy.csv"

entropy_table_files = paste(prefix, seq(16, 28, 1), entropy_suffix, sep = "_")

outputfiles = paste("high_quality", seq(16, 28, 1), sep = "_")

files = cbind(entropy_table_files, cluster_files, frequency_files, outputfiles)
# identify those high quality 
Sys.time()
apply(files, 1, dump_high_quality_cluster, size = 10,entropy_threshold = 0.2 )
Sys.time()

# Identify large clusters
outputfiles = paste("large_clusters", seq(16, 28, 1), sep = "_")

files = cbind(entropy_table_files, cluster_files, frequency_files, outputfiles)
Sys.time()
#apply(files, 1, dump_high_quality_cluster, size = 10, entropy_threshold = 1)
Sys.time()
