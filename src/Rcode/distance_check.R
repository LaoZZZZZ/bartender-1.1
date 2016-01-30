library(stringdist)
setwd("/home/laozzzzz/Documents/barcode_single_end/simulation_data")
truth = read.table("True_barcode_freq_seq.txt", header = F, sep = '\t', stringsAsFactors = F)

cluster_result = read.table("cluster_result_seq.txt", header = F, sep = '\t', stringsAsFactors = F)

intersection = merge(truth, cluster_result, by = "V1")

missing = truth[which(!(truth$V1 %in% intersection$V1)),]
false_positive = cluster_result[which(!(cluster_result$V1 %in% intersection$V1)),]
large_missing_barcode = missing[which(missing$V2 > 10),]
which.max(large_missing_barcode[, 2])
distmatrix = stringdistmatrix(large_missing_barcode$V1[which.max(large_missing_barcode[,2])], intersection$V1)
which.min(distmatrix)
intersection[which.min(distmatrix),]
large_missing_barcode[which.max(large_missing_barcode[,2]),]

false_distmatrix = stringdistmatrix(large_missing_barcode$V1[which.max(large_missing_barcode[,2])], false_positive$V1)
false_positive[which(false_distmatrix == min(false_distmatrix)),]
