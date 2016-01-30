
setwd("/home/laozzzzz/Documents/barcode_single_end/data/single_end_data/")
source("/home/laozzzzz/Documents/barcode_single_end/Rcode/positionWeight.R")


prefix = "abnormal_cluster"

bp_suffix = "bp_frequency.csv"

frequency_files = paste(prefix, seq(16,28,1), bp_suffix, sep = "_")
abnormal_cluster_files = paste(prefix, "_", seq(16,28,1), ".csv", sep = "")

print(frequency_files)
print(abnormal_cluster_files)

read_cluster <- function(file) {
  cluster = read.table(file, header = TRUE, stringsAsFactors = FALSE)
  return(cluster)
}

cluster = c()
proportion = c()
barcode_length = 16
for( i in 1:length(abnormal_cluster_files)) {
  print(abnormal_cluster_files[i])
  temp = read.table(abnormal_cluster_files[i], header = TRUE, stringsAsFactors = FALSE, sep = ",")
  barcode.length = rep(barcode_length, nrow(temp))
  temp = cbind(temp,barcode.length)
  proportion = c(proportion, nrow(temp[which(temp$Cluster.Size >= 10),])/nrow(temp))
  cluster = rbind(cluster, temp)

  barcode_length = barcode_length + 1
}


colnames(cluster) = c("Cluster.ID", "Cluster.Size", "Centers", "time_point_0", "barcode.length") 
dim(cluster)
attach(cluster)
cluster = cluster[order(Cluster.Size),]
boxplot(cluster$Cluster.Size)
quantile(cluster$Cluster.Size)
large_clusters = cluster[which(Cluster.Size >= 10), ]
print(abnormal_cluster_files[i])
table(cluster$barcode.length)
hist(cluster$barcode.length, main = "Barcode length distribution", xlab = "Barcode length")
detach(cluster)
summary(large_clusters)

# Calculates the entropy
# and combine the frequency table and entropy table among
# all clusters with different lengths.
entropy_table = c()
frequency_table = c()
barcode_length = 16
for( i in 1:length(frequency_files)){
  print(frequency_files[i])
  frequency_temp = read.table(frequency_files[i], header = TRUE, sep = ",", stringsAsFactors = FALSE)
  barcode.length = rep(barcode_length, nrow(frequency_temp))
  
  entropy_temp = pwm_process(frequency_temp[,-1])
  row_size = nrow(frequency_temp)
  
  # Adds extra dummy part to make all cluster have same number of columns.
  if (barcode_length < 28) {
    dummy_part = matrix(0, nrow = row_size, ncol = 28 - barcode_length)
    frequency_temp = cbind(frequency_temp, dummy_part)
    entropy_temp = cbind(entropy_temp, dummy_part[seq(1, row_size, 4),])
  } 
  
  entropy_temp = cbind(frequency_temp[seq(1, row_size, 4),1],
                       entropy_temp,
                       barcode.length[seq(1, row_size, 4)])

  new_colnames = paste("position_", seq(0, 27), sep = "")
  colnames(entropy_temp) = c("Cluster.ID", new_colnames, "barcode.length" )
  
  
  frequency_temp = cbind(frequency_temp, barcode.length)
  colnames(frequency_temp) = c("Cluster.ID", new_colnames, "barcode.length")
  entropy_table = rbind(entropy_table, entropy_temp)
  frequency_table = rbind(frequency_table, frequency_temp)
  barcode_length = barcode_length + 1
}

write.table(entropy_table, file = "whole_entropy_table.csv", sep = ",", row.names = FALSE,
            col.names = TRUE)

write.table(frequency_table, file = "whole_frequency_table.csv", sep = ",", row.names = FALSE,
            col.names = TRUE)

calculation <- function(c, func) {
  return(func(c[2:c[30]]))
}
mean_entropy = apply(entropy_table, 1, calculation, func = mean)
max_entropy = apply(entropy_table, 1, calculation, func = max)
min_entropy = apply(entropy_table, 1, calculation, func = min)
median_enropy = apply(entropy_table, 1, calculation, func = median)
quantile_entropy = apply(entropy_table, 1, calculation, func = quantile)

prob = c(.5, .75,.90,.93, .95,.96,.98,.99, .999)
quantile(median_enropy, probs = prob)
quantile(max_entropy, probs = prob)

entropy_table = cbind(entropy_table, max_entropy)
# Extract large cluster that has low quality

cluster_with_entropy = merge(cluster, entropy_table, by = "Cluster.ID")
threshold = 10
attach(cluster_with_entropy)
large_clusters = cluster_with_entropy[which(Cluster.Size >= threshold & max_entropy > 0.32), ]
detach(cluster_with_entropy)
dim(large_clusters)
large_clusters = large_clusters[order(Cluster.Size,max_entropy),]
