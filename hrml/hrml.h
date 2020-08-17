#pragma once

#include <iostream>
#include <fstream> //For reading files 
#include <string> //getline, string.find
#include <vector>

enum enum_wordMode
{
    /*
    * tagMode looking for all the existent parameters for this tag
    * paraMode, looking for the value of the current param
    * father mode looking for sons
    */
    unknown = 0,
    startTag = 1,
    startEndTag = 2,
    gotAParam = 3,
    gotAnEqual = 4,
    gotAValue = 5,
    gotAValueThenEndTag = 6,
    gotEndOfHeader = 7,
    gotAnEndOfTag = 8
};

enum enum_familyMode
{
    undefined = 0,
    fatherMode = 1,
    sonMode = 2 //it means that we will need to get the father of the current node to set it as the current node
};

/*
Each tag will have a pinter to a propStruct where its
properties weill be placed.
*/
struct property
{
    std::string description = "";
    std::string value = "";
    property* next = NULL;
};

//Each tag will be represented as a tag Struct
struct tag
{
    //----------------->variable
    std::string name = ""; // tag1, tag2, tag3....
    //----------------->properties tree
    property* tagProperty = NULL;
    //----------------->family tree
    tag* father = NULL;
    tag* son = NULL;
    tag* brother = NULL;
};

void createProperty(std::string s);

void addSonToFamily(tag* fatherTag, tag* sonTag);

void addBrotherToFamily(tag* bigBrotherTag, tag* smallBrotherTag);

void getTheFatherOfCurrentNode();

//Prints the content of an string vector, no matter its size (autodetect size)
void printStringVector(std::vector<std::string>& data);

void createTag(std::string tagName);

int detectWordMode(std::string s, int& familyMode);

//Removes balnk spaces, \n, and \t from a string
std::string stripString(std::string s);

//Removes an especific char from a string (orvelaoding stripString func)
std::string stripString(std::string s, char skipThis);

/*
Splits a string (based on a delimiter) and return the result in a vector which
is modified (in size and content) from this function thanks to the reference passing (&)
*/
void splitString(std::string& str, char delimiter, std::vector<std::string>& resultContainer);

// Probably tho most important fucntion, this chunk of code makes the structure (tree)
// based on the raw data read form the hrml file
void createDataStruct(std::string data);

std::string readFile(std::string path);