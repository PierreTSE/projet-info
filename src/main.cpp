// Début du fichier main.cpp
/**
 *  \file main.cpp
 *  \brief Contient la fonction main
 *  
 *  Ce fichier contient la fonction :
 *  int main(int argc, char** argv) 
 *  qui est le point d'entrée d'un programme C++
 *  (cf documentation de la fonction en question).
 */

#include "Application.hpp" // Inclut le header Application.hpp
#include <iostream> // Inclut la bibliothèque de gestion de flux d'entrée-sortie standards
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
 *  Pour plus d'information sur cette fonction, le lecteur est invité à consulter les spécifications du 
 *  standard C++.
 *  Nous en donnons une version de travail ici, la version finale étant inaccessible publiquement :
 *  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4640.pdf#page=77
 */
/*
 *  Pour indiquer au compilateur que les paramètres argc et argv ne seront probablement pas utilisés,
 *  nous pouvons utiliser un attribut qui fait partie des standards et sera donc multiplateforme
 *  à condition d'utiliser un compilateur conforme :
 *  http://en.cppreference.com/w/cpp/language/attributes
 *  Cet attribut en particulier n'est disponible qu'à partir de la norme C++17 mais ce n'est pas
 *  un problème ici étant donné que d'autres fonctionnalités dans le programme nécéssitent également 
 *  l'usage de C++17.
 */
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    // Début du programme C++

    // Cette partie sert à gérer le fichier de log
	std::ofstream os("Log.txt", std::ios::out | std::ios::app); // Flux de sortie pour écrire le log
	std::unique_ptr<std::streambuf, std::function<void(std::streambuf*)>> buff( 
	    std::clog.rdbuf(), [](std::streambuf* buf) { std::clog.rdbuf(buf); });
    /* Pointeur intelligent sur le rdbuf original de std::clog qui permet grâce à son destructeur personnalisé
    *  de remettre std::clog à son état normal (lui réattribuer son buffer interne original).
    */
	std::clog.rdbuf(os.rdbuf()); // Redirection de std::clog sur le fichier texte ouvert précédemment 
    /* Nous aurions pu utiliser cette fonctionnalité si elle était implementée de manière standarde,
    *  comme le proposent Peter Sommerlad et Andrew L. Sandoval dans ce papier :
    *  http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0052r7.pdf
    */

    Application app; // Création de l'application
    return app.execute(); // Exécution de l'application


    // Fin de la fonction main
}

// Fin du programme C++

// Fin du fichier main.cpp