# plot the Venndiagram
setwd("~/Documents/barcode_single_end/Rcode")
library(VennDiagram)
#source("/Users/lukez/Documents/barcode_single_end/Rcode/distance_bitwise_util.R")
truth = read.table("../simulation_data/True_barcode_seq.csv", 
                   header = F, sep = ',', stringsAsFactors = F)

cluster <- read.table("../simulation_data/simulated_data_cluster_cluster.csv",
                             header = TRUE, sep = ',', stringsAsFactors = F)


dim(truth)
dim(cluster)

combined = list()
truth = truth[which(truth$V2 > 2),]
result_cluster = cluster[which(cluster$time_point_1 > 2),]
combined[[1]] = truth$V1
combined[[2]] = result_cluster$Center
names(combined) = c("truth", "estimated")
#pdf("comparison_plot.pdf")
overlap <- calculate.overlap(combined)
plot.new()
draw.pairwise.venn(length(overlap[[1]]), 
                   length(overlap[[2]]),
                   length(overlap[[3]]),
                   scaled = T, label.col = c('green', 'yellow','red'),
                   fill = c('green','red'),
                   main = "Venndiagram ",
                   main.cex = 2)
venn.plot <- venn.diagram(combined,
             "overlap_simulation.tiff",
             scaled = T,
             ext.text = T,
             cex = 2.5,
             fill = c("red","blue"),
             label.col = c("red", "black","green"),
             cat.col = c("red", "green"))

venn.plot <- venn.diagram(
  x = list(
    "A" = 1:100,
    "B" = 96:140
  ),
  filename = "Venn_2set_complex.tiff",
  scaled = TRUE,
  ext.text = TRUE,
  ext.line.lwd = 2,
  ext.dist = -0.15,
  ext.length = 0.9,
  ext.pos = -4,
  inverted = TRUE,
  cex = 2.5,
  cat.cex = 2.5,
  rotation.degree = 45,
  main = "Complex Venn Diagram",
  sub = "Featuring: rotation and external lines",
  main.cex = 2,
  sub.cex = 1
);
false_positive = result_cluster[which(!(result_cluster$Center %in% truth$V1)),]
plot.new()
<<<<<<< HEAD
hist(false_positive$time_point_1, col = 'red', xlab = 'Estimated barcodes size',
     main = "False positive barcodes")
=======
hist(false_positive$time_point_1, col = 'green', xlab = 'Barcode size', 
     main = "")
>>>>>>> 48178798de83fcdfbc4792b4f19b12ea0fdcafa7
dim(false_positive)
summary(false_positive$time_point_1)

missing = truth[which(!(truth$V1 %in% result_cluster$Center)),]
<<<<<<< HEAD
hist(missing$V2, col = 'green', xlab = 'Barcode size',
     main = "Missing barcodes")
=======
hist(missing$V2, col = 'red', xlab = 'Barcode size',
     main = "")
>>>>>>> 48178798de83fcdfbc4792b4f19b12ea0fdcafa7
dim(missing)
summary(missing$V2)


merged = merge(truth, result_cluster, by.x = "V1", by.y = "Center")

plot(merged$V2, merged$time_point_1, pch = 20, col = 'blue',
     xlab = "True frequency", ylab = "Estimated frequency")
abline(0, 1,col = 'red')
dev.off()

par(mfrow = c(1,1))

hist(log10(truth$V2), col = "red",
     xlab = "Barcode size distribution in log10 scale", main =" ",
     breaks = seq(0.4, 3.2, 0.2), xlim = c(0.4,3.2), freq = F)
hist(log10(result_cluster$time_point_1), col = "green", freq = F, 
     xlab = "Barcode size distribution in log10 scale", main = " ",
     breaks = seq(0.4, 3.2, 0.2), xlim = c(0.4,3.2))
plot.new()
draw.pairwise.venn(length(overlap[[1]]), 
                   length(overlap[[2]]),
                   length(overlap[[3]]),
                   scaled = T, label.col = c('green', 'yellow','red'),
                   fill = c('green','red'),
                   main = "Venndiagram ")
plot(merged$V2, merged$time_point_1, pch = 20, col = 'blue',
     xlab = "True frequency", ylab = "Estimated frequency", main = " ")
abline(0, 1,col = 'red')
###########################Find the clusters that swallow all missing barcode ##############

belongs = read.csv("../simulation_data/simulated_data_cluster_barcode.csv", header = T)


missing_info = merge(belongs, missing, by.x = "Barcode.Sequence", by.y = "V1")

names(missing_info)
dim(missing_info)
names(cluster)
missing_barcode_belongs = merge(missing_info, cluster, by = "Cluster.ID")[, c("Barcode.Sequence", "V2", "Center",  "time_point_1", "Cluster.Score", "Cluster.ID")]


names(missing_barcode_belongs) = c("Truth", "True.size", "Estimated_sequence", "Estimated_size", "Cluster.Score", "Cluster.ID")
is_in_truth = ifelse(missing_barcode_belongs$Estimated_sequence %in% truth$V1, "yes", "no")
missing_barcode_belongs = cbind(missing_barcode_belongs,is_in_truth)
missing_barcode_belongs = missing_barcode_belongs[order(-missing_barcode_belongs$Estimated_size),]
write.table(missing_barcode_belongs, "missing_info.csv", sep = ',', quote = F, col.names = T, row.names = F) 
