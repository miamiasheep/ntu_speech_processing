#include "hmm.h"
#include <math.h>
void line(){
    printf("\n==================\n");
}
int main(int argc,char *argv[]){
    HMM hmms[5];

    int total,correct;
    int i,j,k,l,n;
    char sequence[10000][50];
    char answer[10000][15];
    const char *sequence_in=argv[2];
    const char *model_list=argv[1];
    const char *result_ans=argv[3];
    /***********************
    const char *sequence_in="testing_data1.txt";
    const char *model_list="modellist.txt";
    const char *result_ans="result.txt";
    *************************/
    load_models( model_list, hmms, 5);
	dump_models( hmms, 5);
	int count=0;
	int cc=0;
	char c;

    FILE *fp2=fopen("testing_answer.txt","r");
    while(1){
        c = fgetc(fp2);
        if((c>='a'&&c<='z')||c=='.'||c=='_'||(c>='0'&&c<='9')){
            answer[cc][count]=c;
            count++;
        }
        else if(c==EOF){
                break;
        }else{
            count=0;
            cc++;
            continue;
            }
        }
    fclose(fp2);
    FILE *fp = fopen(sequence_in,"r");
    //system("pause");
    cc=0;count=0;
    while(1){
        c = fgetc(fp);
        if(c>='A'&&c<='F'){
            sequence[cc][count]=c;
            count++;
        }
        else if(c==EOF){
                break;
        }else{
            count=0;
            cc++;
            continue;
            }
        }
    fclose(fp);
/**************************
for(i=0;i<cc;i++){
    printf("\n");
    for(j=0;j<50;j++){
        printf("%c ",sequence[i][j]);

    }
    system("pause");
}
*******************/
/*************************
for(i=0;i<cc;i++){
    printf("\n");
    for(j=0;j<12;j++){
        printf("%c ",answer[i][j]);

    }
    system("pause");
}
******************************/
//////////////////////////////////////////////viterbi algorithm
int ans[10000];
double viterbi[50][6];
double max,viterbi_max[6],temp;
int mark_max;
total=0;correct=0;
FILE *fp3=fopen(result_ans,"w");

for(n=0;n<cc;n++){
    for(l=0;l<5;l++){
        for(i=0;i<50;i++){
            for(j=0;j<6;j++){
                if(i==0){
                    viterbi[i][j]=hmms[l].initial[j]*hmms[l].observation[(int)(sequence[n][i]-'A')][j];
                }//if
                else{
                    max=0;
                    for(k=0;k<6;k++){
                        temp=viterbi[i-1][k]*hmms[l].transition[k][j]*hmms[l].observation[(int)(sequence[n][i]-'A')][j];
                        if(temp>=max)max=temp;
                    }
                    viterbi[i][j]=max;
                }//else
            }//for j
        }//for i
        viterbi_max[l]=0;
        for(i=0;i<6;i++){
           if(viterbi[49][i]>=viterbi_max[l])viterbi_max[l]=viterbi[49][i];
        }
    }//for l
    max=0.0;
    for(l=0;l<5;l++){
        if(viterbi_max[l]>=max){
            //printf("viterbi_max:%e \n",viterbi_max[l]);
            //system("pause");
            max=viterbi_max[l];
            mark_max=l;
        }
    }
    ans[n] = mark_max;
    total++;
    switch(mark_max){
        case 0:
            fprintf(fp3,"model_01.txt %e\n",max);
            if(strcmp(answer[n],"model_01.txt")==0)correct++;
            break;
        case 1:
            fprintf(fp3,"model_02.txt %e\n",max);
            if(strcmp(answer[n],"model_02.txt")==0)correct++;
            break;
        case 2:
            fprintf(fp3,"model_03.txt %e\n",max);
            if(strcmp(answer[n],"model_03.txt")==0)correct++;
            break;
        case 3:
            fprintf(fp3,"model_04.txt %e\n",max);
            if(strcmp(answer[n],"model_04.txt")==0)correct++;
            break;
        case 4:
            fprintf(fp3,"model_05.txt %e\n",max);
            if(strcmp(answer[n],"model_05.txt")==0)correct++;
            break;
    }
    /************
    printf("answer: ");
    for(i=0;i<12;i++){
        printf("%c",answer[n][i]);
    }

    printf("\n");
    printf("predict: %d\n",mark_max);
    printf("correct count:%d\n",correct);
    system("pause");
    ***********************/
    //if(n>=(cc-20))printf("%d %d \n",n,ans[0]);
    //printf("answer:%lf ",mark_max);
}// for n
double acc;
acc=(double)correct/(double)total;
printf("accuracy: %lf\n",acc);
FILE *fp4=fopen("acc.txt","w");
fprintf(fp4,"%lf \n",acc);
fclose(fp3);
fclose(fp4);
/*************************************8
printf("\n====================\n");
for(n=0;n<cc;n++){
    printf("%d %d\n",n,ans[n]);
    getchar();
}
*****************/
//getchar();
//////////////////////////////////////////////viterbi algorithm
    return 0;
}
