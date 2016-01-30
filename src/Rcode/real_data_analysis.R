# real data analysis

t1 = read.table("/home/laozzzzz/Downloads/G0_52.csv", header = T, sep =',')
t2 = read.table("/home/laozzzzz/Downloads/G0_61.csv", header = T, sep =',')
t3 = read.table("/home/laozzzzz/Downloads/G0_71.csv", header = T, sep =',')
sum (t1$colum1)
sum(t2$colum1)
sum(t3$colum1)


result1 = read.table("/home/laozzzzz/Downloads/G0_52_barcode_Sasha.txt", header = F, sep ='\t')
result2 = read.table("/home/laozzzzz/Downloads/G0_61_barcode_Sasha.txt", header = F, sep ='\t')
result3 = read.table("/home/laozzzzz/Downloads/G0_71_barcode_Sasha.txt", header = F, sep ='\t')
dim(result1)
dim(result2)
dim(result3)
result1 = result1[which(result1$V2 > 5),]
par(mfrow = c(1,3))
hist(log(result1$V2), main = "G0_52", xlab = "Estimated number of reads per barcode", ylab = "Number of unique barcodes")
hist(log(result2$V2), main = "G0_61", xlab = "Estimated number of reads per barcode", ylab = "Number of unique barcodes")
hist(log(result3$V2), main = "G0_71", xlab = "Estimated number of reads per barcode", ylab = "Number of unique barcodes")

summary(result1$V2)
intersection = merge(result1, result2, by= "V1")
intersection = merge(intersection, result3, by = 'V1')
cor(intersection[,-1])
plot(intersection$V2.x, intersection$V2.y)
summary(intersection[,-1])
nrow(intersection)/nrow(result1)
nrow(intersection)/nrow(result2)
nrow(intersection)/nrow(result3)
