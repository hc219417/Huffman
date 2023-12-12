/**
 * @file: huffman.cc
 * @author: Hannah Culver
 * @date: October 26, 2022
 * @brief: design a huffman code tree to encode and decode text files (the tree
 *         must be constructed based on a set of alphabets and their probabilities)
 * 
 * @objective: assign the shortest codes to the characters with the highest probabilities
 * @pat: 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
 */

#include "huffman.h"
using namespace std;

/**
 * displayUsage
 * @param none
 * @return display usage options
 */
void displayUsage();

int main(int argc, char const *argv[]){

    Huffman myhuffman; //create huffman code tree object (this includes building the tree)

    if(argc < 1 || argc > 4){ //check for argument count, must be between 1 and 4 (but NOT 3)
		cout << "Error: invalid command line argument" << endl;
		exit(0);
	}

    if(argc == 1){ //argument count equals 1
        if(std::string(argv[0]) != "./huffman"){
            cout << "Error: invalid command line argument" << endl;
		    exit(0);
        }
        displayUsage(); //help
    }

    if(argc == 2){ //argument count equals 2
        if(std::string(argv[1]) == "-h"){ //help
            displayUsage();
        }
        else if(std::string(argv[1]) == "-p"){ //print
            myhuffman.print();
        }
        else{
            cout << "Error: invalid command line argument" << endl;
		    exit(0);
        }
    }

    else if(argc == 3){ //argument count equals 3 (not allowed)
        cout << "Error: invalid command line argument" << endl;
		exit(0);
    }

    else if(argc == 4){ //argument count equals 4
        if(std::string(argv[1]) == "-e"){ //encode
            myhuffman.encode(std::string(argv[2]), std::string(argv[3]));
        }
        else if(std::string(argv[1]) == "-d"){ //decode
            myhuffman.decode(std::string(argv[2]), std::string(argv[3]));
        }
        else{
            cout << "Error: invalid command line argument" << endl;
		    exit(0);
        }
    }

    return 0;
}

void displayUsage(){
    cout << "options:" << endl;
    cout << "-e: encode file (convert all letters to codes)" << endl;
    cout << "-d: decode file (convert all codes back to letters)" << endl;
    cout << "-p: print all characters with their codes and the average code length" << endl;
    cout << "-h: help (usage)" << endl;
    cout << "\nexamples:" << endl;
    cout << "./huffman -e inputFile.txt outputFile.txt" << endl;
    cout << "./huffman -d inputFile.txt outputFile.txt" << endl;
    cout << "./huffman -p" << endl;
    cout << "./huffman -h (or just ./huffman)" << endl;
}