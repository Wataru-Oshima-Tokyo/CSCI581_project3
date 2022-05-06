#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#define rep(i,a,b) for(int i=a; i<b;i++)
// #define PRINT


using namespace std;

long double alpha;
// 
vector<vector<long double> > weights;
vector<int> LayerID, TotalFilters, FilterSize, Stride, MatrixSize, Channels, ActivationType;
vector<string> LayerType;
vector<long double> b;


void getReady_input(string inputfile_txt,vector<vector<long double> > &input);
void getReady_weight(string weightsfile_txt);
void getReady_structure(string strucutrefile_txt);
vector<long double>  convolutionFunction(vector<long double> &input, int i, int j, int w, bool t);
vector<long double>  avgPoolFunciton(vector<long double> &input, int i, int j);
vector<long double>  maxPoolFunciton(vector<long double> &input, int i, int j);
vector<long double> fullyConnectedFunction(vector<long double> &input,int, int, int);
vector<long double> fullyConnectedFunction(vector<vector<long double> > &input,int i, int j, int w);
void printFunction(vector<long double> &output);
vector<long double> getConvResult(vector<vector<long double> >, int);
template <typename T> T tanh(T z);
template <typename T> T sigmoid(T z);


int main(int argc, char* argv[]){
    string inputfile_txt, weightsfile_txt, strucutrefile_txt;
    vector<vector<long double> > input;

    if(argc!=5){
        puts("Please follow this format ./run <input.txt> <weights.txt> <structure.txt> <alpha>\n");
        exit(1);
    }else{
        inputfile_txt = argv[1];
        weightsfile_txt =argv[2];
        strucutrefile_txt = argv[3];
        alpha = atof(argv[4]);
    }

    //get info from the files ----------------------------------------------------------------------

    getReady_input(inputfile_txt, input);
    getReady_weight(weightsfile_txt);
    getReady_structure(strucutrefile_txt);
    //done reading info from files-------------------------------------------------------------------
    //show the input file
    #ifdef PRINT
    rep(i,0,input.size()){
        rep(j,0,input[i].size()){
            if(j%(MatrixSize[0]) ==0 && j!=0) printf("\n");
            printf("%5.1Lf ",input[i][j]);    
        }
        printf("\n");
    }
    #endif



    //show the structure file


    //check the max channels so that I can know how many hidden channels are needed
    int max_channels=0;
    rep(i, 0, LayerID.size()){
        max_channels = max(max_channels, Channels[i]);
    }
    
    
    rep(k,0,input.size()){
        int weight_count=0;
        vector<vector<long double> > hidden;
        vector<vector<long double> > output;
        vector<long double> final_output;
        rep(i,0,max_channels){
        vector<long double> temp;
            hidden.push_back(temp);
        } 

        rep(i,1,LayerID.size()){
        if(LayerType[i] == "C"){
            rep(j,0,Channels[i]){
                if(i==1){
                    hidden[j] = convolutionFunction(input[k], i, j, weight_count, true);
                    weight_count++;
                }
                else{
                    // if(Channels[i]>=1){
                    vector<vector<long double> > temp;
                    rep(h,0, hidden.size()){
                        temp.push_back(convolutionFunction(hidden[h], i, h, weight_count, false));   
                    }
                    weight_count++;
                    output.push_back(getConvResult(temp, i));
                    #ifdef PRINT
                    printf("\n");
                    rep(n,0,output.size()){
                        rep(m,0,output[n].size()){
                            if(m!=0 && m%MatrixSize[i]==0) printf("\n");
                            printf("%1.16Lf ", output[n][m]);
                        }
                    }
                    printf("\n");
                    #endif

                    // }else{
                        // vector<vector<long double>> temp;
                        // int w= weight_count;
                        // rep(h,0, Channels[i]){
                        //     temp.push_back(convolutionFunction(hidden[j], i, j, weight_count, false));
                        //     weight_count++;
                        // }
                        

                        // weight_count=w;
                        // hidden[j].clear();
                        // hidden[j] = temp[0];
                        // rep(q,0,hidden[j].size()){
                        //     hidden[j][q]=0;
                        // }
                        // rep(q,0,temp.size()){
                        //     rep(r,0,temp[q].size()){
                        //         hidden[j][r]+=temp[q][r];
                        //         printf("%1.10LF ", hidden[j][r]);
                        //     }
                        // }
                        // printf("\n");
                        // rep(q,0,hidden[j].size()){
                        //     hidden[j][q] +=b[i];
                        //     if(ActivationType[i]==1){
                        //         hidden[j][q] = tanh<long double>(hidden[j][q]);
                        //     }else{
                        //         hidden[j][q] = sigmoid<long double>(hidden[j][q]);
                        //     }
                        // }

                    // }
                } 

            }
        }else if (LayerType[i] == "A"){
            rep(j,0,Channels[i]){
                if(i==1)
                    hidden[j] = avgPoolFunciton(input[k],i,j);
                else{
                    hidden[j] = avgPoolFunciton(hidden[j],i,j);
                }
                    // avgPoolFunciton(hidden[j],i,j);
            }
        }else if (LayerType[i] == "M"){
            rep(j,0,Channels[i]){
                if(i==1)
                   hidden[j] = maxPoolFunciton(input[k],i,j);
                else
                   hidden[j] = maxPoolFunciton(hidden[j],i,j);
            }
        }else if (LayerType[i] == "F"){
            rep(j,0,Channels[i]){
                if(i==1)
                    hidden[j] = fullyConnectedFunction(input[k],i,j,weight_count);
                else{
                    // printf("output size is %lu\n", output.size());
                    final_output = fullyConnectedFunction(output,i,j, weight_count);
                }
                    
            }
        }
        
       if(i == LayerID.size()-1){
           if(i==1){
               rep(a,0, hidden.size()){
                   printFunction(hidden[a]);
               }
           }
           else if(LayerType[i] == "C"){
               rep(a,0, output.size()){
                   printFunction(output[a]);
               }

           }else if (LayerType[i] == "A"){
               rep(a,0, hidden.size()){
                   printFunction(hidden[a]);
               }
           }else if (LayerType[i] == "M"){
                rep(a,0, hidden.size()){
                   printFunction(hidden[a]);
               }
           }else if (LayerType[i] == "F"){
            //     rep(a,0, output.size()){
            //        printFunction(output[a]);
            //    }
                printFunction(final_output);

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
    rep(i,0,LayerID.size()){
        printf("%7.1d ",LayerID[i]);
        printf("%9.1s ",LayerType[i].c_str());
        printf("%9.1d",TotalFilters[i]);
        printf("%14.1d",FilterSize[i]);
        printf("%7.1d",Stride[i]);
        printf("%11.1d",MatrixSize[i]);
        printf("%9.1d",Channels[i]);
        printf("%13.1d",ActivationType[i]);
        printf("%7.2Lf\n",b[i]);
    }
    #endif
    return 0;
}


void getReady_input(string inputfile_txt, vector<vector< long double> > &input){
    //get the image from input and put to vector
    ifstream input_file(inputfile_txt);
    if (input_file.is_open())
    {  
        string temp;
        while(getline(input_file, temp)){
            vector<long double> temp_vec;
            std::istringstream iss(temp);
            string tempint;
            while(iss >> tempint){
                temp_vec.push_back(stold(tempint.c_str()));
                // cout << tempint <<endl;
            }
            input.push_back(temp_vec);
        }
    }
    input_file.close();
}
void getReady_weight(string weightsfile_txt){
    //get the weights from weights file and put to vector
    ifstream weight_file(weightsfile_txt);
    if (weight_file.is_open())
    {  
        string temp;
        while(getline(weight_file, temp)){
            vector<long double> temp_vec;
            std::istringstream iss(temp);
            string tempint;
            while(iss >> tempint){
                temp_vec.push_back(stold(tempint.c_str()));
            }
            weights.push_back(temp_vec);
        }
    }
    weight_file.close();
}
void getReady_structure(string strucutrefile_txt){
    //get the structure from structure file and put to vector
    ifstream structure_file(strucutrefile_txt);
    if (structure_file.is_open())
    {  
        string temp;
        while(getline(structure_file, temp)){
            std::istringstream iss(temp);
            string tempint;
            int count=0;
            while(iss >> tempint){
                if(count==0){
                    LayerID.push_back(atoi(tempint.c_str()));
                }else if (count==1){
                    LayerType.push_back(tempint.c_str());
                }else if (count==2){
                    TotalFilters.push_back(atoi(tempint.c_str()));
                }else if (count==3){
                    FilterSize.push_back(atoi(tempint.c_str()));
                }else if (count==4){
                    Stride.push_back(atoi(tempint.c_str()));
                }else if (count==5){
                    MatrixSize.push_back(atoi(tempint.c_str()));
                }else if (count==6){
                    Channels.push_back(atoi(tempint.c_str()));
                }else if (count==7){
                    ActivationType.push_back(atoi(tempint.c_str()));
                }else if (count==8){
                    b.push_back(atof(tempint.c_str()));
                }
                count++;
            }
        }
    }
    structure_file.close();
}

vector<long double> getConvResult(vector<vector<long double> > hidden, int i){
    vector<long double> _output;
    rep(x,0,hidden[0].size()){
        long double temp=0;
        rep(y,0,hidden.size()){
            temp+= hidden[y][x];
        }
        temp += b[i];
        if(ActivationType[i]==1){
            temp = tanh<long double>(temp);
        }else{
            temp = sigmoid<long double>(temp);
        }
        _output.push_back(temp);
    }
    // rep(n,0,_output.size()){
    //     if(n!=0 && n%MatrixSize[i]==0) printf("\n");
    //         printf("%10.6Lf ", _output[n]);
    // }
    return _output;
}

vector<long double> convolutionFunction(vector<long double> &input, int i, int j, int w, bool t){
    int col=0;
    int i_h_count=0;
    int i_v_count=0;
    int count=0;
    int w_h_count=0;
    vector<long double> _output;
    while(1){
        #ifdef PRINT
        // printf("\nArea %d\n", count);
        #endif
        // printf("col %d\n", col);
        long double result=0;
        int loop_counter=0;
        for(;;){
            #ifdef PRINT
            // printf("%1.6Lf ",input[col]);
            // printf("%4.6Lf ",weights[w][w_h_count]);
            #endif
            result+= input[col] * weights[w][w_h_count];
            col++;
            w_h_count++;
            if(loop_counter!=0 && (loop_counter+1)%FilterSize[i]==0){
                col += MatrixSize[i-1] -FilterSize[i];
                #ifdef PRINT
                // printf("\n");
                #endif
            }
            loop_counter++;
            if(loop_counter==FilterSize[i]*FilterSize[i]) break;
        }
        #ifdef PRINT
        // printf("\n");
        #endif
        if(t){
            result+=b[i];
            // printf("result is %Lf\n", result);
            if(ActivationType[i]==1){
                result = tanh<long double>(result);
            }else{
                result = sigmoid<long double>(result);
            }
        }
        _output.push_back(result); 
        i_h_count++;
        w_h_count=0;
        col=i_v_count*MatrixSize[i-1]+ i_h_count*Stride[i];
        if (i_h_count%MatrixSize[i]==0){
                i_v_count+=Stride[i];
                i_h_count=0;
                w_h_count=0;
                col = i_v_count*MatrixSize[i-1];
        } 
        count++;
        if(count==MatrixSize[i]*MatrixSize[i]) break;
    }


    
    #ifdef PRINT
    if(i ==1){
        printf("\n%d st iteration: ",i);
    }else if(i==2){
        printf("\n%d nd iteration: ",i);
    }else if(i==3){
        printf("\n%d rd iteration: ",i);
    }else{
        printf("\n%d th iteration: ",i);
    }
    if(j ==1){
        printf("%d st channel convolutional filtered output\n", j);
    }else if(j==2){
        printf("%d nd channel convolutional filtered output\n", j);
    }else if(j==3){
        printf("%d rd channel convolutional filtered output\n", j);
    }else{
        printf("%d th channel convolutional filtered output\n", j);
    }
    rep(n,0,_output.size()){
        if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%1.16Lf ", _output[n]);
    }
    printf("\n");
    #endif
    return _output;
}

vector<long double> avgPoolFunciton(vector<long double> &input, int i, int j){
    int col=0;
    int row=0;
    int i_h_count=0;
    int i_v_count=0;
    int count=0;
    vector<long double> _output;
    while(1){
        // printf("\nAverage Filter %d\n", count);
        long double result=0;
        int loop_counter=0;
        for(;;){
            // printf("%4.6Lf ",input[col]);
            result+= input[col];
            col++;
            if(loop_counter!=0 && (loop_counter+1)%FilterSize[i]==0){
                col += MatrixSize[i-1] -FilterSize[i];
            }
            loop_counter++;
            if(loop_counter==FilterSize[i]*FilterSize[i]) break;

        }
        result = result/(FilterSize[i]*FilterSize[i]);
        _output.push_back(result); 
        i_h_count++;
        col=i_v_count*MatrixSize[i-1]+ i_h_count*Stride[i];
        row=0;
        if (i_h_count%MatrixSize[i]==0){
                i_v_count+=Stride[i];
                i_h_count=0;
                col = i_v_count*MatrixSize[i-1];
        } 
        count++;
        if(count==MatrixSize[i]*MatrixSize[i]) break;
    }

    #ifdef PRINT
    if(i ==1){
        printf("\n%d st iteration: ",i);
    }else if(i==2){
        printf("\n%d nd iteration: ",i);
    }else if(i==3){
        printf("\n%d rd iteration: ",i);
    }else{
        printf("\n%d th iteration: ",i);
    }
    if(j ==1){
        printf("%d st channel average filtered output\n", j);
    }else if(j==2){
        printf("%d nd channel average filtered output\n", j);
    }else if(j==3){
        printf("%d rd channel average filtered output\n", j);
    }else{
        printf("%d th channel average filtered output\n", j);
    }
    rep(n,0,_output.size()){
        // if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%1.16Lf ", _output[n]);
    }
    printf("\n");
    #endif
    return _output;
}

vector<long double> maxPoolFunciton(vector<long double> &input, int i, int j){
    int col=0;
    int row=0;
    int i_h_count=0;
    int i_v_count=0;
    int count=0;
    vector<long double> _output;
    while(1){
        // printf("\nMax Filter %d\n", count);
        long double result=-INT16_MAX;
        int loop_counter=0;
        for(;;){
            // printf("%4.6Lf ",input[col]);
            if(result<input[col]) result = input[col];
            col++;
            if(loop_counter!=0 && (loop_counter+1)%FilterSize[i]==0){
                col += MatrixSize[i-1] -FilterSize[i];
                // printf("\n");
            }
            loop_counter++;
            if(loop_counter==FilterSize[i]*FilterSize[i]) break;

        }
        _output.push_back(result); 
        i_h_count++;
        col=i_v_count*MatrixSize[i-1]+ i_h_count*Stride[i];
        row=0;
        if (i_h_count%MatrixSize[i]==0){
                i_v_count+=Stride[i];
                i_h_count=0;
                col = i_v_count*MatrixSize[i-1];
        } 
        count++;
        if(count==MatrixSize[i]*MatrixSize[i]) break;
    }
    #ifdef PRINT
    if(i ==1){
        printf("\n%d st iteration: ",i);
    }else if(i==2){
        printf("\n%d nd iteration: ",i);
    }else if(i==3){
        printf("\n%d rd iteration: ",i);
    }else{
        printf("\n%d th iteration: ",i);
    }
    if(j ==1){
        printf("%d st channel max filtered output\n", j);
    }else if(j==2){
        printf("%d nd channel max filtered output\n", j);
    }else if(j==3){
        printf("%d rd channel max filtered output\n", j);
    }else{
        printf("%d th channel max filtered output\n", j);
    }
    rep(n,0,_output.size()){
        // if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%1.16Lf ", _output[n]);
    }
    printf("\n");
    #endif
    return _output;
}

vector<long double> fullyConnectedFunction(vector<long double> &input,int i, int j, int w){
    vector<long double > result;
    int w_count =w;
    int a=0;
    while(1){
        long double temp=0;
        int x=0;
        while(1){
            // puts("here\n");
            temp +=input[x] * weights[w_count][a];
            w_count++;
            x++;
            if(w_count >= weights.size()) break;
        }
        a++;
        w_count =w;
        temp += b[i];
        if(ActivationType[i]==1){
            temp = tanh<long double>(temp);
        }else{
            temp = sigmoid<long double>(temp);
        }
        result.push_back(temp);
        if(a >=weights[w_count].size()) break;
    }
    // printf("Fully connected result\n ");
    // rep(x,0,result.size())
    //      printf("%7.16Lf ", result[x]);
    return result;
}


vector<long double> fullyConnectedFunction(vector<vector<long double> > &input, int i, int j, int w){
    vector<long double > result;
    int w_count =w;
    int a=0;
    while(1){
        long double temp=0;
        int x=0;
        while(1){
            // puts("here\n");
            rep(z,0, input.size())
                temp +=input[z][x] * weights[w_count][a];
            w_count++;
            x++;
            if(w_count >= weights.size()) break;
        }
        a++;
        w_count =w;
        temp += b[i];
        if(ActivationType[i]==1){
            temp = tanh<long double>(temp);
        }else{
            temp = sigmoid<long double>(temp);
        }
        result.push_back(temp);
        if(a >=weights[w_count].size()) break;
    }
    // printf("Fully connected result\n ");
    // rep(x,0,result.size())
    //      printf("%7.16Lf ", result[x]);
    return result;
}






template <typename T> T sigmoid(T z){
    // return 1/(1+(T)pow(M_E, -z));
    return 1/(1+(T)exp(-z));
}
template <typename T> T tanh(T z){
    // return ((T)pow(M_E, z)-(T)pow(M_E, -z))/((T)pow(M_E, z)+(T)pow(M_E, -z));
    return ((T)exp(z)-(T)exp(-z))/((T)exp(z)+(T)exp(-z));
}

void printFunction(vector<long double> &input){
    rep(n,0,input.size()){
        cout << std::showpoint << std::fixed << setprecision(16) << input[n] << " ";
    }
    printf("\n");
}