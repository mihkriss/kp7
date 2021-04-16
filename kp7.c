#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void swap(double *a,double *b)
{
	double t=*a;
	*a=*b;
	*b=t;
}

double **readmatrixfromfile(char *filename,int *w,int *h)
{
	FILE *in=fopen(filename,"rb");
	if(!in)
	{
		printf("Нет такого файла\n");
		exit(2);
	}
	fscanf(in,"%d %d",h,w);
	if(*h!=*w)
	{
		printf("Для решения этой задачи нужна квадратная матрица\n");
		exit(3);
	}
	double **res=(double**)malloc((*h)*sizeof(double*));
	for(int i=0;i<*w;i++)
		res[i]=(double*)malloc((*w)*sizeof(double));
	for(int y=0;y<*h;y++)
	for(int x=0;x<*w;x++)
		fscanf(in,"%lf",&(res[x][y]));
	fclose(in);
	return res;
}

void printmatrix(double **mtx,int w,int h)
{
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++)
			printf("%lf ",mtx[x][y]);
		printf("\n");
	}
}

void printmatrixvar(double **mtx,int w,int h)
{
	int cnt[h];
	int ccnt=1;
	printf("A: 0 0 0");
	for(int y=0;y<h;y++)
	{
		int strt=ccnt;
		int total=-1;
		for(int x=0;x<w;x++)
			total+=mtx[x][y]!=0;
		for(int x=0;x<w;x++)
		{
			double cnum=mtx[x][y];
			if(cnum!=0)
			{
				printf(" %d %lf %d",x,cnum,total ? (ccnt+1):0);
				ccnt++;
				total--;
			}
		}
		cnt[y]=(y==0 ? 1:cnt[y-1])==strt ? 0:strt;
	}
	printf("\nM:");
	for(int i=0;i<h;i++)
		printf(" %d",cnt[i]);
	printf("\n");
}

void transposediag(double **mtx,int s)
{
	for(int y=0;y<s;y++)
	for(int x=0;x<s-y;x++)
		swap(&(mtx[x][y]),&(mtx[s-y-1][s-x-1]));
}

char issym1(double **mtx,int s)
{
	for(int y=0;y<s;y++)
	for(int x=0;x<s;x++)
		if(mtx[x][y]!=-mtx[y][x])
			return 0;
	return 1;
}

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("Использование: [имя_файла]\n");
		return 1;
	}
	int w1;
	int h1;
	double **mtx1=readmatrixfromfile(argv[1],&w1,&h1);
	printf("Обычный вид матрицы:\n");
	printmatrix(mtx1,w1,h1);
	printf("\nВид матрицы по варианту:\n");
	printmatrixvar(mtx1,w1,h1);
	transposediag(mtx1,w1);
	printf("\nОбычный вид матрицы (после преобразования):\n");
	printmatrix(mtx1,w1,h1);
	printf("\nВид матрицы по варианту (после преобразования):\n");
	printmatrixvar(mtx1,w1,h1);
	printf("\nЯвляется ли матрица кососимметричной? %s\n",issym1(mtx1,w1) ? "Да":"Нет");
    return 0;
}

