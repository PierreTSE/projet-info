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
#include <iostream> // Inclut la bibliothèque de gestion de flux d'entrée-sortie standards
#include "Utilities/TextBox/TextBox.hpp"
/**
 * \fn main
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
/*
 * Pour indiquer au compilateur que les paramètres argc et argv ne seront probablement pas utilisés,
 * nous pouvons utiliser un attribut qui fait partie des standards et sera donc multiplateforme
 * à condition d'utiliser un compilateur conforme :
 * http://en.cppreference.com/w/cpp/language/attributes
 * Cet attribut en particulier n'est disponible qu'à partir de la norme C++17 mais ce n'est pas
 * un problème ici étant donné que d'autres fonctionnalités dans le programme nécéssitent également 
 * l'usage de C++17.
 */
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    //Début du programme 

    // Cette partie est triviale, et sa compréhension est laissée en exercice au lecteur
	std::ofstream os("Log.txt", std::ios::out | std::ios::app);
	std::unique_ptr<std::streambuf, std::function<void(std::streambuf*)>> buff(std::clog.rdbuf(), [](std::streambuf* buf) { std::clog.rdbuf(buf); });
	std::clog.rdbuf(os.rdbuf());

	std::string str = pOpen("");

    Application app; // Création de l'application
    return app.execute(); // Exécution de l'application



    // Fin de la fonction main
}

// Fin du programme C++

// Fin du fichier main.cpp