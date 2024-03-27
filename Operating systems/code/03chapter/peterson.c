while (true) {
    flag[i] = true; // Mark that i wants to execute the CS
    turn = j; // Give process j the possibility to execute its CS
    while (flag[j] && turn == j); // Wait for process j to finish
    /* Critical section */
    flag[i] = false; // Mark that i finished executing its CS
}