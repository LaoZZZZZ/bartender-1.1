//
//  oneproportiontest.hpp
//  barcode_project
//
//  Created by luzhao on 1/13/16.
//  Copyright © 2016 luzhao. All rights reserved.
//


#ifndef FORMATS_H_
#define FORMATS_H_

namespace barcodeSpace {
/**
 * File-format constants and names
 */

typedef enum file_format {
    FASTQ = 1,
    FASTA,
    RAWSEQ,
}file_format;
    
typedef enum CLUSTERTYPE{
    DICTATOR = 1,
    CONSENSUS,
}CLUSTERTYPE;

typedef enum NStrategy{
    CHOMP = 1,
    DISCARD,
    NTOA,
    NTOG
}NStrategy;

typedef enum TESTSTRATEGY {
    TWOPROPORTIONUNPOOLED = 1,
    TWOPROPORTIONPOOLED,
    ONEPROPORTION
}TESTSTRATEGY;
    
}   // namespace barcodeSpace
#endif /*FORMATS_H_*/
