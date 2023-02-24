#include <stdio.h>
#include <stdlib.h>
#include <string.h>
									//Muhammed Ali LALE - 20011045
typedef struct instagramAccounts{
	int enable;		//1. aþamadan sonra dugumleri elemek icin
	int no;			//dugum numarasi
	char fname[20];	//ad
	char lname[20];	//soyad
	int *follows;	//takip edilenler dizisi
	int numOfFollows;	//takip ettikleri sayisi
	int numOfFollowers;	//takipci sayisi
	int totalConnection;//tpolam baglanti sayisi
	int visited;	//visited kontrolu
}ACCOUNTS;

ACCOUNTS* readFile(char*,int*);
int countFollowers(ACCOUNTS*,int,int);
void eleminate(ACCOUNTS*,int,int);
void dfs(ACCOUNTS*,int);
void setNotVisited(ACCOUNTS*,int);

int main(){
	int M,X,Y,size,j,i;
	ACCOUNTS* accounts = readFile("socialNET.txt",&size);	//socialNet.txtden veriler okunur ve accounts dizisinde tutulur. size dugum sayisi
	
	char mod;	//Kullanici karakter girerse hata vermemesi icin mod secimi char ile
	printf("  Lutfen programi hangi modda calistirmak istediginizi seciniz\n");
	printf("  1.Normal Mod ");
	printf("  2.Detay Mod \n\t >");
	
	scanf("%c",&mod);
	while(mod!='1' && mod!='2'){
		printf(" Lutfen 1 veya 2 seceneklerini seciniz!\n\t>");
		scanf("%c",&mod);
	}
	
	printf(" Belirtilen M degerini giriniz: ");
	scanf("%d", &M);
	
	printf(" Belirtilen X degerini giriniz: ");
	scanf("%d", &X);
	
	printf(" Belirtilen Y degerini giriniz: ");
	scanf("%d", &Y);
	
	int flag=countFollowers(accounts,size,M);	//takipci sayilarini say
	
	if(mod=='2'){	//detay modddaysa balangýc durumunu ekrana yazdir
		printf("\n 1.Graftaki tum dugumlerin balangic durumundaki indegree degerleri: ");
		for(i=0; i<size; i++){
			printf("\n  %d -\tindegree degeri: %d", accounts[i].no, accounts[i].numOfFollowers);
		}
	}
	
	while(flag){	//elenecek eleman kalmayana kadar elemanlarý ele ve yeniden hesapla
		eleminate(accounts,size,M);
		flag=countFollowers(accounts,size,M);
	}
	
	if(mod=='2'){	//detay moddaysa eleme ilemi sonucunda kalanlari ekranaa yazdýr
		printf("\n\n 2.M degeri ile elenmeyen dugumlerin ad,soyad ve indegree degerleri: ");
		for(i=0; i<size; i++){
			if(accounts[i].enable)
			printf("\n  %d -\t Ad: %s  Soyad:%s\tindegree degeri: %d", accounts[i].no, accounts[i].fname, accounts[i].lname, accounts[i].numOfFollowers);
		}
	}	
	
		//dfs ile total baglanti sayisi bulma
	for (i=0; i<size; i++){	
		if(accounts[i].enable){
			dfs(accounts,i);
			setNotVisited(accounts,size);	//dfsten dönünce tum dugumleri notvisited yap.
		}
	}
	
	printf("\n\n 3.Influencer bilgileri\n");	//influencerlari ekrana yazdir
	for (j=0;j<size;j++){
		if(accounts[j].enable && accounts[j].numOfFollowers>=X && accounts[j].totalConnection >=Y){
			printf("  %d -\t Ad: %s  Soyad:%s\tindegree degeri: %d\ttoplam baglanti sayisi:%d\n",accounts[j].no, accounts[j].fname, accounts[j].lname, accounts[j].numOfFollowers,accounts[j].totalConnection);
		}
	}
	
	system("PAUSE");
	return 0;
}

void dfs(ACCOUNTS *accounts, int current){	
	accounts[current].visited = 1;	//dugumu visited olarak isaretle
	int i, N = accounts[current].numOfFollows;
	for(i=0; i<N; i++){
		if(!accounts[accounts[current].follows[i]-1].visited && accounts[accounts[current].follows[i]-1].enable){	//visited degilse ve elenmemisse
			accounts[accounts[current].follows[i]-1].totalConnection++;	//ilgili dugumun baglanti sayisini arttir
			dfs(accounts,accounts[current].follows[i]-1);	//ilgili dugumde dfs cagir
		}
	}
}

void setNotVisited(ACCOUNTS *accounts, int N){	//tüm dugumleri notvisited yap
	int i;
	for (i=0; i<N; i++){
		accounts[i].visited=0;
	}
}

int countFollowers(ACCOUNTS *accounts,int N,int M){	//her dugumun in degree sayisini bul
	int i,j;
	int flag=0;
	for(i=0; i<N ;i++){
		if(accounts[i].enable){
			for(j=0; j<accounts[i].numOfFollows; j++)
				accounts[accounts[i].follows[j]-1].numOfFollowers++;
			}
	}
	
	for(i=0; i<N ;i++){	//elemeyapmaya gerek var mi kontrolu
		if(accounts[i].enable && accounts[i].numOfFollowers < M)
			flag=1;
	}
	return flag; //kontrolu dondurur
}

void eleminate(ACCOUNTS *accounts,int N, int M){ //indegree sayisi Mden kucuk olanlarý disable et
	int i;
	for(i=0; i<N ;i++){
		if(accounts[i].numOfFollowers < M)
			accounts[i].enable = 0;
		accounts[i].numOfFollowers = 0;	//takipci sayisini sifirla (yeniden hesaplanacak)
	}
}

ACCOUNTS* readFile(char *fileName, int *N){	//dosya okuma
	ACCOUNTS *accounts = (ACCOUNTS*)malloc(sizeof(ACCOUNTS));
	char line[255];
	int i=0, n;
	FILE *fp = fopen(fileName,"r"); //dosya oku
	while(fgets(line,255,fp)!=NULL){ //ilk satiri okur
		if(i!=0){	//ilk dugum degilse reallocla genislet
			accounts=(ACCOUNTS*)realloc(accounts,(i+1)*sizeof(ACCOUNTS));
		}
		accounts[i].enable=1;	//init
		accounts[i].visited=0;
		accounts[i].totalConnection = 0;
		accounts[i].numOfFollowers = 0;
		//ilk satiri virgullere gore ayirip degiskenlere atar
		char *ptr = strtok(line, ",");
		accounts[i].no=atoi(ptr);
		ptr = strtok(NULL, ",");
		strcpy(accounts[i].fname,ptr);
		ptr = strtok(NULL, "\n");
		strcpy(accounts[i].lname,ptr);
		
		accounts[i].numOfFollows=1;	
		accounts[i].follows = (int*)malloc(sizeof(int)); //takip edilenler dizisi olusturulur
		
		
		fgets(line,255,fp);	//ikinci satiri okur
		ptr = strtok(line, ",\n");
		accounts[i].follows[0]=atoi(ptr);
				
		ptr = strtok(NULL, ",\n");
		while (ptr!=NULL){	//satir sonuna dek okuyup diziye atar
			n=++accounts[i].numOfFollows;
			accounts[i].follows = (int*)realloc(accounts[i].follows, n*sizeof(int)); //takip edilenler dizisini genisletir
			accounts[i].follows[n-1]=atoi(ptr);
			ptr = strtok(NULL, ",\n");
		}
		i++;
	}
		*N=i;	//eleman sayisini günceller
		return accounts;	//struct dizisini dondurur
}
