# include <bits/stdc++.h>
# include <climits>
// #include <ctime>
using namespace std;
# define for_range(i,a,b) for(int i = a; i < b; ++i)
# define rep(i,a,b) for(int i = a; i <= b; ++i)
//# define invrep(i,b,a) for(int i = b; i >= a; --i)
// # define print(x) copy(x.begin(), x.end(), ostream_iterator<int>(cout, “”)), cout << endl
//typedef pair<int, int> par;
//typedef tuple<int, int, int> triple;
//typedef vector<int> vi;
//typedef vector<par> vp;
//typedef vector<triple> vt;
//typedef vector<vi> matrix;

/*
int get_elem_i(int i, triple & t) {
    switch (i) {
        case 0: return get<0>(t);
        case 1: return get<1>(t);
        case 2: return get<2>(t);
    }
    assert(false);
}
*/

unsigned long int N;

unsigned long long int memo[502][502];
unsigned long long int sizes[502];
unsigned long long int current_min;
unsigned long long int value;

int main() {
    // As seen at: https://es.wikibooks.org/wiki/Optimizaci%C3%B3n_del_Producto_de_Matrices
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> N;
    for_range(i, 0, N) cin >> sizes[i];
    rep(i, 1, N) cin >> memo[i][i];
    
    for_range(diagonal, 1, N) {
        for_range(row, 1, N - diagonal) {
            current_min = ULLONG_MAX;
            for_range(k, row, row + diagonal) {
                value = memo[row][k] + memo[k + 1][row + diagonal] 
                    + (sizes[row - 1] * sizes[k] * sizes[row + diagonal]);
                if (value < current_min) current_min = value;
            }
            memo[row][row + diagonal] = current_min;
        }
    }
    
    cout << memo[1][N-1] << endl;
    return 0;
}
