/**
 * @file: huffman.h
 * @author: Hannah Culver
 * @date: October 26, 2022
 * @brief: header file for the huffman code tree class
 */

#ifndef HUFFMAN
#define HUFFMAN

#include <queue>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>

using namespace std;

struct Node{
    Node(){letter = ' ', frequency = 0.0, code = " ", code_length = 0, left = right = nullptr;} //zero-arg c-tor

    Node(char new_letter, double new_frequency, string new_code, int new_code_length, Node *new_left, Node *new_right){ //full service c-tor
        letter = new_letter;
        frequency = new_frequency;
        code = new_code;
        code_length = new_code_length;
        left = new_left;
        right = new_right;
    }

    char letter;
    double frequency;
    string code;
    int code_length;

    Node *left;
    Node *right;
};

struct Compare{ //compare frequencies (used to create min queue)
    bool operator() (Node *left_node, Node *right_node){
        return(left_node -> frequency > right_node -> frequency);
    }
};

class Huffman{
    public:
    Huffman(); //default constructor

    void encode(string input_filename, string output_filename){file_encode(root, input_filename, output_filename);} //encode (convert letters to codes)
    void decode(string input_filename, string output_filename){file_decode(root, input_filename, output_filename);} //decode (convert codes to letters)

    bool isLeaf(const Node *nodeptr); //used for file_decode() function
    void print(); //print all characters with their codes and the average code length

    private:
    Node *root; //root

    //private functions
    void file_encode(Node *root, string input_filename, string output_filename); //used for encode() function
    void file_decode(Node *root, string input_filename, string output_filename); //used for decode() function

    void print_depth(Node *root, int depth); //used for testing to see if tree was being built properly
    void load_codes(Node *root, string tmp_code); //used in default constructor

    string find_code(Node *root, char target); //used for file_encode() function

    void print_tree(Node *root); //used for print() function
    void avg_code_length(); //used for print() function

    //private variables
    vector<char> letters; //used in default constructor
    vector<double> freqs; //used in default constructor

    vector<char> characters; //used for load_codes() function
    vector<string> ch_codes; //used for load_codes() function

    int size; //number of unique letters (26 in the alphabet)
    double total_code_length = 0.0; //total code lengths
};

#endif

Huffman::Huffman(){ //default constructor
    root = nullptr;

    ifstream inStream;
    inStream.open("alpha.txt"); //read in letters and frequencies from "alpha.txt"
    if(inStream.fail()){
        cout << "Error: file not found" << endl;
        exit(0);
    }

    char file_letter;
    string file_freq;

    while(inStream >> file_letter >> file_freq){
        letters.push_back(file_letter); //load up letters vector
        freqs.push_back(stod(file_freq)); //load up freqs vector
    }
    size = letters.size(); //find total number of unique letters
    inStream.close();

    priority_queue<Node*, vector<Node*>, Compare> q; //create priority queue object of Nodes using Compare structure

    for(int i = 0; i < size; i++){ //load up the queue
        q.push(new Node(letters[i], freqs[i], " ", 0, nullptr, nullptr));
    }

    while(q.size() != 1){ //build the tree
        auto leftptr = q.top(); //take top item
        q.pop(); //remove it from the queue
        auto rightptr = q.top(); //take next top item
        q.pop(); //remove it from the queue
        auto tmp = new Node('$', leftptr -> frequency + rightptr -> frequency, " ", 0, nullptr, nullptr); //add up their frequencies and create
        tmp -> left = leftptr; //adjust left pointer                                                      //a new node with temporary code ('$')
        tmp -> right = rightptr; //adjust right pointer
        q.push(tmp); //add the new node back to the queue
        root = tmp; //set root equal to top node
    }

    load_codes(root, ""); //load codes
}

bool Huffman::isLeaf(const Node *nodeptr){
    return (nodeptr -> left == nullptr) && (nodeptr -> right == nullptr);
}

void Huffman::print(){ //used in the main program (huffman.cc)
    print_tree(root);
    avg_code_length();
}

void Huffman::file_encode(Node *root, string input_filename, string output_filename){
    ifstream inStream;
    inStream.open(input_filename);
    if(inStream.fail()){
        cout << "Error: file not found" << endl;
        exit(0);
    }

    ofstream outStream;
	outStream.open(output_filename);
	if(outStream.fail()){
		cout << "Error: failed to create file" << endl;
		exit(0);
	}

    char ch;

    while(inStream.get(ch)){
        if(isalpha(ch)){
            outStream << find_code(root, char(tolower(ch)));
        }
        else outStream << ch; //punctuations and new lines need not be encoded or decoded
    }                         //and should maintain their locations within the data files

    inStream.close();
    outStream.close();
}

void Huffman::file_decode(Node *root, string input_filename, string output_filename){
    ifstream inStream;
    inStream.open(input_filename);
    if(inStream.fail()){
        cout << "Error: file not found" << endl;
        exit(0);
    }

    ofstream outStream;
	outStream.open(output_filename);
	if(outStream.fail()){
		cout << "Error: failed to create file" << endl;
		exit(0);
	}

    char ch;
    Node *cursor = root;

    while(inStream.get(ch)){ //copied over from notes (w/ some slight modifications)
        if(ch == '0'){
            cursor = cursor -> left;
            if(isLeaf(cursor)){ //character is found
                outStream << cursor -> letter;
                cursor = root; //reset cursor
            }
        }
        else if(ch == '1'){
            cursor = cursor -> right;
            if(isLeaf(cursor)){ //character is found
                outStream << cursor -> letter;
                cursor = root; //reset cursor
            }
        }
        else outStream << ch; //ignore punctuation (anything other than a '0' or '1')
    }

    inStream.close();
    outStream.close();
}

void Huffman::print_depth(Node *root, int depth){ //copied over from notes (for testing)
    if(root != nullptr){
        print_depth(root -> right, depth + 1);
        cout << setw(4*depth) << "" //indent 4*depth
             << root -> letter << endl;
        print_depth(root -> left, depth + 1);
    }
}

void Huffman::load_codes(Node *root, string tmp_code){
    if(root != nullptr){ //in-order traversal
        load_codes(root -> left, tmp_code + "0");
        if(root -> letter != '$'){
            root -> code = tmp_code; //enter code into node
            characters.push_back(root -> letter); //load up characters vector
            ch_codes.push_back(tmp_code); //load up ch_codes vector
            root -> code_length = tmp_code.length(); //find code length
        }
        load_codes(root -> right, tmp_code + "1");
    }
}

string Huffman::find_code(Node *root, char target){
    int index; //idea is to find and return matching index
    for(size_t i = 0; i < characters.size(); i++){
        if(target == characters[i]){
            index = i;
            break;
        } //break out of for loop once match is found
    }
    return ch_codes[index]; //return matching index
}

void Huffman::print_tree(Node *root){
    if(root != nullptr){ //in-order traversal
        print_tree(root -> left);
        if(root -> letter != '$'){
            total_code_length += root -> code_length; //update total_code_length
            cout << root -> letter << ": " << root -> code << endl;
        }
        print_tree(root -> right);
    }
}

void Huffman::avg_code_length(){ //total and average code length (2 in 1)
    cout << "total code length: " << total_code_length << endl;
    cout << "average code length: " << total_code_length / size << endl;
}

//eof