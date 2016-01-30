<<<<<<< HEAD
#barcode with 20 bp

setwd("/home/laozzzzz/Documents/barcode_single_end/data")
files = c("large_clusters_20_entropy.csv",
          "large_clusters_20_clusters.csv",
          "large_clusters_20_frequency.csv")
entropy_table = read.table(files[1], sep = ",", header = TRUE, stringsAsFactors = FALSE)
clusters = read.table(files[2], sep = ",", header = TRUE, stringsAsFactors = FALSE)
frequency_table = read.table(files[3], sep = ",", header = TRUE)
dim(clusters)
attach(clusters)
hist(log10(Cluster.Size), main = "Cluster size distribution", xlab = "Cluster size in log10")
plot(density(log10(Cluster.Size)), main = "Density curve of the cluster size")
detach(clusters)

max_entropy = apply(entropy_table[, -c(1,2)], 1, max)
boxplot(max_entropy)

entropy_table = cbind(entropy_table, max_entropy)
good_clusters = entropy_table[which(entropy_table$max_entropy < 0.06),]
summary(good_clusters$size)

large_clusters = clusters[which(clusters$Cluster.Size > 76640),]
large_cluster_frequency = frequency_table[which(frequency_table$Cluster.ID %in% large_clusters$Cluster.ID),]


library(seqLogo)
plotSeqLogo <- function(single_frequency) {
  pwm <- apply(single_frequency, 2, proportion)
  pwm <- makePWM(pwm) 
  seqLogo(pwm, ic.scale = FALSE)
}
plotSeqLogo(large_cluster_frequency[1:4,-1])
plotSeqLogo(large_cluster_frequency[5:8,-1])

# Extract the bad cluster
bad_clusters = entropy_table[which(entropy_table$max_entropy >= 0.32),]
bad_clusters = bad_clusters[order(-bad_clusters$size),]

worst_clusters = bad_clusters[which(bad_clusters$max_entropy >= 0.5),]


extreme_large_clusters = good_clusters[which(good_clusters$size >= 100000),]
extreme_large_clusters = extreme_large_clusters[order(-extreme_large_clusters$size),]
extreme_large_clusters_freq = frequency_table[which(frequency_table$Cluster.ID %in% extreme_large_clusters$barcode),]

max_proportion = apply(extreme_large_clusters_freq[5:8,-1], 2, max)
sum_proportion = apply(extreme_large_clusters_freq[5:8,-1], 2, sum)
1 - sum(max_proportion)/sum(sum_proportion)

# Calculate the pool error rate
ft_4 = matrix(unlist(extreme_large_clusters_freq[,-1]), nrow=4)
max_bp = apply(ft_4, 2, max)
 
1 - sum(max_bp)/sum(apply(ft_4, 2, sum))
=======
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



cluster_file = "single_end_data_20_cluster.csv"
frequency_file = "single_end_data_20_bp_frequency.csv"
entropy_file = "single_end_data_20_bp_frequency_entropy.csv"



cluster = read.table(cluster_file, header = TRUE, sep = ",")
print(dim(cluster))

large_cluster = cluster[which(Cluster.Size >=5),]
frequency_table = read.table(frequency_file, header = TRUE, sep = ",")
print(dim(frequency_table))
attach(frequency_table)
large_frequency_table = frequency_table[which(Cluster.ID %in% large_cluster$Cluster.ID),]
detach(frequency_table)

entropy_table = read.table(entropy_file, header = TRUE, sep = ",")
print(dim(entropy_table))
>>>>>>> a58574e2cf0ae9eb59cdf42d4a9d12e88e71c1f3

