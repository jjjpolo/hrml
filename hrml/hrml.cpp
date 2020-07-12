// hrml.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <string> //getline, string.find
#include <vector>

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


void printStringVector(vector<std::string> &data)
{
    for (int i =0 ; i<data.size() ; i++)
        cout << "[printStringVector]" << data[i] << endl;
}

void splitString(string& str, char delimiter, vector<std::string>& resultContainer)
{
    cout << endl << "[splitString]" << "--------------------->Start" << endl << str;
    unsigned long int substrRangeLow = 0; //big number in case of large string to be processed
    for (int i = 0; i < str.size(); i++)
    {
        cout << str[i];
        if (str[i] == delimiter)
        {
            resultContainer.push_back(str.substr(substrRangeLow, i - substrRangeLow));
            substrRangeLow = i + 1; //this makes the var points to the next 1st char of the split data
        }
        else if (i == str.size() - 1)
        {
            resultContainer.push_back(str.substr(substrRangeLow, i - substrRangeLow + 1));
        }
    }
    cout << endl << "[splitString]" << "--------------------->Done" << endl << endl;
}

void createDataStruct(string data)
{
    vector<std::string> lines; //this will contain each line (\n split) from the text file
    vector<std::string> words; //this will contain each word (blankSpace split) for each line
    
    splitString(data, '\n', lines);
    printStringVector(lines);

    for (auto element : lines)
    {
        words.clear();
        splitString(element, ' ', words);
        printStringVector(words);
    }
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
