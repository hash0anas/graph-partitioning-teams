#include <algorithm>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <set>
#include <utility>
#include <fstream>

using namespace std;

#define n 100   // n = the number of members

int common_interests = 0, difference_big5 = 0;

void printit(auto iterable) {
  ofstream out("connections.txt");
  out << "{";
  for(int i = 1; i <= n; ++i) {
    out << "{";
    for(int j = 1; j <= n; ++j) {
      out << iterable[i][j];
      out << (j < n ? ", " : "");
    }
    out << (i < n ? "}, " : "}");
  }
  out << "}" << endl;
  out.close();
}

void scoring(vector<set<int>>& prefs, vector<set<int>>& interests, vector<vector<int>> big5) {
  // I/O routine.
  vector<vector<int>> clans(2);
  cout << "Enter the ids of clan #1 separated by spaces, 0, then do the same for clan #2" << endl;
  for(int i = 0; i < 2; ++i) {
    bool donewith = false;
    int x;
    for(int j = 0; !donewith; ++j) {
      cin >> x;
      if(x == 0) donewith = true;
      else clans[i].push_back(x);
    }
  }

  // Computation ...
  vector<int> total_satisfied_prefs(2);
  vector<int> total_common_interests(2);
  vector<int> total_difference_big5(2);
  for(int i = 0; i < 2; ++i) {
    for(int j = 0; j < clans[i].size(); ++j) {
      for(int pref : prefs[clans[i][j]]) {
        if(find(clans[i].begin(), clans[i].end(), pref) != clans[i].end()) total_satisfied_prefs[i]++;
      }
      
      for(int k = j + 1; k < clans[i].size(); ++k) {
        vector<int> v(20);
        auto it = set_intersection(interests[clans[i][j]].begin(), interests[clans[i][j]].end(), interests[clans[i][k]].begin(), interests[clans[i][k]].end(), v.begin());
        v.resize(it-v.begin());
        total_common_interests[i] += v.size();
        
        for(int l = 0; l < 5; ++l) {
          total_difference_big5[i] += (abs(big5[clans[i][j]][l] - big5[clans[i][k]][l]) / 5);
        }
      }
    }
  }

  // Output
  float n1 = pow(clans[0].size(), 2) - clans[0].size();
  float n2 = pow(clans[1].size(), 2) - clans[1].size();

  printf("The number of people in clans[1, 2] = [%d, %d]\n\n", clans[0].size(), clans[1].size());
  printf("Total satsfied preferences in clans[1, 2] = [%d, %d]\n", total_satisfied_prefs[0], total_satisfied_prefs[1]);
  printf("Average of satsfied preferences in clans[1, 2] = [%.2f, %.2f]\n\n", total_satisfied_prefs[0] / (float) clans[0].size(), total_satisfied_prefs[1] / (float) clans[1].size());

  printf("The number of common interests in the club = %d\n", common_interests);
  printf("Total common interests in clans[1, 2] = [%d, %d]\n", total_common_interests[0], total_common_interests[1]);
  printf("Average number of common interests in clans[1, 2] = [%.2f, %.2f]\n\n", total_common_interests[0]/n1, total_common_interests[1]/n2);

  // printf("The total difference of the big-5 in the club = %d\n", difference_big5);
  printf("Total difference in the big-5 in clans[1, 2] = [%d, %d]\n", total_difference_big5[0], total_difference_big5[1]);
  printf("Average difference in the big-5 in clans[1, 2] = [%.2f, %.2f]\n", 5*total_difference_big5[0]/n1, 5*total_difference_big5[1]/n2);
}

int main() {
  random_device rd; //true random generator

  // Prefs .. each person has up to 10 preferences
  vector<set<int>> prefs(n + 1);
  uniform_int_distribution<int> dist(1, 100);
  for(int i = 1; i <= n; ++i) {
    while(prefs[i].size() <= 10) {
      int r = dist(rd);
      if(r != i) // To avoid self loops.
        prefs[i].insert(r);
    }
  }

  // Interests .. easch person might choose, on avg, betwen 5 - 15 interests
  vector<set<int>> interests(n + 1);
  uniform_int_distribution<int> dist2(5, 15);
  uniform_int_distribution<int> dist3(1, 20);
  for(int i = 1; i <= n; ++i) {
    int rs = dist2(rd); //no of chosen interests may vary
    while(interests[i].size() <= rs) {
      interests[i].insert(dist3(rd));
    }
  }

  //Big-5 .. each person has 5 values, mostly between 20 - 90, I think.
  vector<vector<int>> big5(n + 1, vector<int>(5));
  uniform_int_distribution<int> dist4(20, 90);
  for(int i = 1; i <= n; ++i) {
    for(int j = 0; j < 5; ++j) {
      big5[i][j] = dist4(rd);
    }
  }
  
  //Now, work on the adj. matrix according to the criteria agreed upon earlier.
  vector<vector<int>> adj(n + 1, vector<int>(n + 1));
  for(int i = 1; i <= n; ++i) {
    for(int j = i + 1; j <= n; ++j) {
      // edge = 50 * ii + 30 * pp + 20 * bb;
      // where ii, pp and bb must all be between 0 - 1
      double pp = (prefs[i].count(j) + prefs[j].count(i)) / 2.0;

      vector<int> v(20); // 20 interests max
      auto it = set_intersection(interests[i].begin(), interests[i].end(), interests[j].begin(), interests[j].end(), v.begin());
      v.resize(it-v.begin());
      common_interests += v.size();
      double ii = (double) v.size() / min(interests[i].size(), interests[j].size());

      double bb = 0;
      for(int k = 0; k < 5; ++k) {
        bb += abs(big5[i][k] - big5[j][k]);
      }
      difference_big5 += (int) bb;
      // bb divided by 5 then by 70, because 70 is the max difference possible 90 - 20
      bb /= 350.0;
      
      int edge = 50 * ii + 30 * pp + 20 * bb;
      adj[i][j] = adj[j][i] = edge;
    }
  }
  printit(adj);
  scoring(prefs, interests, big5);
}
