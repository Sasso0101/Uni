#include <iostream>

using namespace std;

int main() {
    int grades[10] = { 18, 24, 30, 27, 20 };

    /* horizontal bar chart (easy/medium) */
    for (int i = 0; i < 10; i++) {
      cout << i << " ";
      for (int j = 0; j < grades[i]; j++) {
        cout << "*";
      }
      cout << " " << grades[i];
      cout << endl;
    }

    /* stats: 0->avg, 1 */
    int grades2[4][5] = {
      {18, 24, 20, 24, 25 },
      {20, 21, 18, 10, 20 },
      {17, 24, 20, 30, 30 },
      {30, 30, 30, 30, 30 },
    };

    int stats[10][3] = {};

    int sum_class = 0;
    int max_class = 0;
    int min_class = 30;

    for (int i = 0; i < 10; i++) {
      int sum = 0;
      int min = 30;
      int max = 0;

      for (int j = 0; j < 5; j++) {
        sum += grades2[i][j];
        sum_class += grades2[i][j];

        if (min > grades2[i][j]) {
          min = grades2[i][j];
        }
        if (max < grades2[i][j]) {
          max = grades2[i][j];
        }

        if (min_class > grades2[i][j]) {
          min_class = grades2[i][j];
        }
        if (max_class < grades2[i][j]) {
          max_class = grades2[i][j];
        }
      }
      cout << "Student " << i << ":" << endl;
      cout << "  Grades: ";
      for (int k = 0; k < 5; k++) {
        cout << grades2[i][k] << " ";
      }
      cout << endl;
      cout << "  Average: " << sum / 5.0 << endl;
      cout << "  Min: " << min << endl;
      cout << "  Max: " << max << endl;
    }

    cout << "Class:" << endl;
    cout << "  Average: " << sum_class / (4 * 5.0) << endl;
    cout << "  Min: " << min_class << endl;
    cout << "  Max: " << max_class << endl;


    /* vertical bar chart (difficult) */
    for (int j = 30; j > 0; j--) {
      for (int i = 0; i < 10; i++) {
        if (grades[i] >= j) {
          cout << " * ";
        }
        else {
          cout << "   ";
        }
      }
      cout << endl;
    }
}