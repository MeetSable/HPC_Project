#ifndef DATAREADER_H
#define DATAREADER_H

#include <fstream>
#include <vector>
#include <any>
#include <omp.h>

#include "HuffmanCode.hpp"
#include "AdaptiveHuffmanCode.hpp"

namespace utils{

    template <typename T>
    void ReadDataset(const std::string& filename, std::vector<T> &dataArray){
        std::ifstream dataBuff(filename, std::ios::binary | std::ios::in);
        if(!dataBuff.is_open()){
            std::cout << "Error opening dataset file" << std::endl;
            return;
        }

        int size = sizeof(T);
        char temp[size];
        char prev = temp[0];
        while(dataBuff.read(temp, sizeof(temp))){
            dataArray.push_back((T)temp[0]);
        }
    }
}

namespace implementations{

    template <typename T>
    static void Chunks2Data(const std::vector<std::vector<T>>& dataChunks, std::vector<T> &dataset){
        if (!dataset.empty())
            dataset.clear();
        for(auto& chunk : dataChunks){
            dataset.insert(dataset.end(), chunk.begin(), chunk.end());
        }
    }

    template <typename T>
    void Data2Chunks(const std::vector<T> &Ddataset, const int num_chunks, std::vector<std::vector<T>> &dataChunks){
        std::vector<T> temp;
        uint32_t s = dataset.size();
        uint32_t len = s / num_chunks;

        for(int i = 0 ; i < s ; i++){
            temp.push_back(dataset[i]);
            if(temp.size() == len){
                dataChunks.push_back(temp);
                temp.clear();
            }
        }

        if(!temp.empty()){
            dataChunks.push_back(temp);
            temp.clear();
        }
        
    }

    /*!
        @brief Calculate Walltime based on start, end and CLOCKS_PER_SEC
    */
    double wallTime(const clock_t& start, const clock_t& end){
        return (double)(end - start)/(double)CLOCKS_PER_SEC;
    }


    /// @brief Test Huffman Encoding method for serial and parallel implementations
    /// @tparam T datatype of dataset
    /// @param dataset vector of dataset
    /// @param resultsDir where the results of the test will be stored
    /// @param max_threads max_number of threads available
    /// @param step step_size for increment number of threads
    template <typename T>
    void HuffmanTest(const std::vector<T>& dataset, const std::string resultsDir, const int max_threads = 1, const int step = 1){
        // Serial Test


        clock_t startEncodeSerial, endEncodeSerial, startDecodeSerial, endDecodeSerial;
        std::string encodedMessage;
        std::vector<T> decodedData;
        HuffmanCode<T> huffmanSerialCode;

        startEncodeSerial = clock();
        huffmanSerialCode.Encode(dataset, encodedMessage);
        endEncodeSerial = clock();

        startDecodeSerial = clock();
        huffmanSerialCode.Decode(encodedMessage, decodedData);
        endDecodeSerial = clock();

        if(max_threads == 1)
            return;
        // Parallel Test



        inline void HuffmanParallelTest(const int num_threads){
            std::vector<std::vector<T>> dataChunks, decodedDataChunks(num_threads);
            std::vector<T> decodedData;
            HuffmanCode<T> huffmanParallelCode;
            Data2Chunks<T>(dataset, num_threads, dataChunks);
            
            #pragma omp parallel private(huffmanParallelCode) shared(dataChunks)
            {
                std::string encodedMessage;
                std::vector<T> decodedData;
                int thread_num = omp_get_thread_num();
                huffmanParallelCode.Encode(Data2Chunks[thread_num], encodedMessage);
                huffmanParallelCode.Decode(encodedMessage, decodedData); 

                decodedDataChunks[thread_num] = decodedData;
            }

            Chunks2Data<T>(decodedDataChunks, decodedDa)
        }

        for(int i = 0 ; i < max_threads ; i += step){
            HuffmanParallelTest(i);
        }        
    }

}

#endif
