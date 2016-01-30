#!/bin/bash

#./simulation_test /home/laozzzzz/Documents/barcode_single_end/data/single_end_data/single_end_table.csv /home/laozzzzz/Documents/barcode_single_end/data/single_end_cluster_7 10 0 2 3

#./simulation_test ~/barcode_single_end/barcode_single_end/simulation_data/Simulated_barcode_freq.txt ~/barcode_single_end/barcode_single_end/simulation_data/simulated_data_cluster 4 1  
#Qbartender_single_com -f ~/barcode_single_end/barcode_single_end/simulation_data/Simulated_barcode_freq_primer.csv -o ~/barcode_single_end/barcode_single_end/simulation_data/simulated_data_cluster -c 1 -e 0.02
#bartender_single_com -f primer_test.csv -o primer_test -c 1 -e 0.02
#bartender_single_com -f ../../Simulated_barcode_freq_primer.csv -o primer_test -c 1 -e 0.02
#bartender_single_com -f ../../4M_7_barcode.csv -o primer_test -c 1 -e 0.02

bartender_single_com -f /home/luzhao/barcode_single_end/data/Simulated_barcode_freq_primer.csv -o simulated_barcode -c 1 -e 0.02 -l 5  
#./simulation_test ~/Documents/barcode_single_end/simulation_data/trucated.txt ~/Documents/barcode_single_end/simulation_data/trucated_cluster 4 1  
#./simulation_test /home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation/Sequenced_time_point_1.csv /home/laozzzzz/Documents/barcode_single_end/simulation_data/result_1 10 0   





