#include <iostream> 
using namespace std; 


int BinarySearch(int arr[], int size, int target, bool first) {

    int left = 0;
    int right = size - 1; 
    int result = -1; 

    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            result = mid;
            
            if (first) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } 
        else if (arr[mid] < target) {
            left = mid + 1; 
        } 
        else {
            right = mid - 1;
        }
    }
    return result;
}



int main() {

    
    int arr[] = { 5, 4, 3, 8, 8, 9, 10, 99, 88};
    int size = sizeof(arr) / sizeof(arr[0]); 
    int target = 8;
    int first = BinarySearch(arr, size, target, true);
    int last = BinarySearch(arr, size, target, false);
    cout << "[" << first << ", " << last << "]" << endl;
    
    return 0; 
}