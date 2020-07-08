// hrml.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <string> //getline

using namespace std;

struct propStruct
{
    string propName;
    string value;
    propStruct* next;
};

struct tag
{
    //----------------->variable
    string sysID;
    //----------------->properties tree
    propStruct* propertie;
    //----------------->family tree
    tag* father;
    tag* son;
    tag* brother;
};


void printArray(string *data, int size)
{
    for (int i =0 ; i<size ; i++)
        cout << *(data+i) << endl;
}

// modify array size + save splitted data in each array space + return array size
int splitString(string text, string*& line, char delimiter) //string*& lets you modify an array size
{
    int newSize = 3; //test value
    delete[] line; //delete array -> you get something like string* line
    line = new string[newSize]; // create a new array with a new size
    line[0] = "line 0";
    line[1] = "line 1";
    line[2] = "line 2";
    return newSize; //returns array size so you will know the number of splitted elements
}

void createDataStruct(string data)
{
    int lineSize = 1; // just to initialize, it will change after split function
    string* line = new string[lineSize]; //just to initalize/convert apu to array, it will change after split function
    lineSize = splitString(data, line, '\n');
    printArray(line, lineSize);
    //split to get words
    //int wordsSize = 1;
    //string* words = new string[wordSize]
    //process each word with a for loop maybe
}

string readFile(string path)
{
    ifstream myfile(path); //create file object
    string tempBuffer, content;
    while (getline(myfile, tempBuffer)) //read document line by line
        content += tempBuffer + '\n';
    myfile.close();
    return content;
}

int main()
{
    //variables
    string rawData;

    rawData = readFile("./test.hrml");  // get text from document
    createDataStruct(rawData); // create data structure
    cin.get(); // pause console 
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
