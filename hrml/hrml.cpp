/*
hrml.cpp contains the main fucntion
*/


#include <iostream>
#include <fstream> //For reading files 
#include <string> //getline, string.find
#include <vector>

using namespace std;

enum enum_wordMode
{
    /*
    * tagMode looking for all the existent parameters for this tag
    * paraMode, looking for the value of the current param
    * father mode looking for sons
    */
    unknownMode,
    tagMode,
    tagThenFatherMode,
    paramMode,
    assingingValueMode,
    readValueMode,
    readValueThenFatherMode,
    brotherMode,
    fatherMode
}wordMode;

/*
Each tag will have a pinter to a propStruct where its 
properties weill be placed.
*/
struct propStruct
{
    std::string propName;
    std::string value;
    propStruct* next;
};


//Each tag will be represented as a tag Struct
struct tag
{
    //----------------->variable
    std::string sysID;
    //----------------->properties tree
    propStruct* propertie;
    //----------------->family tree
    tag* father;
    tag* son;
    tag* brother;
};

//Prints the content of an string vector, no matter its size (autodetect size)
void printStringVector(vector<std::string> &data)
{
    for (int i =0 ; i<data.size() ; i++)
        std::cout << "[printStringVector]" << data[i] << endl;
}

int detectWordMode(string s)
{
    int response = paramMode;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '<')
            response = tagMode;
        else if (s[i] == '>' && response == tagMode)
            return tagThenFatherMode;
        else if (s[i] == '=')
            return assingingValueMode;
        else if (s[i] == '"')
            response = readValueMode;
        else if (s[i] == '>' && response == readValueMode)
            return readValueThenFatherMode;
        else if (s[i] == '>')
            return fatherMode;
        else if (s[i] == '/')
            return brotherMode;
    }
    return response;
}

//Removes balnk spaces, \n, and \t from a string
std::string stripString(std::string s)
{
    std::string response = "";
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t')
        {
            //std::cout << "{" << s[i] << "}" << std::endl;
            response += s[i];
        }
    }
    std::cout << "[strip] " << response << std::endl;
    return response;
}

/*
Splits a string (based on a delimiter) and return the result in a vector which 
is modified (in size and content) from this function thanks to the reference passing (&)
*/
void splitString(string& str, char delimiter, vector<std::string>& resultContainer)
{
    std::cout << endl << "[splitString]" << "-------------------------------->Start" << std::endl;
    unsigned long int substrRangeLow = 0; //big number in case of large string to be processed
    for (int i = 0; i < str.size(); i++)
    {
        std::cout << str[i];
        if (str[i] == delimiter)
        {
            resultContainer.push_back(str.substr(substrRangeLow, i - substrRangeLow));
            substrRangeLow = i + 1; //this makes the var points to the next 1st char of the split data
        }
        else if (i == str.size() - 1) //when passing over the end and \n was not found
        {
            resultContainer.push_back(str.substr(substrRangeLow, i - substrRangeLow + 1));
        }
    }
    std::cout << endl << "[splitString]" << "-------------------------------->Done!" << endl;
}

// Probably tho most important fucntion, this chunk of code makes the structure (tree)
// based on the raw data read form the hrml file
void createDataStruct(string data)
{
    vector<std::string> lines; //this will contain each line (\n split) from the text file
    vector<std::string> words; //this will contain each word (blankSpace split) for each line
    
    splitString(data, '\n', lines);
    //printStringVector(lines);

    /*
    For each line it will walk throut it and get each word by splitting the string using 
    a 'blank space' delimiter. 
    The result of splitting will be stored in the words vector which will be cleared 
    before processing a new line
    Finally, for each word detected it will perform an specific action that could be
    create a new tag (node) or create a new propertie node. 
    */
    int currentWordMode = 0;
    for (auto element : lines)
    {
        words.clear();
        splitString(element, ' ', words);
        for (int i = 0; i < words.size(); i++)
        {
            words[i] = stripString(words[i]);
            currentWordMode = detectWordMode(words[i]);
            std::cout << "[detecWordMode] " << words[i] << " detected as mode: " << currentWordMode << std::endl;
        }
        //printStringVector(words);
    }
}

string readFile(string path)
{
    //create file object
    ifstream myfile(path); 

    string tempBuffer, content;

    //read document line by line
    while (getline(myfile, tempBuffer))
        content += tempBuffer + '\n';

    myfile.close();
    return content;
}

int main()
{
    string rawData;

    rawData = readFile("./test.hrml");
    createDataStruct(rawData); 
    cin.get(); 
}