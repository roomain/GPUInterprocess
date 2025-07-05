// GPUInterprocess.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <format>
#include <type_traits>
#include <stdlib.h>     //for using the function sleep
#include "SharedMemory.h"
#include "MultithreadData.h"
#include "openCLFunctions.h"

static bool g_Run = true;

using SharedString = MultithreadData<std::string>;



void writeInterprocessData()
{
    SharedMemory<SharedString> sharedMemWrite(L"SharedTest", OpenMode::Write_Only, 65536);
    SharedString* const sharedDataWrite = sharedMemWrite.createObject("sharedData");
    sharedDataWrite->process([](std::string& data) {data = "Hello world"; });

    std::string base = "Hello World";
    std::string generated;
    int index = 1;
    while (g_Run)
    {
        generated = std::format("{0} {1}", base, index);
        std::cout << "Write: " << generated << std::endl;
        sharedDataWrite->process([&generated](std::string& data) {data = generated; });
        Sleep(5000);
        ++index;
    }
}


int main()
{
    displayOpenCLInfo();

    //writeInterprocessData();
    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
