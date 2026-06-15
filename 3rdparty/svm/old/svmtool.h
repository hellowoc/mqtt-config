#ifndef SVM_TOOL
#define SVM_TOOL


#define COEF_COUNT 10

enum svm_result {
    tr_succeeded=0,         //train succeeded
    tr_unsupported_type,    //unsupported input svm type
    tr_no_good_data,        //no good data, countGood=0
    tr_no_bad_data,         //no bad data,countBad=0
    tr_null_data_ptr,       //input data pointer is null
    tr_failed_write_file,   //failed to write sample file
    tr_no_config_file,      //no config file(svm-scale,svm-train)
    tr_unknown_error,       //unknown error

};
enum svm_type {
    color_normal=0,         //input r,g,b
    color_advanced,         //input r,g,b
    color_similarity,       //input r,g,b
    shape_normal,           //input area,edge,width,height,centroid
    color_similarity_n1,    //input r,g,b,n
    color_similarity_n2,    //input r,g,b,n
    color_similarity_n3,    //input r,g,b,n
    svmtype_count,
    color_advanced_n1,      //input r,g,b,n,reserved
    color_advanced_n2,      //input r,g,b,n
    color_advanced_n3,      //input r,g,b,n
    color_normal_n1,        //input r,g,b,n
    color_normal_n2,        //input r,g,b,n
    color_normal_n3,        //input r,g,b,n

};

/*This function is used to train the sample and give the model parameters*/
/* dataGood:    buffer of good data
 * countGood:   counter of good data
 * dataBad:     buffer of bad data
 * countBad:    counter of bad data
 * type:        svm type, it must be color_normal-shape_normal
 * dirTool:     path of the tool(svm-scale,svm-train)
 * coefs:       output:training results, coefs[COEF_COUNT-1] is the constant*/
svm_result train(const int* dataGood, int countGood, const int* dataBad, int countBad,
        svm_type type, const char* dirTool, double coefs[COEF_COUNT]);

/*This function is used to determine whether the input sample is bad(return true)or not(return false)*/
/* data:        buffer of data to be classified
 * coefs:       training results
 * type:        svm type, it must be color_normal-shape_normal*/
bool isBad(const int* data,const double coefs[COEF_COUNT],svm_type type);

#endif // SVM_TOOL

