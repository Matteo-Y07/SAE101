/**
 * @file CandyCrushHeader.h
 * @author Gabriel Gouin
 * @author Matteo Yanni
 * @brief fichier contenant la déclaration des struct et fonctions
*/

#ifndef CANDYCRUSHHEADER_H
#define CANDYCRUSHHEADER_H

#include <vector>
#include <string>

// Types et Structures
typedef std::vector<unsigned> line;
typedef std::vector<line> mat;

/**
 * @struct maPosition
 * @brief Représente les coordonnées dans la grille
 */
struct maPosition {
    unsigned abs;
    unsigned ord;
};

/**
 * @struct EtapeHistoire
 * @brief contient les paramètres et le scénario du niveau du mode histoire
 */
struct EtapeHistoire {
    std::string nomBoss;
    std::string dialogue;
    unsigned nbCandies;
    unsigned alignementRequis;
    unsigned scoreCible;
    unsigned coupsMax;
};

// Constantes (déclarées "extern" pour être partagées)
extern const unsigned KReset, KRouge, KJaune, KCyan;
extern unsigned KNbCandies, nbAAlignee, scoreAObtenir;

// Prototypes des fonctions
/**
 * @brief permet d'effacer le contenu du terminal
 */
void clearScreen();
/**
 * @brief permet d'attribuer une couleur spécifique à chaque chiffre
 */
void couleur(const unsigned & coul);
/**
 * @brief permet d'initialiser la grille
 */
void initGrid(mat & grid, const size_t & matSize);
/**
 * @brief permet d'afficher la grille
 */
void displayGrid(const mat & grid);
/**
 * @brief permet au joueur d'effectuer un déplacement
 */
void makeAMove(mat & grid, const maPosition & pos, const char & direction);
/**
 * @brief permet de trouver les combos en colonne
 */
bool atLeastThreeInAColumn(const mat & grid, maPosition & pos, unsigned & howMany);
/**
 * @brief permet de trouver les combos en ligne
 */
bool atLeastThreeInARow(const mat & grid, maPosition & pos, unsigned & howMany);
/**
 * @brief permet de trouver les combos en diagonale
 */
bool atLeastThreeInDiagonal (const mat & grid, maPosition & pos, unsigned & howMany, bool & diagDroite);
/**
 * @brief permet de gérer la suppression de chiffre verticalement
 */
void removalInColumn(mat & grid, const maPosition & pos, unsigned howMany);
/**
 * @brief permet de gérer la suppression de chiffre horizontalement
 */
void removalInRow(mat & grid, const maPosition & pos, unsigned howMany);
/**
 * @brief permet de gérer la suppression de chiffre en diagonale
 */
void removalInDiagonal (mat & grid, const maPosition & pos, unsigned howMany, unsigned & score);
/**
 * @brief permet de recharger la grille
 */
void refillGrid(mat & grid);
/**
 * @brief permet d'enlever tous les combos initialement afin d'éviter que le joueur gagne des points sans avoir jouer
 */
void removalAllCombos(mat & grid, maPosition & position, unsigned howMany, unsigned & score, unsigned & NbCoups);
/**
 * @brief permet d'afficher une cinématique
 */
void cinematique(const std::string & texte, unsigned vitesse = 50);
/**
 * @brief permet d'afficher un boss
 */
void afficherBoss(const std::string & nom);


#endif // CANDYCRUSHHEADER_H





