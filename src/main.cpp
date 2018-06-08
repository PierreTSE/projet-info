// Début du fichier main.cpp
/**
 * \file main.cpp
 * \brief Contient la fonction main
 * 
 * Ce fichier contient la fonction :
 * int main(int argc, char** argv) 
 * qui est le point d'entrée d'un programme C++
 * (cf documentation de la fonction en question).
 */

#include "Application.hpp" // Inclut le header Application.hpp

/**
 * \brief Point d'entrée du programme C++
 * \param argc Nombre d'arguments de la commande de lancement du programme
 * \param argv Tableau de chaînes de caractères
 * \return Entier qui correspond à un code de statut
 * 
 *  L'argument 0 de argv contient la commande qui correspond à l'appel du programme.
 *  
 *  Pour la valeur de retour, un 0 correspond à une terminaison sans erreur ; une autre valeur
 *  indique que le programme s'est arrêté anormalement.
 */
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    //Début du programme 

    Application app; // Création de l'application
    return app.execute(); // Exécution de l'application

    // Fin de la fonction main
}

// Fin du programme C++

// Fin du fichier main.cpp