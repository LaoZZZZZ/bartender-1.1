# calculate the distance of two barcodes
library(bit)
letter = c('A', 'C', 'G', 'T')
hammingDistance <- function(barcode1, barcode2) {
  distance = 0
  
  bits1 = bit(barcode1)
  bits2 = bit(barcode2)
  diff = xor.bit(bits1, bits2)
  print(diff)
  i = 1
  while (i < 40) {
    if (any(diff[i:i+1])) {
      distance = distance + 1    
    }
    i = i + 2;
  }
  return(distance)
}


toSequence <- function(barcode) {
  str = c()
  for(i in 1:20) {
    l = bitAnd(barcode,3)
    str[i] = letter[l + 1]
    barcode = bitShiftR(barcode, 2)
  }  
  return(str)
}
