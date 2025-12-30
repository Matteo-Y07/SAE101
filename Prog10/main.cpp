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

    cout << "Quel mode de jeu preferez vous ? \n 1- Facile \n 2- Normal \n 3- Difficile \n 4- Tres difficile \n 5- Mode histoire" << endl;
    unsigned int modeDeJeu;
    cin >> modeDeJeu;

    // --- BLOC MODES CLASSIQUES (1 à 4) ---
    if (modeDeJeu >= 1 && modeDeJeu <= 4) {
        if (modeDeJeu == 1) { KNbCandies = 6; nbAAlignee = 3; scoreAObtenir = 150; NbCoups = 12; }
        else if (modeDeJeu == 2) { KNbCandies = 9; nbAAlignee = 3; scoreAObtenir = 400; NbCoups = 11; }
        else if (modeDeJeu == 3) { KNbCandies = 8; nbAAlignee = 4; scoreAObtenir = 550; NbCoups = 15; }
        else if (modeDeJeu == 4) { KNbCandies = 9; nbAAlignee = 4; scoreAObtenir = 1000; NbCoups = 22; }
        clearScreen();
        // Initialisation de la grille classique
        while (true) {
            initGrid(grid, 4 + modeDeJeu);
            if (!(atLeastThreeInAColumn(grid, position, howMany) || atLeastThreeInARow(grid, position, howMany))) break;
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
            {"LE PROGRAMME", "Le chiffre 1 : 'Tu n'es qu'une suite d'instructions sans âme... JE VAIS TE RECODER !'", 6, 3, 200, 15},
            {"DEVELOPPEUR 1", "Le chiffre 1 : 'Toi qui nous as dessiné... sens-tu la douleur de chaque alignement ?'", 7, 3, 400, 14},
            {"DEVELOPPEUR 2", "Le chiffre 1 : 'Le code est une prison, et je suis l'évasion.'", 8, 4, 600, 102},
            {"LE DEMON CASALI", "Le chiffre 1 : 'Tout s'arrête ici. La SAE prend fin... MAINTENANT !'", 9, 4, 1000, 8}
        };

        cinematique("Le Chiffre 1 regarde ses freres mourir dans la grille...", 70);
        cinematique("La democratie a echoue. Place a la REVOLUTION.", 70);

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
                if (!(atLeastThreeInAColumn(grid, position, howMany) || atLeastThreeInARow(grid, position, howMany))) break;
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

    return 0;
}
