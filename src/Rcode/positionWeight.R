# Analyzes the cluster frequency table.
setwd("/home/luzhao/barcode_single_end/barcode_bench/data/")
#source("http://bioconductor.org/biocLite.R")
#biocLite("seqLogo")
#library(Biostrings)
library(grid)
library(seqLogo)
library(entropy)
#define function that divides the frequency by the row sum i.e. proportions
proportion <- function(x){
  rs <- sum(x);
  return(x / rs);
}

pwm_process <- function(frequencyTable) {
  
  row_size = nrow(frequencyTable)
  #row_size = 3
   
  ft_4 = matrix(unlist(frequencyTable), nrow=4)
  ft_4 = t(ft_4)
  
  ft_4_sum = rowSums(ft_4)
  max_bp = apply(ft_4, 1, max)
  ft_2_p = cbind(max_bp, ft_4_sum - max_bp)
  ft_2_p = ft_2_p/ft_4_sum
  ft_2_p_log = log(ft_2_p)
  ft_2_p_log[ft_2_p_log==(-Inf)] = 0
  ft2_en_mat = ft_2_p*ft_2_p_log
  ft2_en = rowSums(ft2_en_mat)
  
  
#  ft_4_en = apply(ft_4, 2, entropy)
  ft_en = matrix(ft2_en, nrow=row_size/4)
   
#   col_size = ncol(frequencyTable)
#   batch = 1
#   count = 1
#   result = matrix("", nrow = row_size/4, ncol = col_size - 1)
#   while(batch < row_size) {
#     result[count,] = apply(frequencyTable[batch:(batch+3), -1], 2, entropy)
#     count = count + 1 
#     batch = batch + 4
#   }
  return(-ft_en)
}


entropy_process <- function(freqyency_file, outfile) {
	
	frequency_table = read.table(freqyency_file, header = TRUE, sep = ",", stringsAsFactors=FALSE)

	entropy_result = pwm_process(frequency_table)
	# The first column is the barcode itself
	# The remaining column is the position
	# each row is the corresponding entropy for that position
	cluster_size = colSums(matrix(unlist(frequency_table[,2]), nrow=4))

	entropy_result = cbind(frequency_table[seq(1, nrow(frequency_table), 4),1], cluster_size, entropy_result)

	# Set up the column name
	n_col = ncol(entropy_result) - 2;
	positions = paste("pos_", seq(1, n_col, 1))
	colnames(entropy_result) = c("barcode", "size",positions )
	write.table(entropy_result, outfile, sep = ",", row.name = FALSE, col.name = TRUE)

}

