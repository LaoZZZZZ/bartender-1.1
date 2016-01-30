setwd("/home/luzhao/barcode_single_end/barcode_bench/data")


dump_abnormal_cluster <- function(files) {
	abnormal_barcode = read.table(files[1], sep = ",", header = FALSE)
	clusters = read.table(files[2], sep = ",", header = TRUE, stringsAsFactors = FALSE)
	frequency_table = read.table(files[3], sep = ",", header = TRUE)

	attach(clusters)
	abnormal_cluster = clusters[which(Cluster.ID %in% abnormal_barcode$V1),]
	detach(clusters)

	attach(frequency_table)
	abnormal_frequency_table = frequency_table[which(Cluster.ID %in% abnormal_barcode$V1),]
	detach(frequency_table)

	write.table(abnormal_cluster,file = paste(files[4], ".csv", sep =""), sep = ",", row.names = FALSE,
                    col.names = TRUE)

	write.table(abnormal_frequency_table, file = paste(files[4], "_bp_frequency.csv", sep = ""), sep = ",", row.names = FALSE,
                    col.names = TRUE)
}


#abnormal_cluster_id = "single_end_data_16_bp_frequency_abnormal_cluster_id.csv"
#cluster_file = "single_end_data_16_cluster.csv"
#frequency_file = "single_end_data_16_bp_frequency.csv"
#outprefix = "abnormal_cluster_16"
#dump_abnormal_cluster(abnormal_cluster_id, cluster_file, frequency_file, outprefix)
