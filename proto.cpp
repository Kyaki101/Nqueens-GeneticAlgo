#include <bits/stdc++.h>

typedef std::vector<int> vec;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

int n, k;
long long popu;

typedef struct individual {
    double score;
    vec board;
    int colision;

    // Constructor
    individual(const vec& b, double scr, int cols) : board(b), score(scr), colision(cols) {}
    bool operator==(const individual & other) const {
        return (board == other.board);
    }
}individual;

typedef std::vector<individual> mat;

mat storage;
mat nextGen;


//Compares for ordering
bool comp(individual & a, individual & b){
    return a.score < b.score;
}

void mutate(individual & child, int index){
    std::uniform_int_distribution<int> distribution(0, n - 1);
    int num = distribution(generator);
    child.board[index] = num;
}

//Generates crossovers for new gens
void crossover(individual & a, individual & b, individual & newChild){
    std::uniform_int_distribution<int> distribution(0, 10);
    if(a.score <= b.score){
        for(int i = 0; i < n; i++){
            int num = distribution(generator);
            if(num <= 7)newChild.board[i] = a.board[i];
            else newChild.board[i] = b.board[i];
        }
    }
    else{
        for(int i = 0; i < n; i++){
            int num = distribution(generator);
            if(num <= 7)newChild.board[i] = b.board[i];
            else newChild.board[i] = a.board[i];
        }
    }
    std::uniform_int_distribution<int> mut(1, 100);
    for(int i = 0; i < n; i++){
        int num = mut(generator);
        if(num <= 5)mutate(newChild, i);
    }
}

void printNQueensBoard(individual & indi) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (indi.board[i] == j) {
                std::cout << "Q ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
}

void printDna(individual & indi){
    std::cout << "ELITE--------------------\n";
    std::cout << "DNA: \n";
    printNQueensBoard(indi);
    std::cout << '\n';
    std::cout << "Fittness: " << indi.score << '\n';
    std::cout << "Colisiones: " << indi.colision << '\n';
    std::cout << "-------------------------\n";
}
double inverseExponentialFunction(int x) {
    double base = 2.0;
    double exponent = -0.1;

    return std::pow(base, x * exponent);
}

//Generates the score for each individual
void genScore(individual & ind){
    int counter = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (ind.board[i] == ind.board[j] || abs(ind.board[i] - ind.board[j]) == abs(i - j)) {
                counter ++;
            }
        }
    }
    ind.score = inverseExponentialFunction(counter);
    ind.colision = counter;
}

//fills the first generation
void generational(individual & indi){
    std::uniform_int_distribution<int> distribution(0, n - 1);
    for(int i = 0; i < n; i++){
        int randNum = distribution(generator);
        indi.board[i] = randNum;
    }
}

//finds the individual with the least amount of collissions
int findBest(){
    int index;
    double best = -(1 << 30);
    for(int i = 0; i < popu; i++){
        if(storage[i].score >= best){
            best = storage[i].score;
            index = i;
        }
    }
    return index;
}

int main(){
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cin >> n >> k;
    popu = n * 3;
    storage.assign(popu, individual(vec(n, 0), 0, 0));
    for(int i = 0; i < popu; i++){
        generational(storage[i]);
        genScore(storage[i]);
    }
    std::uniform_int_distribution<int> pickTop(0, popu / 2);
    individual best = storage[findBest()];
    if(k) {std::cout << "GEN " << 0 << '\n'; printDna(best);}
    int g = 0;
    int repeated = 0;
    while(true){
        // std::sort(storage.begin(), storage.end(), comp);
        nextGen.assign(popu, individual(vec(n, 0), 0, 0));
        nextGen[0] = best;
        int i = 1;
        while(i < popu){
            int a = pickTop(generator);
            int b = pickTop(generator);
            if(a == b) a++;
            crossover(storage[a], storage[b], nextGen[i]);
            genScore(nextGen[i]);
            i++;
        }
        storage = nextGen;
        individual newBest = storage[findBest()];
        if(newBest == best){repeated ++;}
        else{best = newBest; repeated = 0;}
        if(k) {std::cout << "GEN " << g + 1 << '\n'; printDna(best);}
        if(best.colision == 0){
            std::cout << "Correct Result: \n";
            printDna(best);
            std::cout << "gen: " << g << '\n';
            break;
        }
        else if(repeated >= n * n){
            std::cout << "Forced Result: \n";
            printDna(best);
            std::cout << "gen: " << g << '\n';
            break;
        }
        // for(individual indi : storage){
        //     for(int i : indi.board){
        //         std::cout << i << " ";
        //     }
        //     std::cout << std::endl;
        // }
        g++;
    }


    return 0;
}