#include <stdio.h>
#include <stdlib.h>

#define MIN -999999999

int bruteForce(int*,int,int*,int*);
int divideAndConquer(int*, int, int,int*,int*);


int main(){
	int* maden;
	int N, i;
	int ilkIndis, sonIndis;
	
	printf(" Dizinin Eleman sayisini giriniz: ");
	scanf("%d",&N);	
	maden = (int*)malloc(N*sizeof(int)); 
	for(i=0; i<N; i++){
		printf("Eleman %d: ",i+1);	
		scanf("%d",&maden[i]);
	}
	
	printf("\nBrute Force cozum ile cozum:\n");
	printf(" En yuksek kazanc: %d\n",bruteForce(maden,N,&ilkIndis,&sonIndis));
	printf(" Kazilmasi gereken kesintisiz blok butunu: %d-%d\n",ilkIndis,sonIndis);

	printf("\nDivide and Conquer ile cozum: \n");
	printf(" En yuksek kazanc: %d\n",divideAndConquer(maden,0,N-1,&ilkIndis,&sonIndis));
	printf(" Kazilmasi gereken kesintisiz blok butunu: %d-%d",ilkIndis,sonIndis);
	
	return 0;
	
}

int bruteForce(int maden[], int n, int *ilkIndis, int *sonIndis){
	int kazanc=maden[0]; 
	int toplam=0;
	int i, j;
	
	for(i=0; i<n; i++){
		toplam = 0;
		
		for(j=i; j<n; j++){
			toplam += maden[j];
			
			if(kazanc < toplam){
				kazanc = toplam;
				*ilkIndis=i;
				*sonIndis=j;
			}
		}
	}
	return kazanc;
}

int divideAndConquer(int maden[], int ilk, int son,int *ilkIndis, int *sonIndis){
	if(ilk >= son){
		return maden[ilk];
	}
	
	int solLimit, sagLimit;
	int mid_in = (ilk+son)/2; //ortadaki elemanýnindisi
	int max_ilk = MIN, max_son = MIN;
	int i, toplam = 0;
	
	for(i=mid_in; i>=ilk; i--){
		toplam += maden[i];
		if(max_ilk < toplam){
			max_ilk = toplam;
			solLimit = i;
		}
	}
	
	toplam=0;
	for(i=mid_in+1; i<=son; i++){
		toplam += maden[i];
		if(max_son < toplam){
			max_son = toplam;
			sagLimit=i;
		}
	}
	
	int maxSonuc;
	int ilkSonuc=divideAndConquer(maden,ilk,mid_in,ilkIndis,sonIndis);
	int sonSonuc=divideAndConquer(maden,mid_in+1,son,ilkIndis,sonIndis);
	
	if ( ilkSonuc > sonSonuc){
		maxSonuc=ilkSonuc;
		*ilkIndis=solLimit;
		*sonIndis=mid_in;
	}else{
		maxSonuc=sonSonuc;
		*ilkIndis=mid_in;
		*sonIndis=sagLimit;
	}
		
	if((max_ilk+max_son) > maxSonuc){
		*ilkIndis=solLimit;
		*sonIndis=sagLimit;
		maxSonuc= (max_ilk+max_son);
	}
	return maxSonuc;
}

