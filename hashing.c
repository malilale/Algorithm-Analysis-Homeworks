#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//Muhammed Ali LALE - 20011045 - Algoritma Analizi 3. ödev - Hashing

struct webCrawler{
	char **links; //keyin geçtiði linkler dizisi
	char key[30]; 
	int size;	//link dizisinin boyutu
	int tries;	//hashtable'a yerleþtirikenki deneme sayýsý
};

int findSize();
int keyExists(char**, char*, int);
int isPrime(int);
int findClosestPrimeNumber(int);
unsigned long long int hornerMethod(char*);
void readFile(struct webCrawler*,int);
void findKey(struct webCrawler*,int,char*);
void findMultipleKeys(struct webCrawler*,int,char*,char*,char*);
void init(struct webCrawler*,int);

int main(){
	float loadfactor;
	int size=findSize(); //key sayýsý bulunur
	int hashSize, mod;
	char s[2], keys[60], key1[30], key2[30], op[10];
	int i,j;
	
	printf("Lutfen Load Factor degerini giriniz (0-1) (. kullanarak) : ");
	scanf("%f", &loadfactor);
	hashSize=findClosestPrimeNumber(size/loadfactor); 
	
	struct webCrawler *crawler = (struct webCrawler*)malloc(hashSize*sizeof(struct webCrawler));
	init(crawler,hashSize);	//structa size ve tries deðerlerini ayarlar
	readFile(crawler,hashSize);	//dosyayý oku ve hashtable hazýrla
	
	printf("\n\nLutfen gotuntulemek istediginiz modu seciniz\n");
	printf("  1.Normal Mod ");
	printf("  2.Detay Mod \n\t >");
	scanf("%d",&mod);
	while(mod!=1 && mod!=2){
		printf("Lutfen 1 veya 2 seceneklerini seciniz!\n\t>");
		scanf("%d",&mod);
	}
	
	if(mod==1){
		printf("\n\nBulmak istediginiz keyleri giriniz:\n\t>");
		gets(s);	//enterý yakalamamak içib boþ gets() fonksiyonu
		gets(keys);	//keyleri oku
		char *ptr = strtok(keys, " \n");  //parse et
		strcpy(key1,ptr); //ilk kelime

		ptr = strtok(NULL, " \n");
		if(ptr!=NULL){ //eðer ikinci bir kelime varsa
			strcpy(op,ptr); //operand
			ptr = strtok(NULL, " \n");
			strcpy(key2,ptr); //ikinci key
			
			findMultipleKeys(crawler,hashSize,key1,op,key2); 
		}else{ //tek kelime aranýyorsa
			findKey(crawler,hashSize,keys);
		}

	}else{
		printf("Number of keys: %d\n",size);
		printf("\nThe size of the hashtable is: %d\n",hashSize);
		for(i=0; i<hashSize;i++){
			if(crawler[i].size!=0){
				printf("\n%d. |%s|  %d denemede yerlesti",i,crawler[i].key, crawler[i].tries);
				for(j=0; j<crawler[i].size; j++)
					printf("\n %s",crawler[i].links[j]);
			}
		}
		printf("\n\nBulmak istediginiz keyleri giriniz:\n\t>"); //arama
		gets(s);	//enterý yakalamamak içib boþ gets() fonksiyonu
		gets(keys);	//keyleri oku
		
		char *ptr = strtok(keys, " \n");  //parse et
		strcpy(key1,ptr); //ilk kelime

		ptr = strtok(NULL, " \n");
		if(ptr!=NULL){ //eðer ikinci bir kelime varsa
			strcpy(op,ptr); //operand
			ptr = strtok(NULL, " \n");
			strcpy(key2,ptr); //ikinci key
			
			findMultipleKeys(crawler,hashSize,key1,op,key2); 
		}else{ //tek kelime aranýyorsa
			findKey(crawler,hashSize,keys);
		}
	}	
	return 0;
}

int keyExists(char** keys, char* word, int N){ //kelimenin dizide olup olmadýðý kontrolü
	int i;
	for(i=0; i<N; i++){
		if(strcmp(keys[i],word)==0)
			return 1;
	}
	return 0;
}

int findSize(){
	char keyLine[100];
	char line[255];
	char **keys;
	char **tmp;
	int size=0,i=0;
	
	keys=(char**)malloc(sizeof(char*));
	keys[0]=(char*)malloc(sizeof(char)*30);
	
	FILE *fp = fopen("Sample.txt","r"); 
	if (fp==NULL){
		printf("File could not open! ");
		exit(1);
	}
	while(fgets(line,255,fp)!=NULL){ 
		fgets(keyLine,255,fp);

		char *ptr = strtok(keyLine, " \n");	//ikinci satýrý boþluða göre parse et
		while (ptr != NULL)
		{
			if(!keyExists(keys,ptr,i+1)) //key dizide yoksa
				strcpy(keys[i++],ptr);	//diziye ekle
			ptr = strtok(NULL, " \n");
			
			tmp = (char**)realloc(keys, (i+1)*sizeof(char*)); //diziyi geniþlet
			if(tmp==NULL){
				fclose(fp);
				printf("Allocation Failure! ");
				exit(1);
			}
			keys=tmp;
			keys[i]=(char*)malloc(sizeof(char)*30);
		}
	}
	return i; //toplam key sayýsýný döndür
}

