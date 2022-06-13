#include <iostream>
#include <random>

//unsigned char* life;
//unsigned char* tmp;
const unsigned int width = 10;
const unsigned int height = 10;
const unsigned int nCells = width * height;


void SetCell(unsigned int x, unsigned int y, unsigned char life[]){
    /*unsigned char* ptr = life + x*width+y;
    *ptr |= 0x01;
    int yright, yleft, xabove, xbelow;
    if (y == width-1){
        yright = -(width - 1);
        yleft = -1;
    }else if(y == 0){
        yright = 1;
        yleft = width-1;
    }else{
        yright = 1;
        yleft = -1;
    }

    if (x == height-1){
        xabove = -1;
        xbelow = -(nCells - width);
    }else if(x == 0){
        xabove = nCells - width;
        xbelow = 1;
    }else{
        xabove = 1;
        xbelow = -1;
    }

    *(ptr+yleft+xabove) += 0x02;
    *(ptr+yleft) += 0x02;
    *(ptr+yleft+xbelow) += 0x02;
    *(ptr+xbelow) += 0x02;
    *(ptr+yright+xbelow) += 0x02;
    *(ptr+yright) += 0x02;
    *(ptr+yright+xabove) += 0x02;
    *(ptr+xabove) += 0x02;*/
    life[x*width+y] |= 0x01;
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
    life[x*width+y+xabove+yleft] += 0x02;
    life[x*width+y+yleft] += 0x02;
    life[x*width+y+yleft+xbelow] += 0x02;
    life[x*width+y+xbelow] += 0x02;
    life[x*width+y+yright+xbelow] += 0x02;
    life[x*width+y+yright] += 0x02;
    life[x*width+y+yright+xabove] += 0x02;
    life[x*width+y+xabove] += 0x02;
}

void ClearCell(unsigned int x, unsigned int y, unsigned char life[]){
    /*unsigned char* ptr = life + x*width+y;
    *ptr &= ~0x01;
    int yright, yleft, xabove, xbelow;
    if (y == width-1){
        yright = -(width - 1);
        yleft = -1;
    }else if(y == 0){
        yright = 1;
        yleft = width-1;
    }else{
        yright = 1;
        yleft = -1;
    }

    if (x == height-1){
        xabove = -1;
        xbelow = -(nCells - width);
    }else if(x == 0){
        xabove = nCells - width;
        xbelow = 1;
    }else{
        xabove = 1;
        xbelow = -1;
    }

    *(ptr+yleft+xabove) -= 0x02;
    *(ptr+yleft) -= 0x02;
    *(ptr+yleft+xbelow) -= 0x02;
    *(ptr+xbelow) -= 0x02;
    *(ptr+yright+xbelow) -= 0x02;
    *(ptr+yright) -= 0x02;
    *(ptr+yright+xabove) -= 0x02;
    *(ptr+xabove) -= 0x02;*/
    life[x*width+y] &= ~0x01;
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
    life[x*width+y+xabove] -= 0x02;
}

void Draw(unsigned char life[]){
    for (unsigned int i = 0; i < height; ++i) {
        for(unsigned int j = 0; j < width; ++j){
            if ((life[j+i*width]&0x01)==0){
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

void NextGeneration(unsigned char life[]){

    /*unsigned char* ptr;
    memcpy(tmp, life, nCells);
    ptr = tmp;
    unsigned int k = 0, j = 0, i = 0;
    while (k < nCells){
        unsigned char state = LifeState(i,j);
        unsigned char neighbours = LifeNeighbours(i,j);
        while (state == 0 && neighbours != 3){
            //ptr++;
            k++;
            j++;
            if (j % width == 0){
                j=0;
                i++;
            }
            state = LifeState(i,j);
            neighbours = LifeNeighbours(i,j);
        }
        if (state == 0){
            SetCell(i, j);
        }else{
            if (neighbours < 2 || neighbours > 3){
                ClearCell(i, j);
            }
        }
        //ptr++;
        j++;
        if (j % width == 0){
            j=0;
            i++;
        }
        k++;
    }*/
    unsigned char tmp[nCells];
    memcpy(tmp, life, nCells);
    int i=0, j=0, k=0;
    while(k < nCells) {
        while(life[k]==0){
            k++;
            j++;
            if (j%width==0 && j != 0){
                j=0;
                i++;
            }
        }
        if ((life[k]&0x01)==0){
            if (life[k]>>1==3){
                SetCell(i, j, tmp);
            }
        }else{
            if (life[k]>>1 != 2 && life[k]>>1 != 3){
                ClearCell(i, j, tmp);
            }
        }
        k++;
        j++;
        if (j%width==0 && j != 0){
            j=0;
            i++;
        }
    }
    memcpy(life, tmp, nCells);
    //Draw(life);
}

int main() {
    unsigned char life[nCells];
    float density = 0.2;

    /*life = (unsigned char*) malloc(nCells*sizeof(unsigned char));
    tmp = (unsigned char*) malloc(nCells*sizeof(unsigned char));
    memset(life, 0, nCells);*/
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float>  distr(0, 1);
    memset(life, 0, nCells);
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            if (distr(generator) <= density){
                SetCell(i, j, life);
            }
        }
    }
    Draw(life);
    printf("\n");
    int k = 0;
    while(k<3){
        NextGeneration(life);
        Draw(life);
        printf("\n");
        k++;
    }
}