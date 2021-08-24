#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <random>
#include <cmath>
#include <set>
#include <utility>
#include <fstream>
#include <climits>

using namespace std;

#define n 87   // n = the number of members

int common_interests = 0, difference_big5 = 0;

void official(vector<int>& clan1, vector<int>& clan2, vector<set<int>>& prefs, vector<set<int>>& interests, vector<vector<int>> big5) {
  vector<int> count(n + 1);
  for(int i = 0; i < clan1.size(); ++i) {
    for(int j = 0; j < clan1.size(); ++j)
      if(prefs[clan1[i]].count(clan1[j])) count[clan1[i]]++;
  }
  for(int i = 0; i < clan2.size(); ++i) {
    for(int j = 0; j < clan2.size(); ++j)
      if(prefs[clan2[i]].count(clan2[j])) count[clan2[i]]++;
  }
  cout << "///////  STATS   ///////" << endl;
  cout << "The luckiest of everyone = " << *max_element(count.begin(), count.end()) << endl;
  sort(count.begin(), count.end());
  cout << "The least lucky of everyone = " << count[10] << endl;
}


void printit(vector<vector<int>> iterable) {
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

  int jack = INT_MIN, njack = INT_MAX;
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
          jack = max(jack, abs(big5[clans[i][j]][l] - big5[clans[i][k]][l]) / 5);
          njack = min(njack, abs(big5[clans[i][j]][l] - big5[clans[i][k]][l]) / 5);
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


  // Further stats ...
  official(clans[0], clans[1], prefs, interests, big5);
}

int main() {
  // Prefs .. each person has up to 6 preferences
  vector<set<int>> prefs(n + 1);
  cout << "Enter the 6 preferences for each person, from 1-87 ids in order, sep. by white spaces\n";
  int tmp;
  for(int i = 1; i <= n; ++i) {
    for(int j = 0; j < 6; ++j) {
      cin >> tmp;
      if(tmp != i) // To avoid self loops.
        prefs[i].insert(tmp);
    }
  }

  // Interests .. 
  vector<set<int>> interests(n + 1);
  cout << "Enter the interests in the same manner, add -1 to the end of each person\n";
  for(int i = 1; i <= n; ++i) {
    tmp = 0;
    while(tmp != -1) {
      cin >> tmp;
      if(tmp != -1)
        interests[i].insert(tmp);
    }
  }

  //Big-5 .. each person has 5 values, mostly between 20 - 90, I think.
  vector<vector<int>> big5(n + 1, vector<int>(5));
  cout << "Now, enter the big5 data, in the same manner as prefs.\n";
  for(int i = 1; i <= n; ++i) {
    for(int j = 0; j < 5; ++j) {
      cin >> big5[i][j];
    }
  }

  // vector<vector<double>> big6(n + 1, vector<double>(6));
  // cout << "Now, enter the big6 data, in the same manner as prefs.\n";
  // for(int i = 1; i <= n; ++i) {
  //   for(int j = 0; j < 6; ++j) {
  //     cin >> big6[i][j];
  //   }
  // }
  
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
      // bb divided by 5 then by 65, because 65 is the max difference possible 107 - 42
      bb /= 325.0;

      // double ss = 0;
      // for(int k = 0; k < 6; ++k) {
      //   ss += abs(big6[i][k] - big6[j][k]);
      // }
      // ss /= 24.0;
      
      int edge = 25 * ii + 25 * pp + 25 * bb;
      adj[i][j] = adj[j][i] = edge;
    }
  }
  printit(adj);
  scoring(prefs, interests, big5);
}
