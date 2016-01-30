setwd("/home/luzhao/barcode_single_end/barcode_single_end/Rcode")

clusters = read.table("../data/clusters.csv", sep = ",", stringsAsFactors = FALSE)
colnames(clusters) = c("barcode", "freq")
entropy_matrix = read.table("../data/entropy_table.csv", sep = ",", stringsAsFactors = FALSE, header = TRUE)
maximum_entropy = apply(entropy_matrix[,-1], 1, max)
col_names = colnames(entropy_matrix)
entropy_matrix = cbind(entropy_matrix, maximum_entropy)
colnames(entropy_matrix) = c(col_names, "max_entropy")
compiled_cluster = merge(clusters, entropy_matrix, by = "barcode")

attach(compiled_cluster)
ordered_cluster = compiled_cluster[order(-freq, -max_entropy),]

write.table(ordered_cluster, "../data/complete_cluster_info.csv", col.name = TRUE, row.name = FALSE, sep = ",")

 
