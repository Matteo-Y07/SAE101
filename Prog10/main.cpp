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
    howMany = 0;
    for (size_t i(1); i < grid.size(); ++i) {
        if (grid[i][pos.abs] == grid[i-1][pos.abs]) {
            howMany+=1;
            if (i==grid.size()-1) howMany+=1;
        } else {
            howMany=0;
        }
        if (howMany>=3) found = true;
    }
    return found;
}

int main()
{
    mat grid;
    initGrid(grid, 5);
    displayGrid(grid);
    while (true) {
        unsigned howMany;
        maPosition position;
        cout << "entrez abscisse ";
        cin >> position.abs;
        cout << "entrez ordoneee ";
        cin >> position.ord;
        char saisie;
        cin >> saisie;
        if (saisie == 'e') break;
        makeAMove(grid, position, saisie);
        displayGrid(grid);
        cout << atLeastThreeInAColumn(grid, position, howMany) << endl;
    }
    return 0;
}
