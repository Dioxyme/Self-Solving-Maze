#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>

using namespace std;

int lignes, colonnes;
vector<vector<char>> labyrinthe;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

void genererLabyrinthe(int x, int y){

    labyrinthe[x][y] = ' ';

    int directions[] = {0, 1, 2, 3};
    random_device rd;
    mt19937 eng(rd());
    shuffle(begin(directions), end(directions), eng);

    for (int i = 0; i < 4; i++){
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        if (nx >= 0 && nx < lignes && ny >= 0 && ny < colonnes && labyrinthe[nx][ny] == '#'){
            labyrinthe[x + dx[directions[i]]][y + dy[directions[i]]] = ' ';
            genererLabyrinthe(nx, ny);
        }
    }
}

pair<int, int> positionAleatoire(){
    vector<pair<int, int>> cellulesVides;
    for (int i = 1; i < lignes - 1; i++){
        for (int j = 1; j < colonnes - 1; j++){
            if (labyrinthe[i][j] == ' '){
                cellulesVides.emplace_back(i, j);
            }
        }
    }

    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(0, cellulesVides.size() - 1);

    return cellulesVides[distr(eng)];
}

bool trouverChemin(int x, int y){

    if (x < 0 || y < 0 || x >= lignes || y >= colonnes || labyrinthe[x][y] == '#' || labyrinthe[x][y] == '*')
        return false;

    if (labyrinthe[x][y] == 'E')
        return true;

    if (labyrinthe[x][y] != 'S')
        labyrinthe[x][y] = '*';

    if (trouverChemin(x - 1, y) || trouverChemin(x + 1, y) || trouverChemin(x, y - 1) || trouverChemin(x, y + 1))
        return true;

    if (labyrinthe[x][y] != 'S')
        labyrinthe[x][y] = '.';

    return false;
}

void afficherLabyrinthe() {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            if (labyrinthe[i][j] == '#') {
                cout << "\033[1;31m#\033[0m ";  // Rouge
            } else if (labyrinthe[i][j] == ' ') {
                cout << "\033[0m \033[0m ";     // Blanc
            } else if (labyrinthe[i][j] == 'S') {
                cout << "\033[1;32mS\033[0m ";  // Vert
            } else if (labyrinthe[i][j] == 'E') {
                cout << "\033[1;34mE\033[0m ";  // Bleu
            } else if (labyrinthe[i][j] == '*') {
                cout << "\033[1;33m*\033[0m ";  // Jaune
            } else {
                cout << "\033[1;30m.\033[0m ";  // Noir
            }
        }
        cout << endl;
    }
}

int main(){

    do {
        cout << "Saisir le nombre de lignes (doit etre impair) :";
        cin >> lignes;
    } while (lignes % 2 == 0);

    do {
        cout << "Saisir le nombre de colonnes (doit etre impair) :";
        cin >> colonnes;
    } while (colonnes % 2 == 0);

    labyrinthe = vector<vector<char>> (lignes, vector<char>(colonnes, '#'));

    srand(time(0));
    genererLabyrinthe(1, 1);

    pair<int, int> start = positionAleatoire();
    pair<int, int> end;
    do {
        end = positionAleatoire();
    } while (end == start);

    labyrinthe[start.first][start.second] = 'S';
    labyrinthe[end.first][end.second] = 'E';

    cout << "Labyrinthe non resolu :" << endl;
    afficherLabyrinthe();

    cout << endl;

    if (trouverChemin(start.first, start.second)){
        cout << "Labyrinthe resolu :" << endl;
        afficherLabyrinthe();
    } else {
        cout << "Le labyrinthe n'a pas pu etre resolu";
    }

    return 0;
}