#include "hrml.h"

property* currentProperty = NULL;
tag* mainTag = NULL; // at the begining we do not have a main node, only the pointer
tag* currentTag = NULL;

void createProperty(std::string s)
{
    if (currentTag != NULL)
    {
        property* newProp = new property;
        newProp->description = s;
        newProp->next = NULL;
        newProp->value = "";

        if (currentTag->tagProperty == NULL)
        {
            /*
            * Passing by this if statement means that the current tag does not have any property
            * so this will be this first one linked by currentTag->tagProperty
            */
            currentProperty = newProp;
            currentTag->tagProperty = newProp;
        }
        else
        {
            /*
            * Passing by this else statement means that the current tag already has a property
            * so the new property will be added as a node linked by property->next
            * New property must be linked to the last one, to do that we need to walk through
            * all the existent property until we find a NULL value in the las property->next
            */
            currentProperty = currentTag->tagProperty; // walking through properties stars form currentTag->property (i.e. 1st prop)
            while (currentProperty->next != NULL)
                currentProperty = currentProperty->next;
            currentProperty->next = newProp;
        }
        std::cout << "[createProperty] New property '" << s << "' created for " << currentTag->name << std::endl;
    }
    else
        std::cout << "There is no current tag to assign property '" << s << "'" << std::endl;
}

void addSonToFamily(tag* fatherTag, tag* sonTag)
{
    fatherTag->son = sonTag;
    sonTag->father = fatherTag;
}

void addBrotherToFamily(tag* bigBrotherTag, tag* smallBrotherTag)
{
    bigBrotherTag->brother = smallBrotherTag;
    smallBrotherTag->father = bigBrotherTag->father;
    smallBrotherTag->brother = NULL;
}

void getTheFatherOfCurrentNode()
{
    if (currentTag->father != NULL)
        currentTag = currentTag->father;
}

//Prints the content of an string vector, no matter its size (autodetect size)
void printStringVector(std::vector<std::string> &data)
{
    for (int i =0 ; i<data.size() ; i++)
        std::cout << "[printStringVector]" << data[i] << std::endl;
}

void createTag(std::string tagName)
{
    std::cout << "[createTag] Creating tag: " << tagName << std::endl;
    tag* newTag = new tag;
    if (currentTag == NULL)
    {
        //it means this is the 1st node
        mainTag = newTag;
        currentTag = mainTag;
    }
    else
    {
        if (currentTag->son == NULL) //if current tag does not have a son add one to it
        {
            //currentTag->son = newTag;
            addSonToFamily(currentTag, newTag);
        }
        else
        {
            // If current tag has a son then point currentTag pointer to its son to add the
            // new tag as a brother
            currentTag = currentTag->son;

            // if current tag has brothers, walk through them until it finds the last brother
            // then add a new brother
            while (currentTag->brother != NULL)
                currentTag = currentTag->brother;
            currentTag->brother = newTag;
        }
    }
    currentTag->name = stripString (tagName, '<'); // here we need to point who is the current node
    std::cout << "[createTag] Tag: " << currentTag->name << " created! " << std::endl;
}

int detectWordMode(std::string s, int &familyMode)
{
    familyMode = fatherMode; //almost always it will be father mode
    int response = gotAParam; // default value since this notation does not have a special char e.g name, age
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '<')
            response = startTag;
        else if (s[i] == '>' && response == startTag)
            return startEndTag;
        else if (s[i] == '=')
            return gotAnEqual;
        else if (s[i] == '"')
            response = gotAValue;
        else if (s[i] == '>' && response == gotAValue)
            return gotAValueThenEndTag;
        else if (s[i] == '>')
            return gotEndOfHeader;
        else if (s[i] == '/')
        {
            //only in this case family mode will change to son mode
            //it means that we will need to get the father of the current node to set it as the current node
            familyMode = sonMode; 
            return gotAnEndOfTag; 
        }
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
    std::cout << "[strip] Strip blanks, new line, and tabs | result: " << response << std::endl;
    return response;
}

//Removes an especific char from a string (orvelaoding stripString func)
std::string stripString(std::string s, char skipThis)
{
    std::string response = "";
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != skipThis)
        {
            //std::cout << "{" << s[i] << "}" << std::endl;
            response += s[i];
        }
    }
    std::cout << "[strip] strip '" << skipThis << "' from: " << s << " result: "<< response << std::endl;
    return response;
}

/*
Splits a string (based on a delimiter) and return the result in a vector which 
is modified (in size and content) from this function thanks to the reference passing (&)
*/
void splitString(std::string& str, char delimiter, std::vector<std::string>& resultContainer)
{
    std::cout << std::endl << "[splitString]" << "-------------------------------->Start" << std::endl;
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
    std::cout << std::endl << "[splitString]" << "-------------------------------->Done!" << std::endl;
}

// Probably tho most important fucntion, this chunk of code makes the structure (tree)
// based on the raw data read form the hrml file
void createDataStruct(std::string data)
{
    std::vector<std::string> lines; //this will contain each line (\n split) from the text file
    std::vector<std::string> words; //this will contain each word (blankSpace split) for each line
    
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
    int currentFamilyMode = 1; // family mode will be father at the begining
    for (auto element : lines)
    {
        words.clear();
        splitString(element, ' ', words);
        for (int i = 0; i < words.size(); i++)
        {
            words[i] = stripString(words[i]);
            currentWordMode = detectWordMode(words[i], currentFamilyMode);
            std::cout << "[detectWordMode] " << words[i] << " detected as mode: " << currentWordMode << std::endl;
            if (currentFamilyMode == sonMode)
                getTheFatherOfCurrentNode();

            switch (currentWordMode)
            {
                case startTag:
                    createTag(words[i]);
                    break;
                case gotAParam:
                    createProperty(words[i]);
                    break;
            }
        }
        //printStringVector(words);
    }
}

std::string readFile(std::string path)
{
    //create file object
    std::ifstream myfile(path);

    std::string tempBuffer, content;

    //read document line by line
    while (getline(myfile, tempBuffer))
        content += tempBuffer + '\n';

    myfile.close();
    return content;
}