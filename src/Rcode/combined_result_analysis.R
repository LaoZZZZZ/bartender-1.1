# analyze the combined clusters
setwd("/home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation")
source("/home/laozzzzz/Documents/barcode_single_end/Rcode/positionWeight.R")
cluster_result = read.table("combine_result_cluster.csv", sep = ',', header = T)
frequency_table = read.table("combine_result_bp_frequency.csv", sep = ',', header = T)

reverse_order = paste("time_point_",rev(0:20), sep = "")

# reverse the time point 
cluster_result = cluster_result[, c(1:4, rev(5:25))]
isBad <- function(row) {
  found = F
  for (i in 5:25) {
    if (found && row[i] != 0) {
      return(T)
    } else if (!found && row[i] == 0) {
      found = T
    }
  } 
  return(F)
}

bad_clusters = apply(cluster_result, 1, isBad)

true_positive = cluster_result[!bad_clusters,]
max_freq = apply(true_positive[,5:25], 1, max)
true_positive = cbind(true_positive, max_freq)
true_positive = true_positive[order(-true_positive$max_freq),]

intersection = true_positive[which(true_positive$Centers %in% truth$barcode),]
# combine the true barcode in different time points
timepoint = seq(1,161, by =8)
files = paste("Truth_time_point_", timepoint, ".csv", sep = "")
truth = read.table(files[1], sep =',', header = T)
for( f in files[-1]) {
  temp = read.table(f, sep = ',', header = T)
  truth = cbind(truth, temp[,2])
}
colnames(truth) = c("barcode", timepoint)

true_positive = cluster_result[which(cluster_result$Centers %in% truth$barcode),]

duplicate_barcode = true_positive[duplicated(true_positive$Centers),]
unique_true_positive = true_positive[!duplicated(true_positive$Centers),]
attach(true_positive)
for (c in unique(duplicate_barcode$Centers)) {
  dup = true_positive[which(Centers == c), ]
  updated = unlist(c(dup[1,1:4], apply(dup[,-c(1,2,3,4)], 2, sum)))
  unique_true_positive[which(unique_true_positive$Centers == c), ] = updated
}
detach(true_positive)
unique_true_positive = unique_true_positive[,c(1:4, seq(25, 5, by = -1))]

unique_true_positive = unique_true_positive[order(unique_true_positive$Centers),]
truth = truth[order(truth$barcode),]
pdf("trajectoty.pdf")
matplot(timepoint,
        t(unique_true_positive[, 5:25]), 
        type = 'l', main = "Trajectory",
        xlab = "Time points",
        ylab = "Frequency")

dev.off()
pdf("trajectory_log.pdf")
timepoint = seq(1,161, by =8)

matplot(timepoint,
        t(log10(unique_true_positive[, 5:25])), 
        type = 'l', main = "Trajectory",
        xlab = "Time points",
        ylab = "Frequency in log10 scale")
dev.off();
head(unique_true_positive)

unique_true_positive = unique_true_positive[order(unique_true_positive$Centers),]

truth = truth[which(truth$barcode %in% unique_true_positive$Centers),]
truth = truth[order(truth$barcode),]

intersection = merge(unique_true_positive, truth, by.x = "Centers", by.y = "barcode")
correlationCal <- function(row) {
  x = as.numeric(row[5:25])
  y = as.numeric(row[26:46])
  return(cor(x,y, method= "pearson"))
}
correlations = apply(intersection, 1, correlationCal)
sorted_cor = sort(correlations)
abnormal_clusters = intersection[correlations < 0.9,]
x = abnormal_clusters[, 5:25]
y = abnormal_clusters[, 26:46]
matplot(t(y), t(x), type = 'l')
plot(as.numeric(x[1,]), as.numeric(y[1,]), xlab = "Estimated", ylab = "Truth")
plot(as.numeric(x[2,]), as.numeric(y[2,]), xlab = "Estimated", ylab = "Truth")
plot(as.numeric(x[7,]), as.numeric(y[7,]), xlab = "Estimated", ylab = "Truth")

abnormal_clusters
