#ifndef cnn_
#define cnn_

#include <vector>
#include <string>
class CNN{
    public:

    //variables
    std::vector<std::vector<long double> > weights;
    std::vector<int> LayerID, TotalFilters, FilterSize, Stride, MatrixSize, Channels, ActivationType;
    std::vector<std::string> LayerType;
    std::vector<long double> b;
    long double alpha;

    //functions
    void getReady_input(std::string inputfile_txt,std::vector<std::vector<long double> > &input);
    void getReady_weight(std::string weightsfile_txt);
    void getReady_structure(std::string strucutrefile_txt);
    std::vector<long double>  convolutionFunction(std::vector<long double> &input, int i, int j, int w, bool t);
    std::vector<long double>  avgPoolFunciton(std::vector<long double> &input, int i, int j);
    std::vector<long double>  maxPoolFunciton(std::vector<long double> &input, int i, int j);
    std::vector<long double> fullyConnectedFunction(std::vector<long double> &input,int, int, int);
    std::vector<long double> fullyConnectedFunction(std::vector<std::vector<long double> > &input,int i, int j, int w);
    void printFunction(std::vector<long double> &output);
    std::vector<long double> getConvResult(std::vector<std::vector<long double> >, int);
    template <typename T> T tanh(T z);
    template <typename T> T sigmoid(T z);
    //---------
    CNN();
    ~CNN();
};


#endif