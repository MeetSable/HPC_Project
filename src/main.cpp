#include <iostream>

#include "utilsAndImpl.hpp"

const std::vector<std::string> filenames = {
    "1. aneurism_256x256x256_uint8.raw",
    "2. bonsai_256x256x256_uint8.raw",
    "3. pancreas_240x512x512_int16.raw",
    "4. vertebra_512x512x512_uint16.raw",
    "5. magnetic_reconnection_512x512x512_float32.raw"
};
const std::string datasetFolderPath = "../datasets/";

int main(){
    // Choose Dataset
    for(auto& name : filenames){
        std::cout << name << std::endl;

    }

    std::cout << "\nEnter Your Choice: ";
    int ch;
    std::cin >> ch;
    std::vector<uint8_t> arrUint8;
    std::vector<int16_t> arrInt16;
    std::vector<uint16_t> arrUint16;
    std::vector<float> arrFloat;
    switch (ch)
    {
    case 1:
        utils::ReadDataset(datasetFolderPath + filenames[ch-1], arrUint8);
        break;
    case 2:
        utils::ReadDataset(datasetFolderPath + filenames[ch-1], arrUint8);
        break;
    case 3:
        utils::ReadDataset(datasetFolderPath + filenames[ch-1], arrInt16);
        break;        
    case 4:
        utils::ReadDataset(datasetFolderPath + filenames[ch-1], arrUint16);
        break;
    case 5:
        utils::ReadDataset(datasetFolderPath + filenames[ch-1], arrFloat);
        break;
    default: 
        std::cout << "\nEnter a valid choice (1-5)!\n";
        break;
    
    }
    return 0;
}

int main2(){

    std::string filepath = "../datasets/aneurism_256x256x256_uint8.raw";
    
    AdaptiveHuffmanCode<uint8_t> adaptiveHuffmanEncoder, adaptiveHuffmanDecoder;
    HuffmanCode<uint8_t> huffmanCode;
    std::vector<uint8_t> dataset, decodedData;
    utils::ReadDataset<uint8_t>(filepath, dataset);

    std::string encodedMessage;
    // huffmanCode.Encode(dataset, encodedMessage);
    // huffmanCode.Decode(encodedMessage, decodedData);
    adaptiveHuffmanEncoder.Encoder(dataset, encodedMessage);
    adaptiveHuffmanDecoder.Decoder(encodedMessage, decodedData);
    double compressionRatio = (double)(dataset.size()*8) / (double)encodedMessage.length();
    // double compressionRatio = huffmanCode.compressionRatio;
    std::cout << "Compression Ratio: "<< compressionRatio << std::endl;

    return 0;
}