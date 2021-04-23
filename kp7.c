#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
	int w1;
	int h1;
	int *arr_M;
	float *arr_A;
	int arr_M_len;
	int arr_A_len;
} matrix1;

void arr_insert(int **arr,int *arrlen,int pos,int len)
{
	*arrlen+=len;
	*arr=realloc(*arr,(*arrlen)*4);
	memmove(*arr+(pos+len),*arr+pos,(*arrlen-pos-len)*4);
	memset(*arr+pos,0,len*4);
}

void arr_remove(int **arr,int *arrlen,int pos,int len)
{
	memmove(*arr+pos,*arr+(pos+len),(*arrlen-(pos+len))*4);
	*arrlen-=len;
	*arr=realloc(*arr,(*arrlen)*4);
}

float getelem(matrix1 *a,int x,int y)
{
	int cur1=a->arr_M[y];
	while(cur1!=0)
	{
		int x1=(int)a->arr_A[cur1*3];
		float value1=a->arr_A[cur1*3+1];
		int next1=(int)a->arr_A[cur1*3+2];
		if(x1==x)
			return value1;
		cur1=next1;
	}
	return 0.0f;
}

void setelem(matrix1 *a,int x,int y,float val)
{
	float now1=getelem(a,x,y);
	if(now1==val)
		return;
	if(now1==0)
	{
		int cur1=a->arr_M[y];
		if(cur1==0)
		{
			a->arr_M[y]=a->arr_A_len/3;
			cur1=a->arr_A_len;
			arr_insert((int**)(&a->arr_A),&a->arr_A_len,a->arr_A_len,3);
			a->arr_A[cur1]=(float)x;
			a->arr_A[cur1+1]=val;
			a->arr_A[cur1+2]=0;
			return;
		}
		while(cur1!=0)
		{
			int x1=(int)a->arr_A[cur1*3];
			float value1=a->arr_A[cur1*3+1];
			int next1=(int)a->arr_A[cur1*3+2];
			if(next1==0)
			{
				a->arr_A[cur1*3+2]=a->arr_A_len/3;
				cur1=a->arr_A_len;
				arr_insert((int**)(&a->arr_A),&a->arr_A_len,a->arr_A_len,3);
				a->arr_A[cur1]=(float)x;
				a->arr_A[cur1+1]=val;
				a->arr_A[cur1+2]=0;
				return;
			}
			cur1=next1;
		}
	}
	else if(val==0)
	{
		int cur1=a->arr_M[y];
		int prev1=0;
		while(cur1!=0)
		{
			int x1=(int)a->arr_A[cur1*3];
			float value1=a->arr_A[cur1*3+1];
			int next1=(int)a->arr_A[cur1*3+2];
			if(x1==x)
			{
				if(prev1==0)
					a->arr_M[y]=next1;
				else
					a->arr_A[prev1*3+2]=next1;
				arr_remove((int**)(&a->arr_A),&a->arr_A_len,cur1*3,3);
				for(int i=0;i<a->arr_M_len;i++)
					if(a->arr_M[i]>cur1)
						a->arr_M[i]--;
				for(int i=2;i<a->arr_A_len;i+=3)
					if(a->arr_A[i]>cur1)
						a->arr_A[i]--;
				break;
			}
			prev1=cur1;
			cur1=next1;
		}
	}
	else
	{
		int cur1=a->arr_M[y];
		while(cur1!=0)
		{
			int x1=(int)a->arr_A[cur1*3];
			float value1=a->arr_A[cur1*3+1];
			int next1=(int)a->arr_A[cur1*3+2];
			if(x1==x)
			{
				a->arr_A[cur1*3+1]=val;
				break;
			}
			cur1=next1;
		}
	}
}

void matrix1_print(matrix1 *a)
{
	printf("M: ");
	for(int i=0;i<a->arr_M_len;i++)
		printf("%d ",a->arr_M[i]);
	printf("\n");
	printf("A: ");
	for(int i=0;i<a->arr_A_len;i+=3)
		printf("%d %f %d ",(int)a->arr_A[i],a->arr_A[i+1],(int)a->arr_A[i+2]);
	printf("\n");
}

void matrix1_printnormal(matrix1 *a)
{
	for(int y=0;y<a->h1;y++)
	{
		for(int x=0;x<a->w1;x++)
			printf("%f\t",getelem(a,x,y));
		printf("\n");
	}
}

void matrix1_fromfile(char *filename,matrix1 *m)
{
	FILE *in=fopen(filename,"rb");
	if(!in)
	{
		printf("Нет такого файла\n");
		exit(2);
	}
	fscanf(in,"%d %d",&(m->h1),&(m->w1));
	if(m->h1!=m->w1)
	{
		printf("Для решения этой задачи нужна квадратная матрица\n");
		exit(3);
	}
	arr_insert(&m->arr_M,&m->arr_M_len,0,m->h1);
	arr_insert((int**)(&m->arr_A),&m->arr_A_len,0,3);
	for(int y=0;y<m->h1;y++)
	for(int x=0;x<m->w1;x++)
	{
		float cur1;
		fscanf(in,"%f",&cur1);
		setelem(m,x,y,cur1);
	}
	fclose(in);
}

// транспонировать матрицу относительно побочной диагонали
void transposediag(matrix1 *mtx)
{
	int s=mtx->w1;
	for(int y=0;y<s;y++)
	for(int x=0;x<s-y;x++)
	{
		float tmp1=getelem(mtx,x,y);
		setelem(mtx,x,y,getelem(mtx,s-y-1,s-x-1));
		setelem(mtx,s-y-1,s-x-1,tmp1);
		//swap(&(mtx[x][y]),&(mtx[s-y-1][s-x-1]));
	}
}

// проверка кососимметричности матрицы
char issym(matrix1 *mtx)
{
	int s=mtx->w1;
	for(int y=0;y<s;y++)
	for(int x=0;x<s;x++)
		if(getelem(mtx,x,y)!=-getelem(mtx,y,x))
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
	matrix1 mtx1={0,0,0,0,0,0};
	matrix1_fromfile(argv[1],&mtx1);
	printf("Обычный вид матрицы:\n");
	matrix1_printnormal(&mtx1);
	printf("\nВид матрицы по варианту:\n");
	matrix1_print(&mtx1);
	transposediag(&mtx1);
	printf("\nОбычный вид матрицы (после преобразования):\n");
	matrix1_printnormal(&mtx1);
	printf("\nВид матрицы по варианту (после преобразования):\n");
	matrix1_print(&mtx1);
	printf("\nЯвляется ли матрица кососимметричной? %s\n",issym(&mtx1) ? "Да":"Нет");
    return 0;
}
