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

    cout << "Quel mode de jeu preferez vous ? \n 1- Facile \n 2- Normal \n 3- Difficile \n 4- Tres difficile \n 5- Mode histoire \n 6- Extrême \n 7- Mode histoire 2" << endl;
    unsigned int modeDeJeu;
    cin >> modeDeJeu;

    // --- BLOC MODES CLASSIQUES (1 à 4) ---
    if (modeDeJeu >= 1 && modeDeJeu <= 4 or modeDeJeu == 6) {
        if (modeDeJeu == 1) { KNbCandies = 6; nbAAlignee = 3; scoreAObtenir = 50; NbCoups = 12; }
        else if (modeDeJeu == 2) { KNbCandies = 9; nbAAlignee = 3; scoreAObtenir = 100; NbCoups = 11; }
        else if (modeDeJeu == 3) { KNbCandies = 8; nbAAlignee = 4; scoreAObtenir = 200; NbCoups = 15; }
        else if (modeDeJeu == 4) { KNbCandies = 9; nbAAlignee = 4; scoreAObtenir = 400; NbCoups = 22; }
        else if (modeDeJeu == 6) { KNbCandies = 15; nbAAlignee = 4; scoreAObtenir = 1000; NbCoups = 50; }
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
    // BLOC MODE HISTOIRE 2
    else if (modeDeJeu == 7) {
        clearScreen();
        vector<EtapeHistoire> campagne = {
            {"LE NOVICE ENRAGÉ", "Jeune Garçon tu es donc mon premier adversaire dans ce tournoi. \n Je ne compte pas m'arrêtez là, prépare ta défaite.", 6, 3, 200, 15},
            {"LE JOUEUR TRISTE", "JAMAIS tu ne me battras. Je suis ici dans le but de remportez le prix. \n Et gagner assez d'argent afin de nourrir ma famille \n Je ne peut donc pas te laisser gagner...", 7, 3, 400, 14},
            {"L'EXPERT FOU", "Tu ferais mieux de me laisser gagner. \n Je n'ai jamais perdu et seul dieu sait ce que je pourrais te faire si jamais je perd...", 8, 4, 600, 102},
            {"L'IMMORTEL", "Cela fait bientôt 2 millénaires que je règne sur ce jeu. \n Nul ne m'a battu et nul ne me battra avant que cet univers prenne fin. \n Sache que tout les joueurs qui m'ont défié sont ressorti fou de l'affrontement.", 9, 4, 1000, 8}
        };

        cinematique("Dans petit village, un jeune garçon dont le nom était sans importance mais rempli d'une ambition dévorante se leva. \n Il avait décidé de devenir quelqu'un. ", 100);
        cinematique("Et quoi de mieux pour obtenir reconnaissance et notoriété que de devenir le meilleur joueur du Jeu. \n Il y avait toujours été doué et donc ce décida de partir en défier les plus célèbres représentants lors du grand tournoi.", 100);

        for (const auto& etape : campagne) {
            afficherBoss(etape.nomBoss);
            cinematique(etape.dialogue, 100);

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
                cinematique("Victoire !" + etape.nomBoss + "est battu, un pas de plus vers la notoriété.", 100);
                score += scoreNiveau;
            } else {
                couleur(KRouge);
                cout << "Le jeune garçon ne devint jamais celui qu'il espérait être... GAME OVER." << endl;
                return 0;
            }
        }
        clearScreen();
        cinematique("Le jeune garçon devint alors une légende et son nom fût connu de tout le pays et même au dela", 100);
        cout << "SCORE FINAL DU JEUNE GARCON : " << score << endl;
    }

    return 0;
}
