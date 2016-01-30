# Multiple time point barcode simulation.

setwd("/home/laozzzzz/Documents/barcode_single_end/simulation_data");
source("/home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation/util.R")
Sys.time()


set.seed(23)
# Some parameters that are used for this simulation.
# Most of these parameters are consistent with that paper.

n.bc = 1e2;     #number of barcodes in the initial generation.
number_reads =  1e2# number of reads at each time point
generations_between_tp = 8 # Generations between two time points

total_generations = 160  # The total number of generations in this evolution process.
len.bc = 20;    #barcode length

r.e = 0.02;     #error rate
percent_of_growth_barcode = 0.05 # percentage of barcodes that have growth advantage
lambda_fitness = 120  # About 5% percent of barcode will have 
fitness_coeff_range = c(0, 0.1) # range of the fitness coefficients.

# Generate fittness coefficient for each barcode
fitness_coeff = rep(0.0, n.bc)
fittness_advantage_barcodes = sample(1:n.bc, percent_of_growth_barcode * n.bc, replace = FALSE)
library(truncdist)
fitness_coeff[fittness_advantage_barcodes] = rtrunc(length(fittness_advantage_barcodes), 
                                                    spec = "exp",
                                                    a = fitness_coeff_range[1],
                                                    b = fitness_coeff_range[2],
                                                    rate = lambda_fitness )
###### Generate true barcode (bc) #################
# At the very initial time point
# Generates n.bc number of barcodes
# and each barcode has frequency 100 
bc.true.mat = generateUniquebpMatrix(n.bc, len.bc, bc.letter)
freq = rep(10, nrow(bc.true.mat))
bc.true.mat.freq = cbind(bc.true.mat, freq, fitness_coeff)

for(i in 0:total_generations) {
  print(i)
  freq <- ceiling(EvolveBarcode(as.numeric(bc.true.mat.freq[,"freq"]),
                                as.numeric(bc.true.mat.freq[, "fitness_coeff"])))
  # If this is the time point for sequencing.
  if (i %% generations_between_tp == 0) {
    normalized_frequency <- normalizeFrequency(number_reads, freq)
    bc.true.mat.freq[, "freq"] = normalized_frequency
    sampled_bc = sampleBarcode(bc.true.mat.freq[, 2:len.bc + 2])
    sequenced_barcode <- sequenceBarcode2(r.e,sampled_bc)
    bc.true.mat.freq[,"barcode"] <- format(bc.true.mat.freq[,"barcode"], scientific = FALSE)
    
    write.table(bc.true.mat.freq[, c("barcode", "freq")], 
                paste("Truth_time_point_", i + 1, ".csv", sep =""), 
                quote = FALSE, sep = ",",
                col.names = TRUE, row.names = FALSE)
    write.table(sequenced_barcode, 
                paste("Sequenced_time_point_", i + 1, ".csv", sep = ""),
                quote = FALSE, sep = ",",
                col.names = FALSE, row.names = FALSE)
  } else {
    bc.true.mat.freq[,"freq"] <- freq
  }
  
}
