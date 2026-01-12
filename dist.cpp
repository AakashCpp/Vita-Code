#include<iostream>
using namespace std;

int main(){

    int size = 4;
    int arr[size] = {3,6,9,1};

    for(int i=0;i<size;i++){
        for(int j=i+1;j<size;j++){
            int temp = 0;
            if(arr[i]<arr[j]){
                if(arr[i] > arr[j]){
                    swap(arr[i], arr[j]);
                }
            }
        }
    }
    int max = 0;

    for(int i = size ; i >=0 ; i--){
        if(arr[i] - arr[i-1] > max){
            max = arr[i] - arr[i-1];
        }
    }

    cout<<max;
    return 0;
}