#include <iostream>
#include <vector>
#include "cnn.h"
#define rep(i,a,b) for( int i=a; i<b;i++)
// #define PRINT


using namespace std;


int main(int argc, char* argv[]){
    string inputfile_txt, weightsfile_txt, strucutrefile_txt;
    vector<vector<long double> > input;
    CNN cnn;
    if(argc!=5){
        puts("Please follow this format ./run <input.txt> <weights.txt> <structure.txt> <alpha>\n");
        exit(1);
    }else{
        inputfile_txt = argv[1];
        weightsfile_txt =argv[2];
        strucutrefile_txt = argv[3];
        cnn.alpha = atof(argv[4]);
    }

    //get info from the files ----------------------------------------------------------------------

    cnn.getReady_input(inputfile_txt, input);
    cnn.getReady_weight(weightsfile_txt);
    cnn.getReady_structure(strucutrefile_txt);
    //done reading info from files-------------------------------------------------------------------
    //show the input file
    #ifdef PRINT
    rep(i,0,(int)input.size()){
        rep(j,0,(int)input[i].size()){
            if(j%(cnn.MatrixSize[0]) ==0 && j!=0) printf("\n");
            printf("%5.1Lf ",input[i][j]);    
        }
        printf("\n");
    }
    #endif



    //show the structure file


    //check the max channels so that I can know how many hidden channels are needed
    int max_channels=0;
    rep(i, 0, (int)cnn.LayerID.size()){
        max_channels = max(max_channels, cnn.Channels[i]);
    }
    
    
    rep(k,0,(int)input.size()){
        int weight_count=0;
        vector<vector<long double> > hidden;
        vector<vector<long double> > output;
        vector<long double> final_output;
        rep(i,0,max_channels){
        vector<long double> temp;
            hidden.push_back(temp);
        } 

        rep(i,1,(int)cnn.LayerID.size()){
        if(cnn.LayerType[i] == "C"){
            rep(j,0,cnn.Channels[i]){
                if(i==1){
                    hidden[j] = cnn.convolutionFunction(input[k], i, j, weight_count, true);
                    weight_count++;
                }
                else{
                    // if(Channels[i]>=1){
                    vector<vector<long double> > temp;
                    rep(h,0, (int)hidden.size()){
                        temp.push_back(cnn.convolutionFunction(hidden[h], i, h, weight_count, false));   
                    }
                    weight_count++;
                    output.push_back(cnn.getConvResult(temp, i));
                    #ifdef PRINT
                    printf("\n");
                    rep(n,0,(int)output.size()){
                        rep(m,0,(int)output[n].size()){
                            if(m!=0 && m%cnn.MatrixSize[i]==0) printf("\n");
                            printf("%1.16Lf ", output[n][m]);
                        }
                    }
                    printf("\n");
                    #endif
                } 

            }
        }else if (cnn.LayerType[i] == "A"){
            rep(j,0,cnn.Channels[i]){
                if(i==1)
                    hidden[j] = cnn.avgPoolFunciton(input[k],i,j);
                else{
                    hidden[j] = cnn.avgPoolFunciton(hidden[j],i,j);
                }
                    // avgPoolFunciton(hidden[j],i,j);
            }
        }else if (cnn.LayerType[i] == "M"){
            rep(j,0,cnn.Channels[i]){
                if(i==1)
                   hidden[j] = cnn.maxPoolFunciton(input[k],i,j);
                else
                   hidden[j] = cnn.maxPoolFunciton(hidden[j],i,j);
            }
        }else if (cnn.LayerType[i] == "F"){
            rep(j,0,cnn.Channels[i]){
                if(i==1)
                    hidden[j] = cnn.fullyConnectedFunction(input[k],i,j,weight_count);
                else{
                    // printf("output size is %lu\n", output.size());
                    final_output = cnn.fullyConnectedFunction(output,i,j, weight_count);
                }
                    
            }
        }
        
       if(i == (int)cnn.LayerID.size()-1){
           if(i==1){
               rep(a,0, (int)hidden.size()){
                   cnn.printFunction(hidden[a]);
               }
           }
           else if(cnn.LayerType[i] == "C"){
               rep(a,0, (int)output.size()){
                   cnn.printFunction(output[a]);
               }

           }else if (cnn.LayerType[i] == "A"){
               rep(a,0, (int)hidden.size()){
                   cnn.printFunction(hidden[a]);
               }
           }else if (cnn.LayerType[i] == "M"){
                rep(a,0, (int)hidden.size()){
                   cnn.printFunction(hidden[a]);
               }
           }else if (cnn.LayerType[i] == "F"){
                cnn.printFunction(final_output);
           }
       } 
    }
        
    }
        #ifdef PRINT
        cout << "LayerID" <<" ";
        cout << "LayerType" <<" ";
        cout << "TotalFilters" <<" ";
        cout << "FilterSize" <<" ";
        cout << "Stride" <<" ";
        cout << "MatrixSize" <<" ";
        cout << "Channels" <<" ";
        cout << "ActivationType" <<" ";
        cout << "Bias" <<endl;
    rep(i,0,cnn.LayerID.size()){
        printf("%7.1d ",cnn.LayerID[i]);
        printf("%9.1s ",cnn.LayerType[i].c_str());
        printf("%9.1d",cnn.TotalFilters[i]);
        printf("%14.1d",cnn.FilterSize[i]);
        printf("%7.1d",cnn.Stride[i]);
        printf("%11.1d",cnn.MatrixSize[i]);
        printf("%9.1d",cnn.Channels[i]);
        printf("%13.1d",cnn.ActivationType[i]);
        printf("%7.2Lf\n",cnn.b[i]);
    }
    #endif
    return 0;
}

