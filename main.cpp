#include <random>
#include <unordered_map>

const unsigned int width = 100;
const unsigned int height = 100;
const unsigned int ncells = width * height;
const float initialdensity = 0.5;
const float prob = 0.028;
const int ntry = 1;
unsigned int livingcells = 0;
const int maxiter = 10000;
bool permutation = false;

//Set a cell as alive and increment the count of neighbours cell alive of the neighbours
void SetCell(unsigned int x, unsigned int y, unsigned char* life){
    *(life+x*width+y) |= 0x01;
    int yright, yleft, xabove, xbelow;
    if (y == 0){
        yleft = width-1;
        yright = 1;
    }else if(y == width -1){
        yleft = -1;
        yright = -(width-1);
    }else{
        yleft = -1;
        yright = 1;
    }
    if (x==0){
        xabove = ncells - width;
        xbelow = width;
    }else if (x == height-1){
        xabove = -width;
        xbelow = -(ncells - width);
    }else{
        xabove = -width;
        xbelow = width;
    }
    *(life+x*width+y+xabove+yleft) += 0x02;
    *(life+x*width+y+yleft) += 0x02;
    *(life+x*width+y+yleft+xbelow) += 0x02;
    *(life+x*width+y+xbelow) += 0x02;
    *(life+x*width+y+yright+xbelow) += 0x02;
    *(life+x*width+y+yright) += 0x02;
    *(life+x*width+y+yright+xabove) += 0x02;
    *(life+x*width+y+xabove) += 0x02;
    livingcells++;
}

//Set a cell as dead and decrement the count of neighbours cell alive of the neighbours
void ClearCell(unsigned int x, unsigned int y, unsigned char* life){
    *(life+x*width+y) &= ~0x01;
    int yright, yleft, xabove, xbelow;
    if (y == 0){
        yleft = width-1;
        yright = 1;
    }else if(y == width -1){
        yleft = -1;
        yright = -(width-1);
    }else{
        yleft = -1;
        yright = 1;
    }
    if (x==0){
        xabove = ncells - width;
        xbelow = width;
    }else if (x == height-1){
        xabove = -width;
        xbelow = -(ncells - width);
    }else{
        xabove = -width;
        xbelow = width;
    }
    *(life+x*width+y+xabove+yleft) -= 0x02;
    *(life+x*width+y+yleft) -= 0x02;
    *(life+x*width+y+yleft+xbelow) -= 0x02;
    *(life+x*width+y+xbelow) -= 0x02;
    *(life+x*width+y+yright+xbelow) -= 0x02;
    *(life+x*width+y+yright) -= 0x02;
    *(life+x*width+y+yright+xabove) -= 0x02;
    *(life+x*width+y+xabove) -= 0x02;
    livingcells--;
}

//Plot the game in the command line
void Draw(unsigned char* life){
    for (unsigned int i = 0; i < height; ++i) {
        for(unsigned int j = 0; j < width; ++j){
            if ((*(life+j+i*width)&0x01)==0){
                printf("o\t");
            }else{
                printf("*\t");
            }
        }
        printf("\n");
    }
}

//Next iteration of the base game of life
void NextGeneration(unsigned char* life){
    unsigned char* tmp;
    tmp = (unsigned char*)malloc(ncells * sizeof(unsigned char));
    memcpy(tmp, life, ncells * sizeof(unsigned char));
    int i=0, j=0, k=0;
    while(k < ncells) {
        while(*(tmp)==0 && k < ncells - 1){//Dead and no neighbours
            k++;
            tmp++;
            j++;
            if (j%width==0 && j != 0){
                j=0;
                i++;
            }
        }
        if (((*(tmp))&0x01)==0){//Dead but possibly to born
            if ((*(tmp))>>1==3){
                SetCell(i, j, life);
            }
        }else{//Alive
            if ((*(tmp))>>1 != 2 && (*(tmp))>>1 != 3){
                ClearCell(i, j, life);
            }
        }
        tmp++;
        k++;
        j++;
        if (j%width==0 && j != 0){
            j=0;
            i++;
        }
    }
}

//Next iteration of the game of life but putting a cell a dead with a given probability
void NextGenerationZero(unsigned char* life, float prob){
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float>  distr(0, 1);

    unsigned char* tmp;
    tmp = (unsigned char*)malloc(ncells * sizeof(unsigned char));
    memcpy(tmp, life, ncells * sizeof(unsigned char));
    int i=0, j=0, k=0;
    while(k < ncells) {
        if (distr(generator) <= prob){
            if (((*(tmp))&0x01)==1){
                ClearCell(i, j, life);
            }
            tmp++;
            k++;
            j++;
            if (j%width==0 && j != 0){
                j=0;
                i++;
            }
        }else{
            if (((*(tmp))&0x01)==0){//Dead but possibly to born
                if ((*(tmp))>>1==3){
                    SetCell(i, j, life);
                }
            }else{//Alive
                if ((*(tmp))>>1 != 2 && (*(tmp))>>1 != 3){
                    ClearCell(i, j, life);
                }
            }
            tmp++;
            k++;
            j++;
            if (j%width==0 && j != 0){
                j=0;
                i++;
            }
        }
    }
}

