# Simulation utility functions

mutationCandidates = rbind(c(1,2,3), c(0,2,3), c(0,1,3), c(0,1,2))

mutA=function(n) sample(c(1,2,3), n ,replace = T);
mutC=function(n) sample(c(0,2,3), n ,replace = T);
mutG=function(n) sample(c(0,1,3), n ,replace = T);
mutT=function(n) sample(c(0,1,2), n ,replace = T);

# Generate the base pair matrix.
# Each row represents a barcoede.
# This function guaruantees that each row of output is unique. 
# The first column is the coded bardode.
# The remaining columns are base pair at each position.
generateUniquebpMatrix <- function(n.bc, len.bc, letters) {
  difference = n.bc
  bc.bp.mat = c()
  bc.mat = c()
  while(TRUE) {
    # Generate next batch of base pair matrix
    temp.bc.bp.mat = matrix(sample(bc.letter, difference*len.bc,  replace=T), nc=len.bc);
    # Transfer the base pair matrix into integer
    # for finding new barcodes that do not appear in the final result.
    temp.bc.mat = encodeDNAMatrix(temp.bc.bp.mat)
    temp.bc.mat = temp.bc.mat[!(temp.bc.mat %in% bc.mat)]
    unique.temp.bc.mat = temp.bc.bp.mat[!duplicated(temp.bc.mat),]
    bc.bp.mat = rbind(bc.bp.mat, unique.temp.bc.mat)
    bc.mat = c(bc.mat, temp.bc.mat)
    difference = n.bc - nrow(bc.bp.mat)
    if (difference == 0) {
      break
    }
  }
  data = cbind(bc.mat, bc.bp.mat )
  colnames(data) = c("barcode", paste("position", seq(1:len.bc), sep = "_"))
  return(data)
}


# Given a base pair matrix
# Convert this matrix to coded integer row-wisely(each row represents a barcode).
encodeDNA <- function(bp.vector) {
  bc.num.true = 0;
  for (j in 1:length(bp.vector)){
    bc.num.true = bc.num.true + bp.vector[j]*4^(j-1);
  }
  return(bc.num.true)  
}

encodeDNAMatrix <- function(bc.bp.mat) {
  bc.num.true = 0;
  for (j in 1:ncol(bc.bp.mat)){
    bc.num.true = bc.num.true + bc.bp.mat[,j]*4^(j-1);
  }
  return(bc.num.true) 
}
# This is another way to sequence the barcode without 
# calling any sub functions
sequenceBarcode2 <- function(r.e, bc.true.mat) {
  n.bc = nrow(bc.true.mat)
  col_size = ncol(bc.true.mat)
  bc.freq = bc.true.mat[, col_size]
  bc.true.mat = bc.true.mat[, -col_size]
  pos.bc.freq = rep(1:n.bc, bc.freq);
  size.batch= n.bc;
  n.batch=ceiling(length(pos.bc.freq)/n.bc);
  batch.start= c(0:(n.batch-1))*size.batch+1;
  batch.end = c(c(1:(n.batch-1))*size.batch, length(pos.bc.freq));
  
  bc.num=NULL;
  for (i.batch in 1:n.batch){
    bc.true.mat.batch = bc.true.mat[pos.bc.freq[batch.start[i.batch]:batch.end[i.batch]],];
    
    n.base = length(bc.true.mat.batch);
    num.mut = rpois(1, n.base*r.e);
    if (length(num.mut) == 0) {
      next
    }
    pos.mut = floor(runif(num.mut, 1, n.base));
    base.mut.0 = base.mut.1 = bc.true.mat.batch[pos.mut];
    A.index = (base.mut.0==0);
    if (sum(A.index) > 0) {
      base.mut.1[A.index]=mutA(sum(A.index));
    }
    C.index = (base.mut.0==1);
    if(sum(C.index) > 0) {
      base.mut.1[C.index]=mutC(sum(C.index));
    } 
    G.index = (base.mut.0==2);
    if (sum(G.index) > 0) {
      base.mut.1[G.index]=mutG(sum(G.index));
    }
    T.index = (base.mut.0==3);
    if(sum(T.index) > 0) {
      base.mut.1[T.index]=mutT(sum(T.index));
    }   
    bc.true.mat.batch[pos.mut]=base.mut.1;
    bc.true.mat.batch = matrix(bc.true.mat.batch, nc=len.bc);
    
    bc.num.i = 0;
    bc.num.i = encodeDNAMatrix(bc.true.mat.batch)
    bc.num=c(bc.num, bc.num.i);
  }
  bc.num.unique = table(bc.num);
  bc.num.unique.freq = cbind(as.numeric(names(bc.num.unique)), bc.num.unique);
  bc.num.unique.freq[,1] <- format(bc.num.unique.freq[,1], scientific = FALSE)
  colnames(bc.num.unique.freq) <- c("barcode", "freq")
  return(bc.num.unique.freq)
}

# Adjust the barcode frequency given the limit of total number of reads.
normalizeFrequency <- function(n.read, frequency) {
  factors = sum(frequency)/n.read
  result <- floor(frequency/factors)
  return(result)
}
# Sample the barcode for the sequence.
sampleBarcode <- function(bc.true.mat.freq) {
  return(bc.true.mat.freq)
}

EvolveBarcode <- function(frequency_lambda, fitness) {
  return(frequency_lambda*(1+fitness))
}

