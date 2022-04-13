# 1. How to compile
## 1-1. For debugging
g++ -g -o crc_encoder_20161663 crc_encoder_20161663.cpp && \
g++ -g -o crc_decoder_20161663 crc_decoder_20161663.cpp

## 1-2. For submit
g++ -o crc_encoder_20161663 crc_encoder_20161663.cpp && \
g++ -o crc_decoder_20161663 crc_decoder_20161663.cpp

# 2. How to run
./crc_encoder_20161663 input_file output_file generator dataword_size && \
./linksim inputfile outputfile error_ratio(0-1) seed_num && \
./crc_decoder_20161663 input_file output_file result_file generator dataword_size

## 2-1. example_1 (generator = 1101, dataword_size = 4, error_rate = 0, seed = 1001)
./crc_encoder_20161663 datastream.tx codedstream.tx 1101 4 && \
./linksim codedstream.tx codedstream.rx 0.0 1001 && \
./crc_decoder_20161663 codedstream.rx datastream.rx result.txt 1101 4

## 2-2. example_2 (generator = 1101, dataword_size = 8, error_rate = 0, seed = 1001)
./crc_encoder_20161663 datastream.tx codedstream.tx 1101 8 && \
./linksim codedstream.tx codedstream.rx 0.0 1001 && \
./crc_decoder_20161663 codedstream.rx datastream.rx result.txt 1101 8

## 2-3. example_3 (generator = 1101 dataword_size = 4, error_rate = 0.05, seed = 1001)
./crc_encoder_20161663 datastream.tx codedstream.tx 1101 4 && \
./linksim codedstream.tx codedstream.rx 0.05 1001 && \
./crc_decoder_20161663 codedstream.rx datastream.rx result.txt 1101 4

# 3. How to clean
rm crc_encoder_20161663 crc_decoder_20161663 codedstream.tx *.rx result.txt

# 4. How to test
## 4-1. encoder test
./encoder_test.out
실행 시 encoder의 output_file을 byte 단위로 보여준다.