//Next iteration of the game of life but the state of the cell with a given probability
void NextGenerationIdentity(unsigned char* life, float prob){
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float>  distr(0, 1);

    unsigned char* tmp;
    tmp = (unsigned char*)malloc(ncells * sizeof(unsigned char));
    memcpy(tmp, life, ncells * sizeof(unsigned char));
    int i=0, j=0, k=0;
    while(k < ncells) {
        if (distr(generator) > prob) {
            if (((*(tmp)) & 0x01) == 0) {//Dead but possibly to born
                if ((*(tmp)) >> 1 == 3) {
                    SetCell(i, j, life);
                }
            } else {//Alive
                if ((*(tmp)) >> 1 != 2 && (*(tmp)) >> 1 != 3) {
                    ClearCell(i, j, life);
                }
            }
            tmp++;
            k++;
            j++;
            if (j % width == 0 && j != 0) {
                j = 0;
                i++;
            }
        }else{
            tmp++;
            k++;
            j++;
            if (j % width == 0 && j != 0) {
                j = 0;
                i++;
            }
        }
    }
}

//We use this to get a sequnce of char that represent the entire map, it s used with hash to check loops
std::string PatternReduce(unsigned char* life){
    std::string temp = "";
    for (int i = 0; i < ncells; ++i) {
        temp += (*(life+i))&0x01;
    }
    return temp;
}

//We permute the state of a cell near one alive
void Permutation(unsigned char* life){
    //Check if it has sense the permutation
    if (livingcells > 1){
        unsigned char* tmp;
        tmp = (unsigned char*)malloc(ncells * sizeof(unsigned char));
        memcpy(tmp, life, ncells * sizeof(unsigned char));
        int index=0;
        while (((*tmp)&0x01)==1 || (*(tmp))>>1==0){
            tmp++;
            index++;
        }
        int j = index%width;
        int i = (index-j)/width;
        SetCell(i, j, life);
    }else{
        printf("No sense in permutating\n");
    }
}

//Plot with gnuplot of the game
void Visualize(FILE *gp, unsigned char* life, int iterations) {
    fprintf(gp, "set title 'Time=%d Living Cells=%d'\n", iterations, livingcells);
    fprintf(gp, "plot '-' w image\n");
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            fprintf(gp, "%d %d %d\n", i, j, life[i*width+j] & 0x01);
        }
        fprintf(gp, "\n");
    }
    fprintf(gp, "e\n");
    fflush(gp);
}

//Plot with gnuplot of the average of alive cells in the game in a logscale
void Graph(FILE *gp, float* densityvect){
    fprintf(gp, "set title 'Average Living Cells'\n");
    fprintf(gp, "set logscale xy\n");;
    fprintf(gp, "plot '-' w lines\n");
    for (int i = 0; i < maxiter; ++i) {
        fprintf(gp, "%d %f\n",i, densityvect[i]/ntry);
    }
    fprintf(gp, "e\n");
    fflush(gp);
}

int main() {
    //Hash table to check if we get the same pattern more times
    std::unordered_map<std::string, unsigned int> patterns;
    //Pointer of the grid
    unsigned char* life;
    life = (unsigned char*)malloc(ncells * sizeof(unsigned char));
    float* densityvect = (float*) malloc(maxiter*sizeof(float));
    memset(densityvect, 0, maxiter*sizeof(float));
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float>  distr(0, 1);
    FILE *gp;
    gp = popen("gnuplot -persist", "w");
    fprintf(gp, "unset key; set cbrange [0:1]\n");
    FILE *gr;
    gr = popen("gnuplot -persist", "w");
    fprintf(gr, "unset key; set cbrange [0:1]\n");

    for (int n = 0; n < ntry; ++n) {
        float* densityptr = densityvect;
        memset(life, 0, ncells * sizeof(unsigned char));
        //Initialization of the grid
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                if (distr(generator) <= initialdensity) {
                    SetCell(i, j, life);
                }
            }
        }
        patterns[PatternReduce(life)] = 0;
        //Draw(life);
        Visualize(gp, life, 0);
        *densityptr += livingcells;
        densityptr++;
        int k = 0;
        bool loop = false;
        do {
            //NextGeneration(life);
            //NextGenerationIdentity(life, prob);
            NextGenerationZero(life, prob);
            *densityptr += livingcells;
            densityptr++;
            Visualize(gp, life, k + 1);
            //Draw(life);
            std::string temp = PatternReduce(life);
            //Cerchiamo nella tabella hash se la configurazione è già presente, altrimenti la inseriamo
            if (patterns.find(temp) == patterns.end()) {
                patterns[temp] = k;
            } else {
                loop = true;
            }
            k++;
            if (loop && permutation) {
                printf("Loop found after %d iterations\n", k);
                printf("Let's permutate\n");
                patterns.clear();
                *densityptr += livingcells;
                densityptr++;
                k = 0;
                Permutation(life);
                Visualize(gp, life, k + 1);
                //Draw(life);
                loop = false;
                permutation = false;
            }
        } while (!loop && k<maxiter);
        printf("Asymptotic state obtained in %d iterations\n", k);
        patterns.clear();
        livingcells = 0;
        permutation = false;
        if (k < maxiter && permutation){
            printf("Asymptotic state retarded by %d iterations", k);
        }
    }
    Graph(gr, densityvect);
}