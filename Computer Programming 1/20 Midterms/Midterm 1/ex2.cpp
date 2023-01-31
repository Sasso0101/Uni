#include <iostream>

using namespace std;

/* Minimum points in array */
int min(const int scores[]) {
    int min = 250;
    for (int i = 0; i < 5; i++) {
        if (scores[i] < min) min = scores[i];
    }
    return min;
}

/* Maximum points in array */
int max(const int scores[]) {
    int max = 0;
    for (int i = 0; i < 5; i++) {
        if (scores[i] > max) max = scores[i];
    }
    return max;
}

/* Average points in array */
double avg(const int scores[]) {
    double avg = 0;
    for (int i = 0; i < 5; i++) {
        avg += scores[i];
    }
    return avg/5;
}

void teamStats(const int scores[5][5], int *minTeam, int *maxTeam, double *avgTeam) {
    *minTeam = 0;
    *maxTeam = 0;
    *avgTeam = 0;
    int gameScores[5] = {};
    for (int game = 0; game < 5; game++) {
        for (int player = 0; player < 5; player++) {
            gameScores[game] += scores[player][game];
        }
    }
    *minTeam = min(gameScores);
    *maxTeam = max(gameScores);
    *avgTeam = avg(gameScores);
}

int main() {
    int scores[5][5] = {
      {6, 10, 5, 9, 12 },
      {20, 18, 15, 12, 18 },
      {2, 6, 7, 5, 8 },
      {10, 12, 9, 14, 14 },
      {15, 22, 18, 12, 17 },
    };

    cout << "--- PLAYER STATISTICS ---" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "  - PLAYER " << i << " -" << endl;
        cout << "Min: " << min(scores[i]) << endl;
        cout << "Max: " << max(scores[i]) << endl;
        cout << "Avg: " << avg(scores[i]) << endl;
    }
    
    cout << "--- TEAM STATISTICS ---" << endl;
    int minTeam, maxTeam;
    double avgTeam;
    teamStats(scores, &minTeam, &maxTeam, &avgTeam);
    cout << "Min team score: " << minTeam << endl;
    cout << "Max team score: " << maxTeam << endl;
    cout << "Avg team score: " << avgTeam << endl;

}