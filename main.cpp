#include <iostream>
#include <random>

const unsigned int width = 20;
const unsigned int height = 20;
const unsigned int nCells = width * height;
int livingcells = 0;


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
}

void ClearCell(unsigned int x, unsigned int y, unsigned char* life){
    /*life[x*width+y] &= ~0x01;
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
        xbelow = 1;
    }else if (x == height-1){
        xabove = -1;
        xbelow = -(nCells-width);
    }else{
        xabove = -width;
        xbelow = width;
    }
    life[x*width+y+xabove+yleft] -= 0x02;
    life[x*width+y+yleft] -= 0x02;
    life[x*width+y+yleft+xbelow] -= 0x02;
    life[x*width+y+xbelow] -= 0x02;
    life[x*width+y+yright+xbelow] -= 0x02;
    life[x*width+y+yright] -= 0x02;
    life[x*width+y+yright+xabove] -= 0x02;
    life[x*width+y+xabove] -= 0x02;*/

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

//unsigned char LifeState(int i, int j){
    //return life[i*width+j]&0x01;
//}

//unsigned char LifeNeighbours(int i, int j){
    //return life[i*width+j]>>1;
//}

void NextGeneration(unsigned char* life){
    unsigned char* tmp;
    tmp = (unsigned char*)malloc(nCells*sizeof(unsigned char));
    memcpy(tmp, life, nCells);
    int i=0, j=0, k=0;
    while(k < nCells) {
        while(*(tmp)==0){//Dead and no neighbours
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
                livingcells++;
            }
        }else{//Alive
            if ((*(tmp))>>1 != 2 && (*(tmp))>>1 != 3){
                ClearCell(i, j, life);
                livingcells--;
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
    memcpy(tmp, life, nCells);
    int i=0, j=0, k=0;
    while(k < nCells) {
        if (distr(generator) <= prob){
            ClearCell(i, j, life);
        }else{
            while(*(tmp)==0){//Dead and no neighbours
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
                    livingcells++;
                }
            }else{//Alive
                if ((*(tmp))>>1 != 2 && (*(tmp))>>1 != 3){
                    ClearCell(i, j, life);
                    livingcells--;
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
    memcpy(tmp, life, nCells);
    int i=0, j=0, k=0;
    while(k < nCells) {
        if (distr(generator) > prob) {
            while (*(tmp) == 0) {//Dead and no neighbours
                k++;
                tmp++;
                j++;
                if (j % width == 0 && j != 0) {
                    j = 0;
                    i++;
                }
            }
            if (((*(tmp)) & 0x01) == 0) {//Dead but possibly to born
                if ((*(tmp)) >> 1 == 3) {
                    SetCell(i, j, life);
                    livingcells++;
                }
            } else {//Alive
                if ((*(tmp)) >> 1 != 2 && (*(tmp)) >> 1 != 3) {
                    ClearCell(i, j, life);
                    livingcells--;
                }
            }
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

int main() {
    //unsigned char life[nCells];
    unsigned char* life;
    life = (unsigned char*)malloc(nCells*sizeof(unsigned char));
    float density = 0.5;

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float>  distr(0, 1);
    memset(life, 0, nCells);
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            if (distr(generator) <= density){
                SetCell(i, j, life);
                livingcells++;
            }
        }
    }
    Draw(life);
    double d = livingcells/((double)nCells);
    printf("Density living cells: %f", d);
    printf("\n\n");
    int k = 0;
    while(k<3){
        //NextGeneration(life);
        NextGenerationIdentity(life, 0.28);
        Draw(life);
        d = livingcells/((double)nCells);
        printf("Density living cells: %f", d);
        printf("\n\n");
        k++;
    }
}