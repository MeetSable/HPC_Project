#ifndef ADAPTIVE_HUFFMAN_CODE_H
#define ADAPTIVE_HUFFMAN_CODE_H

#include <string>
#include <bitset>
#include <sstream>
#include <cstring>
#include <iostream>

template <typename T>
class AdaptiveHuffmanCode{
    struct Node{
        T symbol;
        uint32_t weight;
        uint32_t order;

        Node* left;
        Node* right;
        Node* parent;

        bool isNYT;

        Node()
            :symbol(0), weight(0), order(0), left(nullptr), right(nullptr), parent(nullptr){}
        
        Node(T symbol, int weight, int order, 
            Node* left, Node* right, Node* parent,
            bool isNYT = false)
            : symbol(symbol), weight(weight), order(order), left(left), right(right), parent(parent), isNYT(isNYT) {}
        
        Node(bool isNYT, T symbol)
            : Node() {
            this->isNYT = isNYT;
            this->symbol = symbol;
        }
    };

private:
    uint8_t e = 4;
    uint8_t r = 10;
    Node *root, *NYTNode;

    static const T PSEUDO_EOF = -1;

public:

private:
    Node* GetSymbolNode(const T &symbol, Node* curr) const{
        if (curr == nullptr || curr->symbol == symbol)
            return  curr;
        Node* leftResult = GetSymbolNode(symbol, curr->left);
        return leftResult == nullptr ? GetSymbolNode(symbol, curr->right) : leftResult;
    }

    void FindBlockLeader(Node* curr, Node* currMax) const{
        if(curr == nullptr || currMax == root)
            return;

        if(curr->weight == currMax->weight && curr != currMax->parent && curr->order > currMax->order)
            currMax = curr;

        FindBlockLeader(curr->left, currMax);
        FindBlockLeader(curr->right, currMax);      
    }

    std::string GetPathToSymbol(Node* crr, Node* result, std::string currentPath) const{
        if(crr == result)
            return currentPath;
        if(crr == nullptr)
            return "";
        
        std::string left = GetPathToSymbol(crr->left, result, currentPath + "0");
        return left != "" ? left : GetPathToSymbol(crr->right, result, currentPath + "1");
    }

    void SwapNodes(Node *first, Node *second){
        if(first->parent == nullptr || second->parent == nullptr)
            return;
        if(first->parent == second || second->parent == first)
            return;
        
        Node* &firstRef = first->parent->left == first ? first->parent->left : first->parent->right;
        Node* &secondRef = second->parent->left == second ? second->parent->left : second->parent->right;

        std::swap(firstRef, secondRef);
        std::swap(firstRef->parent, secondRef->parent);
        std::swap(firstRef->order, secondRef->order);
    }

    void SlideAndIncrement(Node* node){
        if(node == nullptr)
            return;
        
        Node* blockLeader = node;
        FindBlockLeader(root, blockLeader);
        if (blockLeader != node)
            SwapNodes(blockLeader, node);
        
        ++node->weight;
        SlideAndIncrement(node->parent);
    }

    void DeleteHuffmanTree(Node* curr){
        if (curr == nullptr) return;

        DeleteHuffmanTree(curr->left);
        DeleteHuffmanTree(curr->right);

        delete curr;
    }

    std::string Encode(const T symbol){
        Node* symbolNode = GetSymbolNode(symbol, root);
        if(symbolNode != nullptr){
            std::string result = GetPathToSymbol(root, symbolNode, "");
            UpdateTreeModel(symbol);
            return result;            
        }
        std::string ss;
        ss = GetPathToSymbol(root, NYTNode, "") + std::bitset<9>(symbol).to_string();
        UpdateTreeModel(symbol);
        return ss;
    }

    T Decode(const char *message, int &index){
        T result = -1;
        Node* crr = root;
        while(result == -1){
            if(crr->symbol != -1){
                result = crr->symbol;
                UpdateTreeModel(crr->symbol);
                crr = root;
            }
            else if (crr->isNYT){
                char numberArr[10];
                strncpy(numberArr, message+index, 10);
                index += 10;
                T number = std::stoi(numberArr, nullptr, 2);
                result = number;
                UpdateTreeModel(number);
                crr = root;
            }
            else if(message[index] == '0'){
                crr = crr->left;
            }
            else
                crr = crr->right;
        }
        return result;
    }

    void UpdateTreeModel(const T& symbol){
        Node* leafToIncrement = nullptr;
        Node* symbolNode = GetSymbolNode(symbol, root);        

        if (symbolNode == nullptr){
            NYTNode->isNYT = false;
            int currentMinOrder = NYTNode->order;
            NYTNode->left = new Node(-1, 0, currentMinOrder - 2, nullptr, nullptr, NYTNode, true);
            NYTNode->right = new Node(symbol, 0, currentMinOrder - 1, nullptr, nullptr, NYTNode, false);
            symbolNode = NYTNode->right;
            NYTNode = NYTNode->left;
        }

        SlideAndIncrement(symbolNode);
    }

public:
    AdaptiveHuffmanCode()
        :root(new Node(-1, 0, 512, nullptr, nullptr, nullptr, true)) {
        NYTNode = root;
    }

    ~AdaptiveHuffmanCode(){
        DeleteHuffmanTree(root);        
    }

    void Encoder(const std::vector<T>& dataset, std::string& encodedMessage){
        long long size = dataset.size();
        long long ind = 0;
        const int printDelay = 500; 
        std::cout << "\nEncoding... \n";
        clock_t start;
        start = clock();
        for(auto& symbol : dataset){
            if(double(clock() - start)/(double)CLOCKS_PER_SEC * 1000 > printDelay){
                printf("\rProgress: %.3f %%", (double)ind/(double)size * 100.0f);
                start = clock();
            }
            encodedMessage += Encode(symbol);
            ind++;
        }
        encodedMessage += Encode(PSEUDO_EOF);
        std::cout << "Encoding done." << std::endl;
    }

    void Decoder(const std::string& encodedMessage, std::vector<T>& decodedData){
        const char *message = encodedMessage.c_str();
        int index = 0;
        T symbol = Decode(message, index);
        std::cout << "\nDecoding... \n";
        const int printDelay = 500;
        clock_t start;
        start = clock();
        while(symbol != PSEUDO_EOF){
            if(double(clock() - start)/(double)CLOCKS_PER_SEC * 1000 > printDelay){
                printf("\rProgress: %.3f  %%", (double)index/(double)encodedMessage.size() * 100.0f);
                start = clock();
            }
            decodedData.push_back(symbol);
            symbol = Decode(message, index);
        }
        std::cout << "Decoding done." << std::endl;

    }
    
};

#endif