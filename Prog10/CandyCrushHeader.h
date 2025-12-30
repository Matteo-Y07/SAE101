#ifndef CANDYCRUSHHEADER_H
#define CANDYCRUSHHEADER_H

#include <vector>
#include <string>

// Types et Structures
typedef std::vector<unsigned> line;
typedef std::vector<line> mat;

struct maPosition {
    unsigned abs;
    unsigned ord;
};

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
void clearScreen();
void couleur(const unsigned & coul);
void initGrid(mat & grid, const size_t & matSize);
void displayGrid(const mat & grid);
void makeAMove(mat & grid, const maPosition & pos, const char & direction);
bool atLeastThreeInAColumn(const mat & grid, maPosition & pos, unsigned & howMany);
bool atLeastThreeInARow(const mat & grid, maPosition & pos, unsigned & howMany);
void removalInColumn(mat & grid, const maPosition & pos, unsigned howMany);
void removalInRow(mat & grid, const maPosition & pos, unsigned howMany);
void refillGrid(mat & grid);
void removalAllCombos(mat & grid, maPosition & position, unsigned howMany, unsigned & score, unsigned & NbCoups);
void cinematique(const std::string & texte, unsigned vitesse = 50);
void afficherBoss(const std::string & nom);

#endif // CANDYCRUSHHEADER_H
