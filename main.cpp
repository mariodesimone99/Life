#include <iostream>
#include <cmath>
#include <random>
#include <unordered_map>
#include <stdio.h>

const unsigned int width = 100;
const unsigned int height = 100;
const unsigned int nCells = width * height;
const float density = 0.5;
const float prob = 0.015;
const int ntry = 1;
unsigned int livingcells = 0;
const int maxiter = 1000000;
bool permutation = false;

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
        xabove = nCells-width;
        xbelow = width;
    }else if (x == height-1){
        xabove = -width;
        xbelow = -(nCells-width);
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
        xabove = nCells-width;
        xbelow = width;
    }else if (x == height-1){
        xabove = -width;
        xbelow = -(nCells-width);
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

void Draw(unsigned char* life){
    for (unsigned int i = 0; i < height; ++i) {
        for(unsigned int j = 0; j < width; ++j){
            if ((*(life+j+i*width)&0x01)==0){
                //printf("o%d\t", life[i*width+j]>>1);
                printf("o\t");
            }else{
                //printf("*%d\t", life[i*width+j]>>1);
                printf("*\t");
            }
        }
        printf("\n");
    }
}

void NextGeneration(unsigned char* life){
    unsigned char* tmp;
    tmp = (unsigned char*)malloc(nCells*sizeof(unsigned char));
    memcpy(tmp, life, nCells*sizeof(unsigned char));
    int i=0, j=0, k=0;
    while(k < nCells) {
        while(*(tmp)==0 && k < nCells-1){//Dead and no neighbours
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

void NextGenerationZero(unsigned char* life, float prob){
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float>  distr(0, 1);

    unsigned char* tmp;
    tmp = (unsigned char*)malloc(nCells*sizeof(unsigned char));
    memcpy(tmp, life, nCells*sizeof(unsigned char));
    int i=0, j=0, k=0;
    while(k < nCells) {
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

void NextGenerationIdentity(unsigned char* life, float prob){
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float>  distr(0, 1);

    unsigned char* tmp;
    tmp = (unsigned char*)malloc(nCells*sizeof(unsigned char));
    memcpy(tmp, life, nCells*sizeof(unsigned char));
    int i=0, j=0, k=0;
    while(k < nCells) {
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

std::string PatternReduce(unsigned char* life){
    std::string temp = "";
    for (int i = 0; i < nCells; ++i) {
        temp += (*(life+i))&0x01;
    }
    return temp;
}

void Permutation(unsigned char* life){
    //Check if it has sense the permutation
    if (livingcells > 1){
        unsigned char* tmp;
        tmp = (unsigned char*)malloc(nCells*sizeof(unsigned char));
        memcpy(tmp, life, nCells*sizeof(unsigned char));
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

void Visualize(FILE *gp, unsigned char* life, int k, int iterations) {
    fprintf(gp, "set title 't=%d'\n", iterations);
    fprintf(gp, "plot '-' w image\n");
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            fprintf(gp, "%d %d %d\n", i, j, life[i*width+j] & 0x01);
        }
        fprintf(gp, "\n");
    }
    //fprintf(gp, "'pause -1'\n");
    fprintf(gp, "e\n");
    fflush(gp);
}

void Graph(FILE *gp, float* densityvect){
    fprintf(gp, "set title 'Average Living Cells'\n");
    fprintf(gp, "set logscale xy\n");;
    fprintf(gp, "plot '-' w lines\n");
    for (int i = 0; i < maxiter; ++i) {
        if (densityvect[i] != 0){
            fprintf(gp, "%d %f\n",i, densityvect[i]/ntry);
        }
    }
    //fprintf(gp, "'pause -1'\n");
    fprintf(gp, "set term png\n");
    fprintf(gp, "set output 'graph.png'\n");
    fprintf(gp, "e\n");
    fflush(gp);
}

int main() {
    std::unordered_map<std::string, unsigned int> patterns;
    unsigned char* life;
    life = (unsigned char*)malloc(nCells*sizeof(unsigned char));
    float* densityvect = (float*) malloc(maxiter*sizeof(float));
    memset(densityvect, 0, maxiter*sizeof(float));
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float>  distr(0, 1);
    FILE *gp;
    gp = popen("gnuplot", "w");
    fprintf(gp, "unset key; set cbrange [0:1]\n");

    for (int n = 0; n < ntry; ++n) {
        float* densityptr = densityvect;
        memset(life, 0, nCells * sizeof(unsigned char));
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                if (distr(generator) <= density) {
                    SetCell(i, j, life);
                }
            }
        }
        patterns[PatternReduce(life)] = 0;
        //Draw(life);
        Visualize(gp, life, 5, 0);
        *densityptr += livingcells;
        densityptr++;
        double d = livingcells / ((double) nCells);
        printf("Density living cells: %f",d);
        printf("\n\n");
        int k = 0;
        bool loop = false;
        do {
            //NextGeneration(life);
            //NextGenerationIdentity(life, prob);
            NextGenerationZero(life, prob);
            *densityptr += livingcells;
            densityptr++;
            Visualize(gp, life, 5, k + 1);
            //Draw(life);
            d = livingcells / ((double) nCells);
            printf("Density living cells: %f", d);
            printf("\n\n");
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
                *densityptr = d;
                densityptr++;
                k = 0;
                Permutation(life);
                Draw(life);
                loop = false;
                permutation = false;
            }
        } while (!loop && k<maxiter);
        patterns.clear();
        livingcells = 0;
        permutation = true;
        if (k < maxiter){
            printf("Asymptotic state retarded by %d iterations", k);
        }else{
            printf("Max iterations exceeded");
        }
    }
    printf("\n");
    fprintf(gp, "clear\n");
    while(true){
        Graph(gp, densityvect);
    }

    /*for (int i = 0; i < maxiter; ++i) {
        if (densityvect[i] != 0){
            printf("%f - %f\n", densityvect[i]/ntry, log10(densityvect[i]/ntry));
        }
    }*/
}