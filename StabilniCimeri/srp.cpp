#include "srp.h"

using namespace std;

SRP::SRP() {}

SRP::~SRP() {}

void SRP::do_proposals(vector<vector<int>>& preferences, vector<vector<int>>& rankings, vector<int>& favorites, vector<int>& proposals_pos, int roommate, int n) {

    for(int i = proposals_pos[roommate] + 1; i < n ; i++) {

        int to_propose = preferences[roommate][i];
        if(favorites[to_propose] == -1 || rankings[to_propose][roommate] < rankings[to_propose][favorites[to_propose]]) {
            int rejected = favorites[to_propose];
            favorites[to_propose] = roommate;
            proposals_pos[roommate] = i;
            if(rejected != -1)
                do_proposals(preferences, rankings, favorites, proposals_pos, rejected, n);
            break;
        }
    }
}

void SRP::dumpToFile(std::string filePath) {
    std::ofstream out(filePath);
    vector<bool> visited(n, false);

    for(int i = 0; i < n; i++) {
        if(!visited[i])
            out << i << "-> " << preferences[i][leftmost[i]] << "\n";
        visited[preferences[i][leftmost[i]]] = true;
    }
}

vector<vector<int>> SRP::stage_one(vector<vector<int>>& preferences, vector<vector<int>>& rankings, vector<int>& favorites, vector<int>& proposals_pos, int n) {

    for(int i = 0; i < n; i++)
        do_proposals(preferences, rankings, favorites, proposals_pos, i, n);

    vector<vector<int>> reduced_preferences(n);
    for(int i = 0; i < n; i++) {
        for(int j = proposals_pos[i]; j <= rankings[i][favorites[i]]; j++) {
            int current_roommate = preferences[i][j];
            if(rankings[current_roommate][i] <= rankings[current_roommate][favorites[current_roommate]])
                reduced_preferences[i].push_back(current_roommate);
        }
    }
    return reduced_preferences;
}

int SRP::phase_two_condition(vector<vector<int>>& preferences, int n) {

    bool not_fully_reduced = false;
    for(int i = 0; i < n; i++) {
        int pref_size = preferences[i].size();
        if(pref_size == 0)
            return -1;
        else if(pref_size > 1)
            not_fully_reduced = true;
    }

    if(not_fully_reduced)
        return 0;
    else
        return 1;
}

void SRP::print_matrix(vector<vector<int>>& m) {
    int n = m.size();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m[i].size(); j++)
            cout << m[i][j] << ' ';
        cout << endl;
    }
}

int SRP::find_all_or_nothing_cycle(vector<vector<int>>& preferences, int n, vector<int>& cycle) {

    int start_person;
    if(last_in_tail == -1) {
        while(1) {
            start_person = rand() % n;
            if(rightmost[start_person] - leftmost[start_person] + 1 >= 2) {
                cycle.push_back(start_person);
                break;
            }
        }
    }
    else
        start_person = last_in_tail;

    vector<bool> repeated(n, false);
    repeated[start_person] = true;

    int current_person = start_person;
    while(1) {
        int q_i = preferences[current_person][leftmost[current_person] + 1];
        current_person = preferences[q_i][rightmost[q_i]];

        if(repeated[current_person]) {
            for(int i = 0; i < cycle.size(); i++)
                if(cycle[i] == current_person) {
                    return i;
                }
        }

        cycle.push_back(current_person);
        repeated[current_person] = true;
    }
}

int SRP::phase_two_reduction(vector<vector<int>>& preferences, vector<vector<int>>& rankings, int n) {
    vector<int> cycle;
    int tail_size = find_all_or_nothing_cycle(preferences, n, cycle);

    if(tail_size == 0)
        last_in_tail = -1;
    else
        last_in_tail = cycle[tail_size - 1];

    for(int i = tail_size; i < cycle.size(); i++) {
        int a_i = cycle[i];
        int b_i1 = preferences[a_i][1];

        for(int j = rightmost[b_i1]; j >= 0; j--) {
            int removed = preferences[b_i1][j];
            if(removed == a_i) {
                rightmost[b_i1] = j;
                if(leftmost[b_i1] > rightmost[b_i1])
                    return -1;
                if(leftmost[b_i1] == rightmost[b_i1])
                    numOfOnes++;
                break;
            }
        }

        leftmost[a_i] = 1;
        if(leftmost[a_i] > rightmost[a_i])
            return -1;
        if(leftmost[a_i] == rightmost[a_i])
            numOfOnes++;
    }

    return 0;
}

bool SRP::stable_roommate_algorithm() {

    vector<int> favorites(n, -1);
    vector<int> proposals_pos(n, -1);

    auto reduced_preferences = stage_one(preferences, rankings, favorites, proposals_pos, n);

    for(int i = 0; i < n; i++) {
        int size = reduced_preferences[i].size();
        rightmost[i] = size - 1;
        if(size == 1)
            numOfOnes++;
    }

    int condition = phase_two_condition(reduced_preferences, n);
    while(condition == 0) {
        condition = phase_two_reduction(reduced_preferences, rankings, n);
        if(numOfOnes == n)
            condition = 1;
    }

    if(condition == -1) {
        cout << "Such matching doesnt exist!\n";
        return false;
    }

    return true;
}
