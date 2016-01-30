library(igraph)
source("/home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation/util.R")
set.seed(23)

sequenceBarcode2 <- function(r.e, bc.true.mat) {
  n.bc = nrow(bc.true.mat)
  col_size = ncol(bc.true.mat)
  len.bc = col_size - 1
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
    #bc.num.i = encodeDNAMatrix(bc.true.mat.batch)
    bc.num.i = apply(bc.true.mat.batch, 1, paste, sep = "", collapse = "")
    bc.num=c(bc.num, bc.num.i);
  }
  bc.num.unique = table(bc.num);
  bc.num.unique.freq = cbind(names(bc.num.unique), bc.num.unique);
  colnames(bc.num.unique.freq) <- c("barcode", "freq")
  return(bc.num.unique.freq)
}
n.bc = 100
len.bc = 20
bc.true.mat = matrix(sample(c(0, 1, 2, 3), n.bc*len.bc,  replace=T), nc=20);

barcode.freq.lambda = 0.1
freq = round(rexp(n.bc, barcode.freq.lambda));
#freq = c(10, 20,30,40)
to_be_sequenced = cbind(bc.true.mat, freq)
r.e = 0.02

sequenced_barcode <- as.matrix(sequenceBarcode2(r.e, to_be_sequenced))
sequenced_barcode[,2] = as.numeric(sequenced_barcode[,2])
sequenced_barcode[,1] = as.character(sequenced_barcode[,1])
sequenced_barcode = sequenced_barcode[order(sequenced_barcode[,2]),]
library(stringdist)
n_node = nrow(sequenced_barcode)
dist_m = stringdistmatrix(sequenced_barcode[,1])
adjacent_matrix = as.matrix(dist_m)
zero_connct = adjacent_matrix > 2
adjacent_matrix[zero_connct] = 0

vertex_size = as.numeric(sequenced_barcode[,2])
vertex_size = vertex_size

adjacent_matrix = adjacent_matrix * max(vertex_size)


vetex_color = ifelse(vertex_size >= 4, 'blue', 'red')

net = graph.adjacency(adjacent_matrix, mode = "undirected", weighted = T, diag = F)

V(net)$color = vetex_color
V(net)$size = vertex_size
plot.igraph(net,vertex.label=NA,layout=layout.fruchterman.reingold)

