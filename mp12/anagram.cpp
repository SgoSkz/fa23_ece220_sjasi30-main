#include "anagram.h"
#include "binary.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

/***
An AnagramDict constructor. This Constructor takes in a filepath
and then creates a binary tree out of all the words inside the
binary tree.

Input:
std::string filename - the path to the file

Output:
N/A
***/
AnagramDict::AnagramDict(std::string filename) {
    filename_ = filename;
    std::string path = "dictionaries/" + filename_;
    std::ifstream file;
    file.open(path.c_str());
    /**
      Your code goes below here
    **/
    if(!file) {
        return;
    }
    std::string line;
    while(!file.eof()) {
        file >> line;
        addWord(line);
    }
    file.close();
}

/***
An AnagramDict member function. This function sorts the given word in
lexicographical order

Input:
std::string word - the word that needs to be sorted

Output:
std::string - the sorted version of the input
***/
std::string AnagramDict::sortWord(std::string word) {
    /**
      Your code goes below here
  **/
    bool diff = false;
    char* real = (char*)malloc(sizeof(char) * word.length() + 1);
    std::strncpy(real, word.c_str(), word.length() + 1);
    char a, b;
    do {
        diff = false;
        for(int i = 0; i < (int)word.length()-1; i++) {
            a = real[i];
            b = real[i+1];
            if(a > b) {
                diff = true;
                real[i] = b;
                real[i+1] = a;
            }
        }
    } while(diff == true);
    std::string r2s = real;
    return r2s;
}

bool exists_in_list(std::list<std::string> t, std::string o) {
    std::list<std::string> strings = t;
    std::list<std::string>::iterator i;
    for(i = strings.begin(); i != strings.end() ; ++i) {
        /* std::cout << *i << std::endl; */
        if(*i == o) {
            return true;
        }
    }
    return false;
}

void print_nodes(Node<std::string, std::list<std::string> > *t) {
    std::list<std::string> strings = t->getData();
    std::list<std::string>::iterator i;
    for(i = strings.begin(); i != strings.end() ; ++i) {
        std::cout << *i << std::endl;
    }
}


/***
An AnagramDict member function. This function adds a word to the tree.
If the words sorted version already exists in the tree add it to the linked
list of that node. If the sorted word does not exist in the tree create a new
node and insert it into the tree
Duplicated words should not be added to the tree.
Input:
std::string word - the word that needs to inserted

Output:
N/A
***/
void AnagramDict::addWord(std::string word) {
    /**
      Your code goes below here
  **/
    if(this->tree.find_node(sortWord(word)) != NULL) {
        /* print_nodes(this->tree.find_node(sortWord(word))); */
        Node<std::string, std::list<std::string> > *ihatethis = this->tree.find_node(sortWord(word));
        std::list<std::string> finalmp = ihatethis->getData();
        if(!exists_in_list(finalmp, word)) {
            finalmp.push_back(word);
            ihatethis->setData(finalmp);
        }
        return;
    }
    std::list<std::string> angarams;
    angarams.push_back(word);
    Node<std::string, std::list<std::string> > *abangarang = new Node<std::string, std::list<std::string> >(sortWord(word), angarams);
    this->tree.insert_node(this->tree.getRoot(), abangarang);
    /* print_nodes(this->tree.getRoot()); */
}

/***
An AnagramDict member function. Does a preorder, postorder, or inorder traversal
and then prints out all the anagrams and words.

The output file should be the traversal order of the tree, but only the data on
each line. View on wiki for more information.

Input:
std::string order - The type of order you want to traverse. Can be "pre",
"post", "in"

Output:
N/A
***/
void AnagramDict::saveAnagrams(std::string order) {
    std::string path = "output/" + order + "_" + filename_;
    std::ofstream file;
    file.open(path.c_str());
    if (!file.is_open()) {
        // create new file
        file.open(path.c_str(), std::ios::out);
    }
    /**
      Your code goes below here
  **/
    std::list<Node<std::string, std::list<std::string> > > nodes;
    if(order == "in") {
        this->tree.in_order(this->tree.getRoot(), nodes);
    }
    if(order == "pre") {
        this->tree.pre_order(this->tree.getRoot(), nodes);
    }
    if(order == "post") {
        this->tree.post_order(this->tree.getRoot(), nodes);
    }
    std::list<Node<std::string, std::list<std::string> > >::iterator i;
    std::list<std::string>::iterator j;
    for(i = nodes.begin(); i != nodes.end(); ++i) {
        std::list<std::string> strings = i->getData();
        for(j = strings.begin(); j != strings.end(); ++j) {
            file << *j << " ";
        }
        file << std::endl;
    }
    file.close();
}
