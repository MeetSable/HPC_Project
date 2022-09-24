#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include <queue>
#include <vector>
#include <string>
#include <unordered_map>

template <typename T>
class HuffmanCode {
    #define MAX_TREE_HT 50
    struct Node {
        T symbol;
        uint32_t freq;
        Node *left, *right;
        Node(const T &item, const uint32_t &freq)
            : symbol(item), freq(freq), left(NULL), right(NULL) {}
        bool isLeaf(){
            return !(this->left) && !(this->right);
        }
    };

    class Comparator{
    public:
        bool operator()(const Node* a, const Node* b){
            return a->freq > b->freq;
        }
    };
    
    using pq = std::priority_queue<Node*, std::vector<Node*>, Comparator>;

private:
    Node *root;
    std::unordered_map<T, uint32_t> symbolFrequencies;
    std::unordered_map<T, std::string> huffmanCodes;

public:
    pq frequencyQueue;
    double compressionRatio;

private:
    void ComputeFrequencies(const std::vector<T>& data){
        // Computing frequencies of all items
        for(auto& item : data){
            symbolFrequencies[item]++;
        }

        //pushing back frequncies into priority queue
        for(auto& item : symbolFrequencies){
            frequencyQueue.push(new Node(item.first, item.second));
        }
    }

    void BuildHuffmanTree(){
        while(frequencyQueue.size() != 1){
            Node* left = frequencyQueue.top();
            frequencyQueue.pop();
            Node* right = frequencyQueue.top();
            frequencyQueue.pop();
            Node* top = new Node('$', left->freq + right->freq);
            top->left = left;
            top->right = right;
            frequencyQueue.push(top);
        }
        root = frequencyQueue.top();
    }

    void BuildHuffmanCodes(Node *curr, int arr[], int top){
        if (curr->left){
            arr[top] = 0;
            BuildHuffmanCodes(curr->left, arr, top+1);
        }

        if (curr->right){
            arr[top] = 1;
            BuildHuffmanCodes(curr->right, arr, top+1);
        }

        if(curr->isLeaf()){
            std::string tempCode;
            for(int i = 0 ; i < top ; i++)
                tempCode += std::to_string(arr[i]);
            huffmanCodes[curr->symbol]+=tempCode;
        }
    }

    void DeleteTree(Node *curr){
        if(curr == nullptr) return;
        DeleteTree(curr->right);
        DeleteTree(curr->left);
        delete curr;
    }
    
    void ComputeCompressionRatio(){
        double up = 0.0, down = 0.0;
        for(auto &i : huffmanCodes){
            up += sizeof(T) * 4.0 * symbolFrequencies[i.first];
            down += i.second.size() * symbolFrequencies[i.first];
        }
        compressionRatio = up / down;
    }

public:
    HuffmanCode(){
        root = NULL;
    }
    
    ~HuffmanCode(){
        DeleteTree(root);
    }
    
    void Encode(const std::vector<T>& data, std::string& encodedMessage){
        // computing frequency and storing it in priority queue
        ComputeFrequencies(data);

        // build huffman tree from the priority queue
        BuildHuffmanTree();

        // Generate codes from the huffman tree
        int arr[MAX_TREE_HT], top = 0;
        BuildHuffmanCodes(root, arr, top);

        // encode the data using Huffman codes
        for(auto& i : data)
            encodedMessage += huffmanCodes[i];
        
        ComputeCompressionRatio();
    }

    void Decode(const std::string& encodedMessage, std::vector<T>& decodedMessage){
        Node* curr = root;
        for(auto& i : encodedMessage){
            if(i=='0')
                curr = curr->left;
            else
                curr = curr->right;
            if (curr->isLeaf()){
                decodedMessage.push_back(curr->symbol);
                curr = root;
            }
        }
    }

};

#endif