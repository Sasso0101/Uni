#include <iostream>

using namespace std;

int main() {
    int v[] = {1,2,2,2,1};
    int left = 0;
    int right = (sizeof(v)/sizeof(int))-1;
    int isPalindrome = true;
    while (isPalindrome && left - right <= 1) {
        if(v[left] != v[right]) isPalindrome = false;
        left++;
        right--;
    }
    if (isPalindrome) cout << "Array is palindrome\n";
    else cout << "Array is not palindrome\n";
}