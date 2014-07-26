#include "hmm.h"
#include <math.h>
void line(){
    printf("\n==================\n");
}
int main(int argc,char *argv[])
{
    int iteration=atoi(argv[1]);
    const char *model_in=argv[2];
    const char *sequence_in=argv[3];
    const char *model_out=argv[4];

    int i,j,k,l,n;
    /*********************************************
    int iteration=30;
    const char *model_in="model_init.txt";
    const char *sequence_in="seq_model_01.txt";
    const char *model_out="model_01.txt";
*****************************************************/


/*
	HMM hmms[5];
	load_models( "modellist.txt", hmms, 5);
	dump_models( hmms, 5);
*/
	HMM hmm_initial;
    loadHMM( &hmm_initial, model_in );
    dumpHMM( stderr, &hmm_initial );
	///////////////////////////////////////////////
	double forward[50][6];
	double backward[50][6];
	double initial[6];
	char sequence[10000][50];
	int count=0;
	int cc=0;
	char c;
    FILE *fp = fopen(sequence_in,"r");
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
/***************************************
    for(i=0;i<10000;i++){
        for(j=0;j<50;j++){
            printf("%d ",sequence[i][j]);
            system("pause");
        }
    }
**************************************/

////////////////////////set sumga and sumep
double sumga[50][6];
double sumep[50][6][6];
double sumgaOK[6][6];

//////////////////////set sumga and sumep
/////////////////////////perform the forward algorithm
/******************************************
int ii;
for( ii=0;ii<5;ii++){
        switch(i){
            case 0:
                sequence_in="seq_model_01.txt";
                model_out="model_01.txt";
                break;
            case 1:
                sequence_in="seq_model_02.txt";
                model_out="model_02.txt";
                break;
            case 2:
                sequence_in="seq_model_03.txt";
                model_out="model_03.txt";
                break;
            case 3:
                sequence_in="seq_model_04.txt";
                model_out="model_04.txt";
                break;
            case 4:
                sequence_in="seq_model_05.txt";
                model_out="model_05.txt";
                break;
        }
        loadHMM( &hmm_initial, model_in );
        dumpHMM( stderr, &hmm_initial );
*************************************/
for(l=0;l<iteration;l++){//////how many iteration
    for(j=0;j<6;j++){
        for(k=0;k<6;k++){
            sumgaOK[j][k]=0;
        }
    }
    for(i=0;i<50;i++){
        for(j=0;j<6;j++){
            sumga[i][j]=0;
            for(k=0;k<6;k++){
                sumep[i][j][k]=0;
            }
        }
    }
for(n=0;n<10000;n++){////////for n
//////////////////////////set forward and backward variable to zero
    for(i=0;i<50;i++){
        for(j=0;j<6;j++){
            forward[i][j]=0.0;
            backward[i][j]=0.0;
        }
    }
/////////////////////////set forward and backward variable to zero
//////////////////////////forward algorithm
    for(i=0;i<50;i++){
        for(j=0;j<6;j++){
            if(i==0){
                forward[i][j]=hmm_initial.initial[j]*hmm_initial.observation[(int)(sequence[n][i]-'A')][j];
            }else{
                for(k=0;k<6;k++){
                    forward[i][j]+=forward[i-1][k]*hmm_initial.transition[k][j]*hmm_initial.observation[(int)(sequence[n][i]-'A')][j];
                }// for k
             }// for else
         }// for j
    }// for i
///////////////////////////////////////////////////forward algorithm
///////////////////////////////////////////////////backward algorithm
for(i=49;i>=0;i--){
    for(j=0;j<6;j++){
        if(i==49){
            backward[i][j]=1;
        }
        else{
            for(k=0;k<6;k++){
                backward[i][j]+=hmm_initial.transition[j][k]*hmm_initial.observation[(int)(sequence[n][i+1]-'A')][k]*backward[i+1][k];
            }
        }
    }
}
/////////////////////////////////////////////////backward algorithm
/******************************
    line();
    for(i=0;i<50;i++){
        for(j=0;j<6;j++){
            printf("%12e ",forward[i][j]);
        }
        printf("\n");
    }
    line();
    for(i=0;i<50;i++){
        for(j=0;j<6;j++){
            printf("%12e ",backward[i][j]);
        }
        printf("\n");
    }
   // system("pause");
   *****************************/
/////////////////////////////////////////////////gamma calculation
double temp=0.0;
double gamma[50][6];
for(i=0;i<50;i++){
    temp=0.0;
    for(j=0;j<6;j++){
        temp+=forward[i][j]*backward[i][j];
    }// for j
    for(j=0;j<6;j++){
        gamma[i][j]=(forward[i][j]*backward[i][j])/temp;
    }
}//for i
/**************************
for(i=0;i<50;i++){
    printf("\n");
    for(j=0;j<6;j++){
        printf("%lf ",gamma[i][j]);
    }
}
system("pause");
*******************************/
/////////////////////////////////////////////////gamma calculation
/////////////////////////////////////////////////gammaOK calculation
double gammaOK[6][6];
for(j=0;j<6;j++){
    for(k=0;k<6;k++){
        temp=0;
        for(i=0;i<50;i++){
            if((int)(sequence[n][i]-'A')==k){
                temp+=gamma[i][j];
            }
            //tempG+=gamma[i][j];
        }
        gammaOK[j][k]=temp;
    }
}
/********************************
temp=0;
for(j=0;j<6;j++){
    printf("\n");
    for(k=0;k<6;k++){
        printf("%lf ",gammaOK[j][k]);
        temp+=gammaOK[j][k];
    }
}
printf("\n temp=%lf ",temp);
printf("\n");
system("pause");
**********************************/

////////////////////////////////////////////////gammaOK calculation
/////////////////////////////////////////////////ep caluculation
double ep[50][6][6];
for(i=0;i<49;i++){
    temp=0;
    for(j=0;j<6;j++){/////j is equal to i in the power point
        for(k=0;k<6;k++){////////k is equal to j in the power point
            temp+=forward[i][j]*hmm_initial.transition[j][k]*hmm_initial.observation[(int)(sequence[n][i+1]-'A')][k]*backward[i+1][k];
        }
    }
    for(j=0;j<6;j++){
        for(k=0;k<6;k++){
            ep[i][j][k]=(forward[i][j]*hmm_initial.transition[j][k]*hmm_initial.observation[(int)(sequence[n][i+1]-'A')][k]*backward[i+1][k])/temp;
        }
    }

}
/******************
temp=0;
for(i=0;i<50;i++){
    temp=0;
    for(j=0;j<6;j++){
        for(k=0;k<6;k++){
            temp+=ep[i][j][k];
        }
    }
    printf("%lf\n",temp);
}
**************************/
/////////////////////////////////////////////////ep calculation
///////////////////////////////////////////////////calculate sumga and sumep
for(i=0;i<50;i++){
    for(j=0;j<6;j++){
        sumga[i][j]+=gamma[i][j];
        for(k=0;k<6;k++){
            sumep[i][j][k]+=ep[i][j][k];
        }
    }
}
for(j=0;j<6;j++){
    for(k=0;k<6;k++){
        sumgaOK[j][k]+=gammaOK[j][k];
    }
}
/************************************
temp=0;
for(j=0;j<6;j++){
    printf("\n");
    for(k=0;k<6;k++){
        printf("%lf ",sumgaOK[j][k]);
        temp+=sumgaOK[j][k];
    }
}
printf("\n n=%d temp%lf ",n,temp);
system("pause");
***************************************/


///////////////////////////////////////////////////calculate sumga and sumep
}///////////////////////////////////////for n!! sequence!!

///////////////////////////////////////////////transition and initial

double temp1=0;
double temp2=0;
double temp=0;
for(i=0;i<6;i++){
    hmm_initial.initial[i]=sumga[0][i]/10000;
    temp+=hmm_initial.initial[i];
}
//printf("initial Total:%lf \n",temp);
for(j=0;j<6;j++){
    temp1=0;
    for(i=0;i<49;i++){
        temp1+=sumga[i][j];
    }
    for(k=0;k<6;k++){
        temp2=0;
        for(i=0;i<49;i++){
            temp2+=sumep[i][j][k];
        }
        hmm_initial.transition[j][k]=temp2/temp1;
    }
}
/****************************
for(j=0;j<6;j++){
    temp=0;
    printf("\n");
    for(k=0;k<6;k++){
        temp+=hmm_initial.transition[j][k];
        printf("%lf ",hmm_initial.transition[j][k]);

    }
    printf("\n");
    printf("transition total: %lf \n ",temp);
}
******************************/
/////////////////////////////////////////////////////caluculate new observation
temp1=0;
for(j=0;j<6;j++){
    temp1=0;

    for(i=0;i<50;i++){
        temp1+=sumga[i][j];
    }

    for(k=0;k<6;k++){
        hmm_initial.observation[k][j]=sumgaOK[j][k]/temp1;
    }
}
/**************************
for (j=0;j<6;j++){
    temp=0;
    for(k=0;k<6;k++){
        temp+=hmm_initial.observation[k][j];
    }
    printf("observation total: %lf\n",temp);
}
system("pause");
*********************************/
/////////////////////////////////////////////////////calculate new observation
}////////////////////////////////////////////////////////////////////////for iter
//}////////////////////////////////////////////////////model setting
double temp=0;
printf("hmm_initial:\n");
FILE *output=fopen(model_out,"w");
fprintf(output,"initial: 6\n");
for(i=0;i<6;i++){
    //hmm_initial.initial[i]=sumga[0][i]/10000;
    temp+=hmm_initial.initial[i];
    printf("%lf ",hmm_initial.initial[i]);
    fprintf(output,"%lf ",hmm_initial.initial[i]);
}
    fprintf(output,"\n");
    fprintf(output,"\n");

printf("\ntemp:\n%lf",temp);
fprintf(output,"transition: 6\n");
double temp1,temp2;
line();
for(i=0;i<6;i++){
    printf("\n");
    temp=0;
    for(j=0;j<6;j++){
        printf("%lf ",hmm_initial.transition[i][j]);
        fprintf(output,"%lf ",hmm_initial.transition[i][j]);
        temp+=hmm_initial.transition[i][j];
    }
    //line();
    fprintf(output,"\n");
    printf("\ntemp=%lf\n",temp);
    //line();
}
line();
fprintf(output,"\n");

///////////////////////////////////////////////////////transition and initial
//////////////////////////////////////////////////////print new observation

double tempA[6];
fprintf(output,"observation: 6\n");
for(i=0;i<6;i++)tempA[i]=0;
for(j=0;j<6;j++){
    printf("\n");
    for(k=0;k<6;k++){
        printf("%lf ",hmm_initial.observation[j][k]);
        fprintf(output,"%lf ",hmm_initial.observation[j][k]);
        tempA[k]+=hmm_initial.observation[j][k];
    }
    fprintf(output,"\n");
}
line();
for(i=0;i<6;i++){
    printf("%lf ",tempA[i]);
}

/////////////////////////////////////////////////////print new ovservation
    printf("\n");
    /////////////////
	//printf("%f\n", log(1.5) ); // make sure the math library is included
	return 0;
}
