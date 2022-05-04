#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#define rep(i,a,b) for(int i=a; i<b;i++)



using namespace std;

long double alpha;
vector<vector<long double> > input;
vector<vector<long double> > weights;
vector<int> LayerID, TotalFilters, FilterSize, Stride, MatrixSize, Channels, ActivationType;
vector<string> LayerType;
vector<long double> b;
    vector<vector<long double> > hidden;
    vector<long double> output;

void getReady_input(string inputfile_txt);
void getReady_weight(string weightsfile_txt);
void getReady_structure(string strucutrefile_txt);
void convolutionFunction(vector<long double> &input, int i, int j, int w);
void avgPoolFunciton(vector<long double> &input, int i, int j);
void maxPoolFunciton(vector<long double> &input, int i, int j);
template <typename T> T tanh(T z);
template <typename T> T sigmoid(T z);

int main(int argc, char* argv[]){
    string inputfile_txt, weightsfile_txt, strucutrefile_txt;
    

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

    getReady_input(inputfile_txt);
    getReady_weight(weightsfile_txt);
    getReady_structure(strucutrefile_txt);
    //done reading info from files-------------------------------------------------------------------
    //show the input file
    rep(i,0,input.size()){
        rep(j,0,input[i].size()){
            if(j%(MatrixSize[i]) ==0 && j!=0) printf("\n");
            printf("%5.1Lf ",input[i][j]);
            
        }
        printf("\n");
    }

    //check the max channels so that I can know how many hidden channels are needed
    int max_channels=0;
    rep(i, 0, LayerID.size()){
        max_channels = max(max_channels, Channels[i]);
    }
    //make neccessary number of channels
    rep(i,0,max_channels){
        vector<long double> temp;
        hidden.push_back(temp);
    } 
    cout <<"Filtering" <<endl;
    int weight_count=0;
    rep(i,1,LayerID.size()){
        if(LayerType[i] == "C"){
            rep(j,0,Channels[i]){
                if(i==1)
                    convolutionFunction(input[0], i, j, weight_count);
                else
                    convolutionFunction(hidden[j], i, j, weight_count);
                weight_count++;
            }
        }else if (LayerType[i] == "A"){
            rep(j,0,Channels[i]){
                if(i==1)
                    avgPoolFunciton(input[0],i,j);
                else{
                    avgPoolFunciton(hidden[j],i,j);
                }
                    // avgPoolFunciton(hidden[j],i,j);
            }
        }else if (LayerType[i] == "M"){
            rep(j,0,Channels[i]){
                if(i==1)
                    avgPoolFunciton(input[0],i,j);
                else
                    maxPoolFunciton(hidden[j],i,j);
            }
            printf("%d\n", FilterSize[i]);
            
        }else if (LayerType[i] == "F"){
            
        }
        
    }
    

    printf("%d\n", max_channels);
    //show the weight file
    printf("M_E is %f\n", M_E);
    // rep(n,0,input[0].size()){
    //     if(n!=0 && n%MatrixSize[1]==0) printf("\n");
    //         printf("%5.6Lf ", input[0][n]);
    // }
    // printf("\n");

    //show the structure file
    rep(i,0,LayerID.size()){
        cout << LayerID[i] <<" ";
        cout << LayerType[i] <<" ";
        cout << TotalFilters[i] <<" ";
        cout << FilterSize[i] <<" ";
        cout << Stride[i] <<" ";
        cout << MatrixSize[i] <<" ";
        cout << Channels[i] <<" ";
        cout << ActivationType[i] <<" ";
        cout << b[i] <<endl;
    }
    

    
    return 0;
}


void getReady_input(string inputfile_txt){
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
void convolutionFunction(vector<long double> &input, int i, int j, int w){

    int col=0;
    int i_h_count=0;
    int i_v_count=0;
    int count=0;
    int w_h_count=0;
    vector<long double> _output;
    while(1){
        // printf("\nFilter %d\n", count);
        // printf("col %d\n", col);
        long double result=0;
        int loop_counter=0;
        for(;;){
            // if((col-i_h_count*Stride[i])%(FilterSize[i]) ==0 && (col-i_h_count*Stride[i])!=0 && (col-i_h_count*Stride[i])%MatrixSize[i-1]!=0) {
            //     row++;
            //     col += FilterSize[i];
            //     // printf("\n");
            //     if(row==FilterSize[i]) break;
            // }
            // // printf("%4.1Lf ",input[col]);
            // // printf("%Lf ",weights[w][w_h_count]);
            // result+= input[col] * weights[w][w_h_count];
                
            // col++;
            // w_h_count++;
            


            // printf("%8.1Lf ",input[col]);
            // printf("%4.6Lf ",weights[w][w_h_count]);
            result+= input[col] * weights[w][w_h_count];
            col++;
            w_h_count++;
            if(loop_counter!=0 && (loop_counter+1)%FilterSize[i]==0){
                col += FilterSize[i];
                // printf("\n");
            }
            loop_counter++;
            if(loop_counter==FilterSize[i]*FilterSize[i]) break;
        }
        // printf("\n");
        if(ActivationType[i]==1){
            result = tanh<long double>(result);
        }else{
            result = sigmoid<long double>(result);
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

    hidden[j] =_output;
    printf("\n%d th channel filtered output\n", j);
    rep(n,0,hidden[j].size()){
        if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%10.6Lf ", hidden[j][n]);
    }
    printf("\n");
    
}




void avgPoolFunciton(vector<long double> &input, int i, int j){
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
                col += FilterSize[i]-1;
                printf("\n");
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
        if(count==MatrixSize[1]*MatrixSize[1]) break;
    }

    printf("\n%d th average channel filtered output\n", j);
    rep(n,0,_output.size()){
        if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%10.6Lf ", _output[n]);
    }
    printf("\n");
    hidden[j].clear();
    hidden[j] = _output;

}


void maxPoolFunciton(vector<long double> &input, int i, int j){
    int col=0;
    int row=0;
    int i_h_count=0;
    int i_v_count=0;
    int count=0;
    vector<long double> _output;
    while(1){
        // printf("\nMax Filter %d\n", count);
        long double result=0;
        int loop_counter=0;
        for(;;){
            // printf("%4.6Lf ",input[col]);
            if(result<input[col]) result = input[col];
            col++;
            if(loop_counter!=0 && (loop_counter+1)%FilterSize[i]==0){
                col += FilterSize[i]-1;
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

    printf("\n%d th max channel filtered output\n", j);
    rep(n,0,_output.size()){
        if(n!=0 && n%MatrixSize[i]==0) printf("\n");
            printf("%10.6Lf ", _output[n]);
    }
    printf("\n");
    hidden[j].clear();
    hidden[j] = _output;
}




template <typename T> T tanh(T z){
    return 1/(1+pow(M_E, -z));
}
template <typename T> T sigmoid(T z){
    return (pow(M_E, z)-pow(M_E, -z))/(pow(M_E, z)+pow(M_E, -z));
}