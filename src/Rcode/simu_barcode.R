#!/usr/local/bin/Rscript
setwd("/Users/lukez/Documents/barcode_single_end/simulation_data");
Sys.time()
set.seed(23)
mutA=function(n) sample(c("C","G","T"),n,replace=T);
mutC=function(n) sample(c("A","G","T"),n,replace=T);
mutG=function(n) sample(c("A","C","T"),n,replace=T);
mutT=function(n) sample(c("A","C","G"),n,replace=T);

options(digits=20);


bc.letter = c("A","C","G","T"); 
coded_dna = matrix(c(0,1,2,3), ncol = 4)
colnames(coded_dna) = bc.letter

n.bc = 1e5;     #number of barcodes
r.e = 0.02;     #error rate
len.bc = 20;    #barcode length

###### Generate true barcode (bc) #################
bc.true.mat = matrix(sample(bc.letter, n.bc*len.bc,  replace=T), nc=len.bc);
bc.num.true = 0;
for (j in 1:ncol(bc.true.mat)){
   bc.num.true = bc.num.true + coded_dna[,bc.true.mat[,j]]*4^(j-1);
}

###### Generate true barcode frequencies #################
barcode.freq.lambda = 0.01
bc.freq = round(rexp(n.bc, barcode.freq.lambda));
# hist(bc.freq); 
# sum(bc.freq);

bc.num.true.freq = cbind(bc.num.true, bc.freq);
bc.num.true.freq[,1] = format(bc.num.true.freq[,1], scientific = FALSE)
write.table(bc.num.true.freq, "True_barcode_freq.txt",sep="\t", quote=F, row.names=F, col.names=F);

###### process barcode in batches #################
pos.bc.freq = rep(1:n.bc, bc.freq);
size.batch=1e6;
n.batch=ceiling(length(pos.bc.freq)/1e6);
batch.start= c(0:(n.batch-1))*size.batch+1;
batch.end = c(c(1:(n.batch-1))*size.batch, length(pos.bc.freq));

bc.num=NULL;
total.mutated = 0
for (i.batch in 1:n.batch){
   bc.true.mat.batch = bc.true.mat[pos.bc.freq[batch.start[i.batch]:batch.end[i.batch]],];
   n.base = length(bc.true.mat.batch);
   num.mut = rpois(1, n.base*r.e);
   total.mutated = total.mutated +  num.mut
   pos.mut = runif(num.mut, 1, n.base);
   base.mut.0 = base.mut.1 = bc.true.mat.batch[pos.mut];
   A.index = (base.mut.0=="A"); base.mut.1[A.index]=mutA(sum(A.index));
   C.index = (base.mut.0=="C"); base.mut.1[C.index]=mutC(sum(C.index));
   G.index = (base.mut.0=="G"); base.mut.1[G.index]=mutG(sum(G.index));
   T.index = (base.mut.0=="T"); base.mut.1[T.index]=mutT(sum(T.index));  
   bc.true.mat.batch[pos.mut]=base.mut.1;
   bc.true.mat.batch = matrix(bc.true.mat.batch, nc=len.bc);
   bc.num.i = 0;
   for (j in 1:ncol(bc.true.mat.batch)){
      bc.num.i = bc.num.i + coded_dna[,bc.true.mat.batch[,j]]*4^(j-1);
   }
   bc.num=c(bc.num, bc.num.i);
}
bc.num.unique = table(bc.num);
bc.num.unique.freq = cbind(as.numeric(names(bc.num.unique)), bc.num.unique);
bc.num.unique.freq[,1] <- format(bc.num.unique.freq[,1], scientific = FALSE)
write.table(bc.num.unique.freq, "Simulated_barcode_freq.txt",sep=",", quote=F, row.names=F, col.names=F);
print(total.mutated/(sum(bc.freq)*20))
Sys.time()
