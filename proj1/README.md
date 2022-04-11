# 1. How to compile
'''
g++ -g -o crc_encoder_20161663 crc_encoder_20161663.cpp && g++ -g -o crc_decoder_20161663 crc_decoder_20161663.cpp
'''

# 2. How to run
'''
./crc_encoder_20161663 input_file output_file generator dataword_size
./linksim inputfile outputfile error_ratio(0-1) seed_num
./crc_decoder_20161663 input_file output_file result_file generator dataword_size
'''

## 2-1. example
'''
./crc_encoder_20161663 datastream.tx codedstream.tx 1101 4
./linksim codedstream.tx codedstream.rx 0.0 1001
./crc_decoder_20161663 codedstream.rx datastream.rx result.txt 1101 4
'''
