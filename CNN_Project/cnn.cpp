#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include "cnn.h"
#define rep(i,a,b) for(int i=a; i<b;i++)
using namespace std;
// #define PRINT

CNN::CNN(){}
CNN::~CNN(){}


void CNN::getReady_input(string inputfile_txt, vector<vector< long double> > &_input){
    //get the image from _input and put to vector
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
            _input.push_back(temp_vec);
        }
    }
    input_file.close();
}
void CNN::getReady_weight(string weightsfile_txt){
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
void CNN::getReady_structure(string strucutrefile_txt){
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

vector<long double> CNN::getConvResult(vector<vector<long double> > hidden, int i){
    vector<long double> _output;
    rep(x,0,(int)hidden[0].size()){
        long double temp=0;
        rep(y,0,(int)hidden.size()){
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

vector<long double> CNN::convolutionFunction(vector<long double> &_input, int i, int j, int w, bool t){
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
            // printf("%1.6Lf ",_input[col]);
            // printf("%4.6Lf ",weights[w][w_h_count]);
            #endif
            result+= _input[col] * weights[w][w_h_count];
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
    rep(n,0,(int)_output.size()){
        if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%1.16Lf ", _output[n]);
    }
    printf("\n");
    #endif
    return _output;
}

vector<long double> CNN::avgPoolFunciton(vector<long double> &_input, int i, int j){
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
            // printf("%4.6Lf ",_input[col]);
            result+= _input[col];
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
    rep(n,0,(int)_output.size()){
        // if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%1.16Lf ", _output[n]);
    }
    printf("\n");
    #endif
    return _output;
}

vector<long double> CNN::maxPoolFunciton(vector<long double> &_input, int i, int j){
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
            // printf("%4.6Lf ",_input[col]);
            if(result<_input[col]) result = _input[col];
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
    rep(n,0,(int)_output.size()){
        // if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%1.16Lf ", _output[n]);
    }
    printf("\n");
    #endif
    return _output;
}

vector<long double> CNN::fullyConnectedFunction(vector<long double> &_input,int i, int j, int w){
    vector<long double > result;
    int w_count =w;
    int a=0;
    while(1){
        long double temp=0;
        int x=0;
        while(1){
            // puts("here\n");
            temp +=_input[x] * weights[w_count][a];
            w_count++;
            x++;
            if(w_count >= (int)weights.size()) break;
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
        if(a >=(int)weights[w_count].size()) break;
    }
    // printf("Fully connected result\n ");
    // rep(x,0,result.size())
    //      printf("%7.16Lf ", result[x]);
    return result;
}

vector<long double> CNN::fullyConnectedFunction(vector<vector<long double> > &_input, int i, int j, int w){
    vector<long double > result;
    int w_count =w;
    int a=0;
    while(1){
        long double temp=0;
        int x=0;
        while(1){
            // puts("here\n");
            rep(z,0, (int)_input.size())
                temp +=_input[z][x] * weights[w_count][a];
            w_count++;
            x++;
            if(w_count >= (int)weights.size()) break;
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
        if(a >=(int)weights[w_count].size()) break;
    }
    // printf("Fully connected result\n ");
    // rep(x,0,result.size())
    //      printf("%7.16Lf ", result[x]);
    return result;
}

template <typename T> T CNN::sigmoid(T z){
    // return 1/(1+(T)pow(M_E, -z));
    return 1/(1+(T)exp(-z));
}
template <typename T> T CNN::tanh(T z){
    // return ((T)pow(M_E, z)-(T)pow(M_E, -z))/((T)pow(M_E, z)+(T)pow(M_E, -z));
    return ((T)exp(z)-(T)exp(-z))/((T)exp(z)+(T)exp(-z));
}

void CNN::printFunction(vector<long double> &_input){
        rep(n,0,(int)_input.size()){
        cout << std::showpoint << std::fixed << setprecision(16) << _input[n] << " ";
    }
    printf("\n");
}