int isPrime(int n) { //sayýnýn asal olup olmadýðýný bulma
	int i;
    if (n <= 2 || n % 2 == 0)
        return n == 2;
    for (i = 3; i <= n / i; i += 2) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int findClosestPrimeNumber(int n){ //büyük en yakýn asal sayýyý bulmak
	int i;
	for (i = 0;; i++) {
        if (isPrime(n + i))
            return n+i;
    }
}

void readFile(struct webCrawler *crawler, int hashSize){ //dosyayý oku ve hashtable oluþtur
	char keyLine[100];
	char line[255], link[100];
	int i;
	unsigned long long int hashCode;
	FILE *fp = fopen("Sample.txt","r"); 
	
	while(fgets(line,255,fp)!=NULL){ 
		fgets(keyLine,255,fp);
		sscanf(line,"%s",link); // /n karakterini at
		
		char *ptr = strtok(keyLine, " \n");
		for(i=0; i<strlen(ptr); i++){		//küçük harfe çevir
			if(ptr[i]>='A' && ptr[i]<='Z')
				ptr[i] = ptr[i] -'A'+'a';
		}
		while (ptr != NULL){
			hashCode = hornerMethod(ptr)%hashSize;	//hashkodunu hesapla
			while(crawler[hashCode].size!=0 && strcmp(crawler[hashCode].key,ptr)!=0){ //hücre boþ deðilse ve keye eþit deðilse
				hashCode=(hashCode+1)%hashSize;	//bir sonraki hücreye bak
				crawler[hashCode].tries++;	//try sayýsýný arttýr
			}
				if(crawler[hashCode].size==0){	//hücre boþsa
					crawler[hashCode].size=1;
					strcpy(crawler[hashCode].key,ptr);
					crawler[hashCode].links = (char**)malloc(sizeof(char*));
					crawler[hashCode].links[0] = (char*)malloc(sizeof(char)*100);
					strcpy(crawler[hashCode].links[0],link);
					
				}else{ //hücredeki key gelen keyle aynýysa
					crawler[hashCode].links = (char**)realloc(crawler[hashCode].links,(crawler[hashCode].size+1)*sizeof(char*));
					crawler[hashCode].links[crawler[hashCode].size] = (char*)malloc(sizeof(char)*100);
					strcpy(crawler[hashCode].links[crawler[hashCode].size++],link); //linki link dizisinin sonuna ekle
				}	
			ptr = strtok(NULL, " \n");
		}
	}
}

void init(struct webCrawler *crawler, int n){	
	int i;
	for(i=0; i<n; i++){
		crawler[i].size=0;	//link sayýsý baþlangýçta 0
		crawler[i].tries=1;	//deneme sayýsý
	}
}

unsigned long long int hornerMethod(char* word){	//string to int
	int i,R=31;
	unsigned long long int result=0;
	int L=strlen(word);
	
	for(i=0; i<L; i++){
		if(word[i]>='A' && word[i]<='Z')	//küçük harfe çevirerek iþlem yap
			word[i] = word[i] -'A'+'a';
		result += (word[i] * pow(R,L-i-1));
	}
return result;
}

void findKey(struct webCrawler *crawler, int hashSize, char* key){	//arama yap
	int i,index = findKeyIndex(crawler,hashSize,key);
	
	for(i=0;i<crawler[index].size; i++)
		printf("%s\n", crawler[index].links[i]);
}

int findKeyIndex(struct webCrawler *crawler, int hashSize, char* key){	//hashkodundan keyi bul
	int hashCode = hornerMethod(key)%hashSize;
	int i=0;
	while(crawler[hashCode].size!=0 && strcmp(crawler[hashCode].key,key)!=0 && i<hashSize){ //linear probinge göre arar
			hashCode=(hashCode+1)%hashSize;
		}
	if(i==hashSize || crawler[hashCode].size==0){	//baþa dönmüþse key yoktur
		printf("Key could not found!\n");
		return -1;
	}else{
		return hashCode;	//bulunan index
	}
}

void findMultipleKeys(struct webCrawler *crawler, int hashSize, char* key1, char* op, char* key2){	//ve veya ile arama
	int i,j, index1 = findKeyIndex(crawler,hashSize,key1);	//key1 indeksini bul
	int index2 = findKeyIndex(crawler,hashSize,key2);		//key2 indeksini bul
	
	if(!strcmp(op,"veya")){		//operand 'veya' ise
		for(i=0; i<crawler[index1].size; i++){	//key1 tüm linklerini yazdýr
			printf(" %s\n",crawler[index1].links[i]);
		}
		for(j=0; j<crawler[index2].size; j++){	//key2 linklerini key1dekilerden farklýysa yazdýr
			if(!keyExists(crawler[index1].links,crawler[index2].links[j],crawler[index1].size))
				printf(" %s\n",crawler[index2].links[j]);
		}
		
	}else if(!strcmp(op,"ve")){		//operand 've' ise
		for(i=0; i<crawler[index1].size; i++){		//key1 ve key2 linkleri sadece aynýysa yazdýr
			for(j=0; j<crawler[index2].size; j++){
				if(!strcmp(crawler[index1].links[i],crawler[index2].links[j]))
					printf(" %s\n",crawler[index2].links[j]);
			}
		}
	}else{
		printf("Hatali bir giris yaptiniz");
		return;
	}
}
