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
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);
const unsigned KImpossible(0);
const string KSymboles("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
int unsigned KNbCandies;
int unsigned nbAAlignee;

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

void couleurFond (const unsigned & coul) {
    cout << "\033[" << coul+10 <<"m";
}

typedef vector <unsigned> line; // un type représentant une ligne de la grille
typedef vector <line> mat; // un type représentant la grille
struct maPosition {
    unsigned abs;
    unsigned ord;
};

void initGrid (mat & grid, const size_t & matSize) {
    for (size_t i(0); i < matSize; ++i) {
        line ligne;
        for (size_t j(0); j < matSize; j++) {
            unsigned valRand (1 + rand() % KNbCandies);
            ligne.emplace_back(valRand);
        }
        grid.emplace_back(ligne);
    }
}

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

void refillGrid(mat & grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        auto it = find(grid[i].begin(), grid[i].end(), KImpossible);

        while (it != grid[i].end()) {
            *it = 1 + rand() % KNbCandies; // *it permet de récupérer l'objet en lui même, alors que it est juste un pointeur vers cet objet.

            it = find(it, grid[i].end(), KImpossible);
        }
    }
}

void removalAllCombos(mat & grid, maPosition & position, unsigned howMany, unsigned & score) {
    unsigned nbCombos(0);
    bool combo;
    do {
        combo = false;
        if (atLeastThreeInAColumn(grid, position, howMany)) {
            ++nbCombos;
            score += howMany * grid[position.ord][position.abs] * nbCombos;
            removalInColumn(grid, position, howMany);
            combo = true;
        }
        if (atLeastThreeInARow(grid, position, howMany)) {
            ++nbCombos;
            score += howMany * grid[position.ord][position.abs] * nbCombos;
            removalInRow(grid, position, howMany);
            combo = true;
        }
        displayGrid(grid);
        cout << "Combo x" << nbCombos << endl;
        this_thread::sleep_for(chrono::milliseconds(500)); //sur internet : gemini (trouver lien vers un vrai site plutot que IA)
        refillGrid(grid);
    } while (combo);
}

int main() {
    srand(time(NULL));
    mat grid;
    unsigned int modeDeJeu;
    maPosition position;
    cout << "Quel mode de jeu préférez vous ? facile(1), normal(2), difficile(3), très difficile(4):" << endl;
    cin >> modeDeJeu;
    if (modeDeJeu==1){KNbCandies = 6; nbAAlignee = 3;}
    else if (modeDeJeu==2){KNbCandies = 9; nbAAlignee = 3;}
    else if (modeDeJeu==3){KNbCandies = 8; nbAAlignee = 4;}
    else if (modeDeJeu==4){KNbCandies = 9; nbAAlignee = 3;}
    unsigned howMany;
    initGrid(grid, 5); //Permet de s'assurer de créer une grille qui ne contient pas initialement de combo
    unsigned nbCoups(0), score(0);
    removalAllCombos(grid, position, howMany, score);

    while (nbCoups < 10) {
        displayGrid(grid);
        couleur(KReset);
        cout << "Coups restants : " << 10 - nbCoups << " | SCORE : " << score << endl;
        cout << "Saisir : abs ord direction (ex: 2 3 z) : ";
        char saisie;
        cin >> position.abs >> position.ord >> saisie;
        if (saisie == 'e') break;
        makeAMove(grid, position, saisie);
        nbCoups++;

        // On traite les alignements tant qu'il y en a
        removalAllCombos(grid, position, howMany, score);
    }
    return 0;
}
