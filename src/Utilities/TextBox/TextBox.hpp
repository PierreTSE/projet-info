#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#include <string>

/**
* \brief Récupère une chaîne de caractères depuis une entrée
* \param str Titre de la fenêtre
* \return Chaîne de caractères récupérée
*/
std::string getTextFromBox(std::string str);

/**
 * \brief implémentation Windows de popen
 */
std::string pOpen(std::string);

#endif // TEXTBOX_HPP
