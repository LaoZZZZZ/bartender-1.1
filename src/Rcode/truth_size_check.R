# check the truth barcode size dynamics
setwd("/home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation")
truth = list()

timepoint = seq(1,161, by =8)

truth_files = paste("result_", timepoint, "_cluster.csv", sep = '')

truth = lapply(truth_files, read.table, sep = ',', header = T)

library(Matrix)
getNonZero <- function(frequency_table) {
  return(nnzero(frequency_table[,2]))
}


size = unlist(lapply(truth, getNonZero))
plot(timepoint, size)
cluster_result = read.table(paste("result", timepoint, "cluster.csv", sep = '_'), sep = ',', header = T)

truth = read.table(paste("Truth_time_point_", timepoint, ".csv", sep = ""), sep = ",", header = T)