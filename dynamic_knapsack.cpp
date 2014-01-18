#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

bool test_bit(char ch, int which_bit){
	switch(which_bit){
		case 0: ch &= 0x80; break;
		case 1: ch &= 0x40; break;
		case 2: ch &= 0x20; break;
		case 3: ch &= 0x10; break;
		case 4: ch &= 0x08; break;
		case 5: ch &= 0x04; break;
		case 6: ch &= 0x02; break;
		case 7: ch &= 0x01; break;
		}
	if(ch)
		return true;
	else
		return false;
}

void turn_seq(unsigned char** M_arr, double** X, double capacity, int N, int* seq){
	int i,which_seg,which_bit;
	double cap;
	for(i = N; i>0; i--){
		cap = X[1][i-1];
		which_seg = capacity/8;
		which_bit = capacity - which_seg*8;
		if(test_bit(M_arr[i][which_seg],which_bit)){
			seq[i-1] = 1;
			capacity-=cap;
		}
		else seq[i-1] = 0;
	}
}


char set_bit(char ch, int which_bit, bool bit){
	if (bit){
		switch(which_bit){
		case 0: ch |= 0x80; break;
		case 1: ch |= 0x40; break;
		case 2: ch |= 0x20; break;
		case 3: ch |= 0x10; break;
		case 4: ch |= 0x08; break;
		case 5: ch |= 0x04; break;
		case 6: ch |= 0x02; break;
		case 7: ch |= 0x01; break;
		}}
	else{
		switch(which_bit){
		case 0: ch &= 0x7F; break;
		case 1: ch &= 0xBF; break;
		case 2: ch &= 0xDF; break;
		case 3: ch &= 0xEF; break;
		case 4: ch &= 0xF7; break;
		case 5: ch &= 0xFB; break;
		case 6: ch &= 0xFD; break;
		case 7: ch &= 0xFE; break;
		}}
	return(ch);
}

int main(int argc, char** argv){
	int i,N,K,w;
	double cap;
	string line;
	char *pch;
	double **X;
	double **M;
	
	//cout << "Ben calisiyorum sorun yok." << "\n";
	X = (double**)malloc(2*sizeof(double*));
	//ifstream file_ ("C:\\Users\\daredavil\\Desktop\\Discrete Opt\\testsistem\\ks\\data\\ks_30_0");
	ifstream file_ (argv[1]);
	if(file_.is_open())
	{
		i = 0;
		while(getline(file_,line)){
			if (i != 0){
			pch = strtok((char*)(line.c_str())," ");
			X[0][i-1] = atoi(pch);
			pch = strtok(NULL," ");
			X[1][i-1] = atoi(pch);}
			else{
				pch = strtok((char*)(line.c_str())," ");
				N = atoi(pch);
				pch = strtok(NULL," ");
				K = atoi(pch);
				X[0] = (double*)malloc(N*sizeof(double));
				X[1] = (double*)malloc(N*sizeof(double));
			}
			i++;
		}
	}
	else{
		cout << "Unable to open file" << "\n";
		free(X);
		exit(1);
	}

	int segment_len = (int)(std::ceil((float)((K+1)/8)) + 1);
	unsigned char** M_arr;
	M_arr = (unsigned char**)malloc((N+1)*sizeof(unsigned char*));
	for(i=0;i<N+1;i++){
		M_arr[i] = (unsigned char*)malloc(segment_len*sizeof(unsigned char));
		if(M_arr[i] == NULL){
			printf("not allocated\n");
			exit(1);
		}
	}


	M = (double**)malloc((N+1)*sizeof(double*));
	//for(i=0;i<N+1;i++)
	//	M[i] = (double*)malloc((K+1)*sizeof(double));
	M[0] = (double*)malloc((K+1)*sizeof(double));
	for(w = 0;w<K+1;w++)
		M[0][w] = 0;
	//for(i=0;i<N+1;i++)
	//	M[i][0] = 0;
	int which_seg, which_bit, value;
	for(i=1;i<N+1;i++){
		M[i] = (double*)malloc((K+1)*sizeof(double)); M[i][0] = 0;
		if(M[i] == NULL) printf("not allocated\n");
		if (i-2>=0)
			free(M[i-2]);
		for(w=0;w<K+1;w++){
			cap = X[1][i-1];
			which_seg = w/8;
			which_bit = w - which_seg*8;
			if (cap > w){
				M[i][w] = M[i-1][w];
				M_arr[i][which_seg] = set_bit(M_arr[i][which_seg],which_bit, false);
			}
			else{
				if (M[i-1][w] > (X[0][i-1] + M[i-1][(int)(w-cap)])){
					M[i][w] = M[i-1][w];
					M_arr[i][which_seg] = set_bit(M_arr[i][which_seg],which_bit, false);
				}
				else{
					M[i][w] = X[0][i-1] + M[i-1][(int)(w-cap)];
					M_arr[i][which_seg] = set_bit(M_arr[i][which_seg],which_bit, true);
				}
				}
			}
	}
	int *seq;
	seq = (int*)malloc(N*sizeof(int));
	turn_seq(M_arr,X,K,N,seq);
	printf("%d\n",(int)M[N][K]);
	double sum = 0;
	for(i=0;i<N;i++){
		if (seq[i]==1)
			sum += X[0][i];
		printf("%d ",seq[i]);
	}
	//printf("\n%f\n",sum);
	//std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	free(seq);
	free(X[0]);
	free(X[1]);
	free(X);
	free(M[N]); free(M[N-1]);
	free(M);
	for(i = 0;i<N+1;i++)
		free(M_arr[i]);
	free(M_arr);
	return 0;
}