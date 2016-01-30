
setwd("/home/luzhao/barcode_single_end/barcode_bench/data/")


prefix = "single_end_data"

suffix = "cluster.csv"

cluster_files = paste(prefix, seq(16,28,1), suffix, sep = "_")

print(cluster_files)


read_cluster <- function(file) {
  cluster = read.table(file, header = TRUE, stringsAsFactors = FALSE)
  return(cluster)
}

cluster = c()
proportion = c()
barcode_length = 16
for( i in 1:length(cluster_files)) {
  temp = read.table(cluster_files[i], header = TRUE, stringsAsFactors = FALSE, sep = ",")
  barcode.length = rep(barcode_length, nrow(temp))
  temp = cbind(temp,barcode.length)
  proportion = c(proportion, nrow(temp[which(temp$Cluster.Size >= 5),])/nrow(temp))
  cluster = rbind(cluster, temp)

  barcode_length = barcode_length + 1
}

png("/home/luzhao/barcode_single_end/barcode_single_end/data/cluster_size_distribution.png")
colnames(cluster) = c("Cluster.ID", "Cluster.Size", "Centers", "time_point_0", "barcode.length") 
dim(cluster)
attach(cluster)
prob = c(0.25, 0.5, 0.75, 0.9, 0.95, 0.99, .999, 1)
print(quantile(cluster$Cluster.Size, probs = prob))
#large_clusters = cluster[which(Cluster.Size >= 5), ]
#print(table(large_clusters$barcode.length)/sum(table(large_clusters$barcode.length)))
hist(log(cluster$Cluster.Size), main = "Cluster Size distribution")

detach(cluster)
dev.off()

