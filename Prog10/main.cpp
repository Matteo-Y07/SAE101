#include <iostream>
#include <stdlib.h>
#include <vector>

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
const unsigned KNbCandies (9);
const unsigned KImpossible(0);

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
    for (size_t i(0); i < grid.size(); ++i) {
        for (size_t j(0); j < grid.size(); j++) {
            if (!(1 <= grid[i][j] && grid[i][j] <= KNbCandies)) cout << "  ";
            else cout << grid[i][j] << ' ';
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
        grid[pos.ord][pos.abs] = grid[pos.ord-1][pos.abs-1];
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
            if(grid[row][col] == grid[row+1][col]) {
                count++;
            } else {
                if (count >= 3) {
                    pos.abs = col;
                    pos.ord = row - count+1;
                    howMany = count;
                    found = true;
                }
                count = 1;
            }
        }

        if (!found && count>=3) {
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
            if(grid[row][col] == grid[row][col+1]) {
                count++;
            } else {
                if (count >= 3) {
                    pos.abs = col - count+1;
                    pos.ord = row;
                    howMany = count;
                    found = true;
                }
                count = 1;
            }
        }

        if (!found && count>=3) {
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

void removalInRow (mat & grid, const maPosition & pos, unsigned  howMany) {
    for (size_t i(0); i < howMany; ++i) {
        maPosition deplacementCol;
        deplacementCol.abs = pos.abs + i;
        deplacementCol.ord = pos.ord;
        removalInColumn(grid, deplacementCol, 1);
    }
}

int main()
{
    srand(time(NULL));

    mat grid;
    initGrid(grid, 3);
    unsigned nbCoups(0);
    unsigned howMany(0);
    while (nbCoups<10) {
        displayGrid(grid);
        cout << "Choisir la position, puis entrez 'z' 'q' 's' ou 'd' selon dans quelle direction vous voulez bouger" << endl;
        maPosition position;
        position.ord=0;
        position.abs=0;
        cout << "entrez abscisse ";
        cin >> position.abs;
        cout << "entrez ordoneee ";
        cin >> position.ord;
        char saisie;
        cin >> saisie;
        if (saisie == 'e') break;
        makeAMove(grid, position, saisie);
            if(atLeastThreeInAColumn(grid, position, howMany)) {
                removalInColumn(grid, position, howMany);
            } else if(atLeastThreeInARow(grid, position, howMany)) {
                removalInRow(grid, position, howMany);
            }
    }
    return 0;
}
