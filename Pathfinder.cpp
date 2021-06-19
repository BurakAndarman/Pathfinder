#include <list>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>
using namespace std;


class Node {
public:
    string name;
    list<string>adjacents;

};

void createNodeNames(string fileName,list<Node>& nodes) {
    Node dummy;
    string line;
    string name;

    ifstream fileStream(fileName);
    if (fileStream.is_open()) {
        while (getline(fileStream, line)) {
            stringstream stringStream(line);
            while (getline(stringStream, name, ',')) {
                Node dummy;
                dummy.name = name;
                nodes.push_back(dummy);
            }
            break;
        }
    }
    else {
        cout << "File could'nt open" << endl;
    }

    fileStream.close();
}

void createNodeAdjacencies(string fileName, list<Node>& nodes) {
    string line;
    string label_1 = "Links:";
    string label_2 = "Paths:";
    bool flag = false;


    ifstream fileStream(fileName);
    if (fileStream.is_open()) {
        while (getline(fileStream, line)) {
            stringstream stringStream(line);
            if (strcmp(line.c_str(), label_1.c_str())==0) {
                flag = true;
                continue;
            }
            else if (strcmp(line.c_str(), label_2.c_str()) == 0) {
                break;
            }
            if (flag == true) {
                int index = line.find("-");
                string sourceNode = line.substr(0, index);
                string targetNode = line.substr(index + 2, line.length() - (index + 2));

                for (auto itr = nodes.begin(); itr != nodes.end(); itr++) {
                    if (itr->name == sourceNode) {
                        itr->adjacents.push_back(targetNode);
                        break;
                    }
                }
            }
        }
    }
    else {
        cout << "File could'nt open" << endl;
    }

    fileStream.close();
}

void createPaths(string fileName, list<string>& paths) {
    string line;
    string label = "Paths:";
    bool flag = false;


    ifstream fileStream(fileName);
    if (fileStream.is_open()) {
        while (getline(fileStream, line)) {
            stringstream stringStream(line);
            if (strcmp(line.c_str(), label.c_str()) == 0) {
                flag = true;
                continue;
            }
            if (flag == true) {
                paths.push_back(line);
            }
        }
    }
    else {
        cout << "File could'nt open" << endl;
    }

    fileStream.close();
}

void createListWithSameLetter(char letter, list<Node> nodes,list<Node>& listWithSameLetter) {
    Node dummy;
    string toString(1, letter);

    for (auto itr = nodes.begin(); itr != nodes.end(); itr++) {
        Node dummy;
        if (itr->name.substr(0, 1)==toString) {
            dummy = *(itr);
            listWithSameLetter.push_back(dummy);
        }
    }
}

void createPathAvailability(string fileName,string word,string availability) {
    ofstream outStream(fileName, ios::app);

    if (outStream.is_open()) {
        outStream << word << ' ' << availability << '\n';
    }
    else {
        cout << "File couldn't open or create" << endl;
    }

    outStream.close();
}


void pathfinder(list<Node> nodes, list<Node> firsLetterList,const char*& letter,string word,bool& word_flag ) {

    list<Node> temp;
    stack<string>passedNodes;
    stack<list<Node>>connections_stack;
    bool flag = false;

    for (auto first_letter_iter = firsLetterList.begin(); first_letter_iter != firsLetterList.end(); first_letter_iter++) {

        letter++;
        list<Node> temp;
        passedNodes.push(first_letter_iter->name);
        createListWithSameLetter(*(letter), nodes, temp);
        connections_stack.push(temp);


        while (true) {

            flag = false;
            string node;
            list<Node>nodeList;

            node = passedNodes.top();
            nodeList = connections_stack.top();

            string next;

            for (auto node_iter = nodes.begin(); node_iter != nodes.end(); node_iter++) {
                if (node_iter->name == node) {
                    for (auto iter = nodeList.begin(); iter != nodeList.end(); iter++) {
                        for (auto iter_adjacency = node_iter->adjacents.begin(); iter_adjacency != node_iter->adjacents.end(); iter_adjacency++) {
                            if (iter->name == *(iter_adjacency)) {
                                next = iter->name;

                                list<Node> temp;
                                passedNodes.push(next);

                                if (passedNodes.size() != word.size()) {
                                    letter++;
                                    createListWithSameLetter(*(letter), nodes, temp);
                                    connections_stack.push(temp);
                                }

                                flag = true;
                                break;
                            }
                        }
                        if (flag == true) {
                            break;
                        }
                    }
                    if (flag == true) {
                        break;
                    }
                }
            }
            

            if (passedNodes.size() == word.size()) {

                createPathAvailability("output.txt", word, "[YES]");

                word_flag = true;
                break;
            }

            if (flag == false && passedNodes.size() == 1) {

                letter--;
                passedNodes.pop();
                connections_stack.pop();

                break;
            }

            else if (flag == false) {

                list<Node>next_connections;

                letter--;
                passedNodes.pop();
                connections_stack.pop();
                next_connections = connections_stack.top();

                for (auto itr = next_connections.begin(); itr != next_connections.end(); itr++) {
                    if (node == itr->name) {
                        next_connections.erase(itr);
                        break;
                    }
                }

                connections_stack.push(next_connections);

            }

        }

        if (word_flag == true) {
            break;
        }

    }
}


int main()
{
    
    list<Node> nodes;
    list<string>paths;

    createNodeNames("input.txt", nodes);
    createNodeAdjacencies("input.txt", nodes);
    createPaths("input.txt", paths);


    string word;
    list<Node> firsLetterList;
    bool word_flag = false;
    bool flag = false;


    for (auto word_iter = paths.begin(); word_iter != paths.end(); word_iter++) {

        firsLetterList.clear();
        word_flag = false;
        word = *(word_iter);
        const char* letter = word.c_str();
        createListWithSameLetter(*(letter), nodes, firsLetterList);

        pathfinder(nodes, firsLetterList, letter, word, word_flag);

        if (word_flag==false) {

            createPathAvailability("output.txt", word, "[NO]");
            
        }

    }

    return 0;
    
}

