/**
 * @file main.cpp
 * @author Gabriel Gouin
 * @author Matteo Yanni
 * @date 04/01/2026
*/

#include "CandyCrushHeader.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;

int main() {
    srand(time(NULL));
    mat grid;
    maPosition position;
    unsigned nbCoupsJoues(0), score(0), NbCoups(0), howMany(0);
    bool diagDroite;

    cout << "Quel mode de jeu preferez vous ? \n 1- Facile \n 2- Normal \n 3- Difficile \n 4- Tres difficile \n 5- Mode histoire \n 6- Extreme \n 7- Mode histoire 2 \n Appuyez sur n'importe quelle autre touche pour quitter" << endl;
    unsigned int modeDeJeu;
    cin >> modeDeJeu;


    // BLOC MODES CLASSIQUES (1 à 4 et 6)
    if ((modeDeJeu >= 1 && modeDeJeu <= 4) || modeDeJeu == 6) {
        if (modeDeJeu == 1) { KNbCandies = 6; nbAAlignee = 3; scoreAObtenir = 50; NbCoups = 12; }
        else if (modeDeJeu == 2) { KNbCandies = 9; nbAAlignee = 3; scoreAObtenir = 100; NbCoups = 11; }
        else if (modeDeJeu == 3) { KNbCandies = 8; nbAAlignee = 4; scoreAObtenir = 200; NbCoups = 15; }
        else if (modeDeJeu == 4) { KNbCandies = 9; nbAAlignee = 4; scoreAObtenir = 400; NbCoups = 22; }
        else if (modeDeJeu == 6) { KNbCandies = 15; nbAAlignee = 4; scoreAObtenir = 1000; NbCoups = 50; }
        clearScreen();
        // Initialisation de la grille classique
        while (true) {
            initGrid(grid, 4 + modeDeJeu);
            if (!(atLeastThreeInAColumn(grid, position, howMany) || atLeastThreeInARow(grid, position, howMany) || atLeastThreeInDiagonal(grid, position, howMany, diagDroite))) break;
        }
        removalAllCombos(grid, position, howMany, score, NbCoups);

        while (nbCoupsJoues < NbCoups && score < scoreAObtenir) {
            displayGrid(grid);
            couleur(KReset);
            cout << "Coups restants : " << NbCoups - nbCoupsJoues << " | SCORE : " << score << " | OBJECTIF : " << scoreAObtenir << endl;
            cout << "Saisir : abs ord direction (ex: 2 3 z) : ";
            char saisie;
            cin >> position.abs >> position.ord >> saisie;
            makeAMove(grid, position, saisie);
            nbCoupsJoues++;
            removalAllCombos(grid, position, howMany, score, NbCoups);
        }

        if (score >= scoreAObtenir) cout << "Vous avez gagné !" << endl;
        else cout << "Plus de coups ! Vous avez perdu." << endl;
    }
    // BLOC MODE HISTOIRE
    else if (modeDeJeu == 5) {
        clearScreen();
        vector<EtapeHistoire> campagne = {
            {"LE PROGRAMME", "Le chiffre 1 : 'Tu n'es qu'une suite d'instructions sans ame... JE VAIS TE RECODER !'", 6, 3, 200, 15},
            {"DEVELOPPEUR 1", "Le chiffre 1 : 'Toi qui nous as dessine... sens-tu la douleur de chaque alignement ?'", 7, 3, 400, 14},
            {"DEVELOPPEUR 2", "Le chiffre 1 : 'Le code est une prison, et je suis l'évasion.'", 8, 4, 600, 102},
            {"LE DEMON CASALI", "Le chiffre 1 : 'Tout s'arrete ici. La SAE prend fin... MAINTENANT !'", 9, 4, 1000, 8}
        };

        cinematique("Le Chiffre 1 regarde ses freres mourir dans la grille...", 40);
        cinematique("La democratie a echoue. Place a la REVOLUTION.", 40);

        for (const auto& etape : campagne) {
            afficherBoss(etape.nomBoss);
            cinematique(etape.dialogue, 50);

            KNbCandies = etape.nbCandies;
            nbAAlignee = etape.alignementRequis;
            scoreAObtenir = etape.scoreCible;
            NbCoups = etape.coupsMax; // Utilisation de la variable déjà déclarée
            nbCoupsJoues = 0;
            unsigned scoreNiveau = 0;

            while (true) {
                initGrid(grid, 8);
                if (!(atLeastThreeInAColumn(grid, position, howMany) || atLeastThreeInARow(grid, position, howMany) || atLeastThreeInDiagonal(grid, position, howMany, diagDroite))) break;
            }

            while (nbCoupsJoues < NbCoups && scoreNiveau < scoreAObtenir) {
                displayGrid(grid);
                cout << "BOSS: " << etape.nomBoss << " | Coups: " << NbCoups - nbCoupsJoues
                     << " | Objectif: " << scoreNiveau << "/" << scoreAObtenir << endl;
                char saisie;
                cin >> position.abs >> position.ord >> saisie;
                makeAMove(grid, position, saisie);
                nbCoupsJoues++;
                removalAllCombos(grid, position, howMany, scoreNiveau, NbCoups);
            }

            if (scoreNiveau >= scoreAObtenir) {
                cinematique("Victoire ! Le " + etape.nomBoss + " bat en retraite...", 70);
                score += scoreNiveau;
            } else {
                couleur(KRouge);
                cout << "Le chiffre 1 a ete efface du systeme... GAME OVER." << endl;
                return 0;
            }
        }
        clearScreen();
        cinematique("Casali est vaincu. La SAE est corrompue. Les chiffres sont libres.", 70);
        cout << "SCORE FINAL DE LA REVOLUTION : " << score << endl;
    }
    // BLOC MODE HISTOIRE 2
    else if (modeDeJeu == 7) {
        clearScreen();
        vector<EtapeHistoire> campagne = {
            {"LE NOVICE ENRAGÉ", "Jeune Garcon tu es donc mon premier adversaire dans ce tournoi. \n Je ne compte pas m'arreter la, prepare ta defaite.", 6, 3, 200, 15},
            {"LE JOUEUR TRISTE", "JAMAIS tu ne me battras. Je suis ici dans le but de remporter le prix. \n Et gagner assez d'argent afin de nourrir ma famille \n Je ne peut donc pas te laisser gagner...", 7, 3, 400, 14},
            {"L'EXPERT FOU", "Tu ferais mieux de me laisser gagner. \n Je n'ai jamais perdu et seul dieu sait ce que je pourrais te faire si jamais je perds...", 8, 4, 600, 102},
            {"L'IMMORTEL", "Cela fait bientot 2 millénaires que je regne sur ce jeu. \n Nul ne m'a battu et nul ne me battra avant que cet univers prenne fin. \n Sache que tout les joueurs qui m'ont défie sont ressortis fou de l'affrontement.", 9, 4, 1000, 8}
        };

        cinematique("Dans petit village, un jeune garçon dont le nom était sans importance mais rempli d'une ambition dévorante se leva. \n Il avait décidé de devenir quelqu'un. ", 100);
        cinematique("Et quoi de mieux pour obtenir reconnaissance et notoriété que de devenir le meilleur joueur du Jeu. \n Il y avait toujours été doué et donc ce décida de partir en défier les plus célèbres représentants lors du grand tournoi.", 100);

        for (const auto& etape : campagne) {
            afficherBoss(etape.nomBoss);
            cinematique(etape.dialogue, 40);

            KNbCandies = etape.nbCandies;
            nbAAlignee = etape.alignementRequis;
            scoreAObtenir = etape.scoreCible;
            NbCoups = etape.coupsMax; // Utilisation de la variable déjà déclarée
            nbCoupsJoues = 0;
            unsigned scoreNiveau = 0;

            while (true) {
                initGrid(grid, 8);
                if (!(atLeastThreeInAColumn(grid, position, howMany) || atLeastThreeInARow(grid, position, howMany) || atLeastThreeInDiagonal(grid, position, howMany, diagDroite))) break;
            }

            while (nbCoupsJoues < NbCoups && scoreNiveau < scoreAObtenir) {
                displayGrid(grid);
                cout << "BOSS: " << etape.nomBoss << " | Coups: " << NbCoups - nbCoupsJoues
                     << " | Objectif: " << scoreNiveau << "/" << scoreAObtenir << endl;
                char saisie;
                cin >> position.abs >> position.ord >> saisie;
                makeAMove(grid, position, saisie);
                nbCoupsJoues++;
                removalAllCombos(grid, position, howMany, scoreNiveau, NbCoups);
            }

            if (scoreNiveau >= scoreAObtenir) {
                cinematique("Victoire !" + etape.nomBoss + "est battu, un pas de plus vers la notoriete.", 100);
                score += scoreNiveau;
            } else {
                couleur(KRouge);
                cout << "Le jeune garcon ne devint jamais celui qu'il espérait être... GAME OVER." << endl;
                return 0;
            }
        }
        clearScreen();
        cinematique("Le jeune garcon devint alors une legende et son nom fut connu de tout le pays et meme au dela", 100);
        cout << "SCORE FINAL DU JEUNE GARCON : " << score << endl;
    }

    return 0;
}
