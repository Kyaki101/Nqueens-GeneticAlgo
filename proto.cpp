#include <bits/stdc++.h>

typedef std::vector<int> vec;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

int n;
long long popu;

typedef struct individual {
    int score;
    vec board;

    // Constructor
    individual(const vec& b, int scr) : board(b), score(scr) {}
}individual;

typedef std::vector<individual> mat;

mat storage;
mat nextGen;


//Compares for ordering
bool comp(individual & a, individual & b){
    return a.score < b.score;
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
}

void printDna(individual & indi){
    for(int i = 0; i < n; i++){
        std::cout << indi.board[i] << " ";
    }
    std::cout << '\n';
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
    ind.score = counter;
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
    int best = (1 << 30);
    for(int i = 0; i < popu; i++){
        if(storage[i].score <= best){
            best = storage[i].score;
            index = i;
        }
    }
    return index;
}

int main(){
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cin >> n;
    popu = n * 3;
    storage.assign(popu, individual(vec(n, 0), 0));
    for(int i = 0; i < popu; i++){
        generational(storage[i]);
        genScore(storage[i]);
    }
    int gens = 1000;
    std::uniform_int_distribution<int> pickTop(0, popu - 2);
    individual best = storage[findBest()];
    printDna(best);
    for(int g = 0; g < gens; g++){
        nextGen.assign(popu, individual(vec(n, 0), 0));
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
        std::cout << "GEN " << g << '\n';
        best = storage[findBest()];
        printDna(best);
        // for(individual indi : storage){
        //     for(int i : indi.board){
        //         std::cout << i << " ";
        //     }
        //     std::cout << std::endl;
        // }
    }


    return 0;
}