/**
 * @file CandyCrushHeader.cpp
 * @author Gabriel Gouin
 * @author Matteo Yanni
*/

#include "CandyCrushHeader.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <thread> // Pour sleep_for
#include <chrono> // Pour les unités de temps (ms, s)
using namespace std;

void clearScreen () {
    cout << "\033[H\033[2J";
}

const unsigned KReset   (0);
const unsigned KRouge   (31);
const unsigned KJaune   (33);
const unsigned KCyan    (36);
const unsigned KImpossible(0);
const string KSymboles("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
int unsigned KNbCandies;
int unsigned nbAAlignee;
int unsigned scoreAObtenir;

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

void couleurFond (const unsigned & coul) {
    cout << "\033[" << coul+10 <<"m";
}

typedef vector <unsigned> line; // un type représentant une ligne de la grille
typedef vector <line> mat; // un type représentant la grille
void initGrid (mat & grid, const size_t & matSize) {
    grid.clear();
    for (size_t i(0); i < matSize; ++i) {
        line ligne;
        for (size_t j(0); j < matSize; j++) {
            unsigned valRand (1 + rand() % KNbCandies);
            ligne.emplace_back(valRand);
        }
        grid.emplace_back(ligne);
    }
}
// fonction qui permet d'afficher la grille
void  displayGrid (const mat & grid) {
    clearScreen();
    couleur(KReset);
    couleurFond(KReset);
    cout << " a   ";
    for (unsigned i(0); i < grid.size(); ++i) {
        cout << i << ' ';
    }
    cout << endl << 'o' << endl;
    for (size_t i(0); i < grid.size(); ++i) {
        couleur(KReset);
        cout << i << "    ";
        for (size_t j(0); j < grid.size(); j++) {
            if (!(1 <= grid[i][j] && grid[i][j] <= KNbCandies)) cout << "  ";
            else {
                couleur(31 + grid[i][j]%6);
                cout << KSymboles[grid[i][j]] << ' ';
            }
        }
        cout << endl;
    }
}
// fonction qui permet au jouer de déplacer un chiffre
void makeAMove (mat & grid, const maPosition & pos, const char & direction) {
    if (direction == 'z' && pos.ord != 0) {
        unsigned temp (grid[pos.ord][pos.abs]);
        grid[pos.ord][pos.abs] = grid[pos.ord-1][pos.abs];
        grid[pos.ord-1][pos.abs] = temp;
    }
    if (direction == 'q' && pos.abs != 0) {
        unsigned temp (grid[pos.ord][pos.abs]);
        grid[pos.ord][pos.abs] = grid[pos.ord][pos.abs-1];
        grid[pos.ord][pos.abs-1] = temp;
    }
    if (direction == 's' && pos.ord != grid.size()-1) {
        unsigned temp (grid[pos.ord][pos.abs]);
        grid[pos.ord][pos.abs] = grid[pos.ord+1][pos.abs];
        grid[pos.ord+1][pos.abs] = temp;
    }
    if (direction == 'd' && pos.abs != grid.size()-1) {
        unsigned temp (grid[pos.ord][pos.abs]);
        grid[pos.ord][pos.abs] = grid[pos.ord][pos.abs+1];
        grid[pos.ord][pos.abs+1] = temp;
    }
}
// fonction qui permet de repérer les combots en colonne
bool atLeastThreeInAColumn (const mat & grid, maPosition & pos, unsigned & howMany) {
    bool found = false;
    for (size_t col(0); col < grid.size() && !found; ++col) {
        unsigned count(1);
        for (size_t row(0); row < grid.size()-1 && !found; ++row) {
            if(grid[row][col] != KImpossible && grid[row][col] == grid[row+1][col]) {
                count++;
            } else {
                if (count >= nbAAlignee) {
                    pos.abs = col;
                    pos.ord = row - count+1;
                    howMany = count;
                    found = true;
                }
                count = 1;
            }
        }

        if (!found && count>=nbAAlignee) {
            pos.abs = col;
            pos.ord = grid.size() - count;
            howMany = count;
            found = true;
        }
    }
    return found;
}
// fonction qui permet de repérer les combots en ligne
bool atLeastThreeInARow (const mat & grid, maPosition & pos, unsigned & howMany) {
    bool found = false;
    for (size_t row(0); row < grid.size() && !found; ++row) {
        unsigned count(1);
        for (size_t col(0); col < grid.size()-1 && !found; ++col) {
            if(grid[row][col] != KImpossible && grid[row][col] == grid[row][col+1]) {
                count++;
            } else {
                if (count >= nbAAlignee) {
                    pos.abs = col - count+1;
                    pos.ord = row;
                    howMany = count;
                    found = true;
                }
                count = 1;
            }
        }

        if (!found && count>=nbAAlignee) {
            pos.abs = grid.size() - count;
            pos.ord = row;
            howMany = count;
            found = true;
        }
    }
    return found;
}

// fonction pour gerer les combos en diagonale.
bool atLeastThreeInDiagonal (const mat & grid, maPosition & pos, unsigned & howMany) {
    bool found = false;
    const size_t size = grid.size();

    // ↘ Diagonale haut-gauche a bas-droite
    for (size_t row = 0; row < size && !found; ++row) {
        for (size_t col = 0; col < size && !found; ++col) {
            unsigned count = 1;
            for (size_t i = 0; row + i + 1 < size && col + i + 1 < size && !found; ++i) {
                if (grid[row + i][col + i] != KImpossible && grid[row + i][col + i] == grid[row + i + 1][col + i + 1])
                {count++;} else {
                    if (count >= nbAAlignee) {
                        pos.abs = col;
                        pos.ord = row;
                        howMany = count;
                        found = true;
                    }
                    count = 1;
                }
            }

            if (!found && count >= nbAAlignee) {
                pos.abs = col;
                pos.ord = row;
                howMany = count;
                found = true;
            }
        }
    }

    // ↙ Diagonale haut-droite a bas-gauche
    for (size_t row = 0; row < size && !found; ++row) {
        for (size_t col = size - 1; col < size && !found; --col) {
            unsigned count = 1;
            for (size_t i = 0; row + i + 1 < size && col >= i + 1 && !found; ++i) {
                if (grid[row + i][col - i] != KImpossible &&
                    grid[row + i][col - i] ==
                        grid[row + i + 1][col - i - 1]) {
                    count++;
                } else {
                    if (count >= nbAAlignee) {
                        pos.abs = col;
                        pos.ord = row;
                        howMany = count;
                        found = true;
                    }
                    count = 1;
                }
            }

            if (!found && count >= nbAAlignee) {
                pos.abs = col;
                pos.ord = row;
                howMany = count;
                found = true;
            }

            if (col == 0) break; // évite probleme underflow
        }
    }

    return found;
}


void removalInColumn (mat & grid, const maPosition & pos, unsigned  howMany) {
    for (int row(pos.ord - 1); row >= 0; --row) {
        grid[row + howMany][pos.abs] = grid[row][pos.abs];
    }
    for (size_t row(0); row < howMany; ++row) {
        grid[row][pos.abs] = KImpossible;
    }
}

void removalInRow (mat & grid, const maPosition & pos, unsigned howMany) {
    for (size_t i(0); i < howMany; ++i) {
        maPosition deplacementCol;
        deplacementCol.abs = pos.abs + i;
        deplacementCol.ord = pos.ord;
        removalInColumn(grid, deplacementCol, 1);
    }
}

void removalInDiagonal(mat & grid, const maPosition & position, unsigned howMany) {
    const size_t taille_grille = grid.size();

    // determiner le sens de la diagonale en regardant la case suivante
    bool diagonaleDroite = false;
    if (position.abs + 1 < taille_grille && position.ord + 1 < taille_grille &&
        grid[position.ord + 1][position.abs + 1] == grid[position.ord][position.abs]) {
        diagonaleDroite = true; // ↘
    } else if (position.abs > 0 && position.ord + 1 < taille_grille &&
               grid[position.ord + 1][position.abs - 1] == grid[position.ord][position.abs]) {
        diagonaleDroite = false; // ↙
    }

    // on supprime la diagonale correspondante
    if (diagonaleDroite) {
        for (unsigned i = 0; i < howMany; ++i)
            grid[position.ord + i][position.abs + i] = KImpossible;
    } else {
        for (unsigned i = 0; i < howMany; ++i)
            grid[position.ord + i][position.abs - i] = KImpossible;
    }
}


// fonction qui permet de recharger la grille
void refillGrid(mat & grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        auto it = find(grid[i].begin(), grid[i].end(), KImpossible);

        while (it != grid[i].end()) {
            *it = 1 + rand() % KNbCandies; // *it permet de récupérer l'objet en lui même, alors que it est juste un pointeur vers cet objet.

            it = find(it, grid[i].end(), KImpossible);
        }
    }
}
// fonction qui permet d'enlever tout les combots lorsque la grille se charge initialement afin d'éviter de gagner des points sans rien faire
void removalAllCombos(mat & grid, maPosition & position, unsigned howMany, unsigned & score, unsigned & NbCoups) {
    unsigned nbCombos(0);
    bool combo;
    unsigned valeurAAjouter(0);
    do {
        combo = false;
        if (atLeastThreeInAColumn(grid, position, howMany)) {
            ++nbCombos;
            valeurAAjouter = howMany * grid[position.ord][position.abs] * nbCombos;
            score += valeurAAjouter;
            removalInColumn(grid, position, howMany);
            combo = true;
            displayGrid(grid);
            cout << "Combo x" << nbCombos << endl;
            cout << "+" << valeurAAjouter << " Score :" << score << endl;
            if (nbCombos != 0 && nbCombos%3 == 0) {NbCoups += nbCombos/3; cout << "+ " << nbCombos/3 << " coup(s) !";}
            this_thread::sleep_for(chrono::milliseconds(1000)); // trouvé sur https://en.cppreference.com/w/cpp/thread/sleep_for.html
        }
        if (atLeastThreeInARow(grid, position, howMany)) {
            ++nbCombos;
            valeurAAjouter = howMany * grid[position.ord][position.abs] * nbCombos;
            score += valeurAAjouter;
            removalInRow(grid, position, howMany);
            combo = true;
            displayGrid(grid);
            cout << "Combo x" << nbCombos << endl;
            cout << "+" << valeurAAjouter << " Score :" << score << endl;
            if (nbCombos != 0 && nbCombos%3 == 0) {NbCoups += nbCombos/3; cout << "+ " << nbCombos/3 << " coups !";}
            this_thread::sleep_for(chrono::milliseconds(1000)); //sur internet
        }
        if (atLeastThreeInDiagonal(grid, position, howMany)) {
            ++nbCombos;
            valeurAAjouter = howMany * grid[position.ord][position.abs] * nbCombos;
            score += valeurAAjouter;

            removalInDiagonal(grid, position, howMany);
            combo = true;

            displayGrid(grid);
            cout << "Combo x" << nbCombos << endl;
            cout << "+" << valeurAAjouter << " Score :" << score << endl;

            if (nbCombos != 0 && nbCombos % 3 == 0) {
                NbCoups += nbCombos / 3;
                cout << "+ " << nbCombos / 3 << " coups !";
            }

            this_thread::sleep_for(chrono::milliseconds(1000));
        }
        refillGrid(grid);
    } while (combo); // utilisation de do .. while pour executer la boucle au moins une fois
}

// histoire
// fonction qui permet d'afficher les cinématiques pour le mode histoire
void cinematique(const string & texte, unsigned vitesse) {
    couleur(KJaune);
    for (char c : texte) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(vitesse));
    }
    cout << endl << endl;
    this_thread::sleep_for(chrono::seconds(1));
}
// fonction qui permet d'afficher le boss lors du mode histoire
void afficherBoss(const string & nom) {
    clearScreen();
    couleur(KRouge);
    cout << "========================================" << endl;
    cout << "   BOSS : " << nom << endl;
    cout << "========================================" << endl;
    couleur(KReset);
    this_thread::sleep_for(chrono::seconds(2));
}


