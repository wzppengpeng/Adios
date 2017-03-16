# Aiolos is a machine learning framework and engine!
## Classifier:
    . KNN
    . ClassifyDecisionTree
    . NaiveBayes
    . Logistic Regression
    . SVM(Linear SVM, Kernal RBF)
    . AdaBoost(Use Stump)
    . RandomForest
    . ANN

## Regression
    . Stand Linear Regression, LWLR Regression
    . Cart Tree Regression
    . Model Tree Regression

## Cluster
    . Kmeans
        Base Kmeans Solution
        binary Kmeans Solution

## How To Use
### The Config File Example Is Like This Below
    #the objective of the job
    objective = classify
    #train predict and both
    mode = both
    #the task model or algorithm to solve the problem
    task = knn
    #the input data path
    input_path = ../cancer_train.csv
    #the predict data path
    predict_path = ../cancer_test.csv
    #the output data path
    output = ret.txt
    #knn's k
    k = 3

## How To Install
### Do These Operations
    . Change The Include Directories Of wzp_cpp_lib
    . make -p build
    . cd build
    . cmake ..
    . make -j4
