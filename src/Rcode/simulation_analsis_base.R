# analyze the performance of single time point.
setwd("/home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation")
source("/home/laozzzzz/Documents/barcode_single_end/Rcode/positionWeight.R")
base_analysis <- function(timepoint) {
  
  cluster_result = read.table(paste("result", timepoint, "cluster.csv", sep = '_'), sep = ',', header = T)
 
  truth = read.table(paste("Truth_time_point_", timepoint, ".csv", sep = ""), sep = ",", header = T)
  colnames(truth) = c("Centers", "freq")
  colnames(cluster_result) = c("Cluster.ID", "Cluster.Size", "Centers", "barcode_length", "freq")
  intersection = merge(cluster_result, truth, by = "Centers")
  
  
  color = abs(intersection$freq.x - intersection$freq.y) > 10
  plot(intersection$freq.x, intersection$freq.y, pch = 16, col = ifelse(color, 'red', 'blue'),
     main = paste("Time point ", timepoint, sep = ""), xlab = "Frequency of clustering result",
     ylab = "True frequency")
  accuracy = nrow(intersection)/nrow(truth)
  false_positive = 1 - nrow(intersection)/nrow(cluster_result)
  correlation= cor(intersection$freq.x, intersection$freq.y)
  return (c(nrow(cluster_result),accuracy, false_positive, correlation))
}

errorEstimator <- function(frequency_table, entropy_threshold) {
  entropy_table = pwm_process(frequency_table[,-1])

  max_entropy = apply(entropy_table, 1, max)
  candidates = max_entropy < entropy_threshold
  index = rep(candidates, rep(4, length(candidates)))
  high_clusters_freq = frequency_table[index,]
  ft_4 = matrix(unlist(high_clusters_freq[,-1]), nrow=4)
  max_bp = apply(ft_4, 2, max)
  return(1 - sum(max_bp)/sum(apply(ft_4, 2, sum)))
  
}
pdf("single_point_scatte_plot.pdf")
timepoint = seq(1,161, by =8)
accuracy_result = c()
for (t in timepoint) {
  accuracy = base_analysis(t)
  accuracy_result = rbind(accuracy_result, accuracy)
}
dev.off()
timepoint = 161
frequency_table = read.table(paste("result", timepoint, "bp_frequency.csv", sep = '_'), sep = ',', header = T)

entropy_table = pwm_process(frequency_table[,-1])

max_entropy = apply(entropy_table, 1, max)
index = which.max(max_entropy)
plotSeqLogo(frequency_table[37:40,])
print(errorEstimator(frequency_table, 1))

