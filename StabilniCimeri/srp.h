#ifndef SRP_H
#define SRP_H

#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class SRP
{
public:
    SRP(vector<vector<int>>& preferences, vector<vector<int>>& rankings, int n)
        : preferences(preferences), rankings(rankings), n(n) {
        leftmost = vector<int>(n, 0);
        rightmost = vector<int>(n, n-1);
        numOfOnes = 0;
        last_in_tail = -1;
    };
    SRP();
    ~SRP();
    bool stable_roommate_algorithm();
    void dumpToFile(std::string filePath);
    void print_matrix(vector<vector<int>>& m);


private:
    vector<vector<int>> preferences;
    vector<vector<int>> rankings;
    int n;
    vector<int> leftmost;
    vector<int> rightmost;
    int numOfOnes;
    int last_in_tail;
    int phase_two_reduction(vector<vector<int>>& preferences, vector<vector<int>>& rankings, int n);
    int find_all_or_nothing_cycle(vector<vector<int>>& preferences, int n, vector<int>& cycle);
    int phase_two_condition(vector<vector<int>>& preferences, int n);
    vector<vector<int>> stage_one(vector<vector<int>>& preferences, vector<vector<int>>& rankings, vector<int>& favorites, vector<int>& proposals_pos, int n);
    void do_proposals(vector<vector<int>>& preferences, vector<vector<int>>& rankings, vector<int>& favorites, vector<int>& proposals_pos, int roommate, int n);


};

#endif // SRP_H
