#!/usr/bin/Rscript

truth = read.csv("../simulation_data/True_barcode_seq.csv", header = F)

simulated = read.csv("../simulation_data/Simulated_barcode_seq.csv", header = F)


mutated_barcode = truth[which(!(truth$V1 %in% simulated$V1)),]

dim(mutated_barcode)
merged = merge(truth, simulated, by = "V1")

merged[which(merged$V2.x < 14),]
mutated_barcode[which(mutated_barcode$V2 > 3),]
