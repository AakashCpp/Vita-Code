#include <bits/stdc++.h>
using namespace std;

int lcs_len(const string &a, const string &b) {
    int n = a.size(), m = b.size();
    vector<int> dp(m+1,0);
    for (int i = 1; i <= n; ++i) {
        int prev = 0;
        for (int j = 1; j <= m; ++j) {
            int cur = dp[j];
            if (a[i-1] == b[j-1]) dp[j] = prev + 1;
            else dp[j] = max(dp[j], dp[j-1]);
            prev = cur;
        }
    }
    return dp[m];
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if(!(cin>>N)) return 0;
    vector<char> s;
    s.reserve(N);
    for(int i=0;i<N;++i){
        string tok;
        if(!(cin>>tok)) return 0;
        s.push_back(tok[0]);
    }
    vector<int> fixedPos;
    int x;
    while(cin >> x){
        fixedPos.push_back(x-1);
    }
    int cntA=0,cntB=0,cntC=0;
    string S;
    S.reserve(N);
    for(char c: s){
        S.push_back(c);
        if(c=='A') ++cntA;
        else if(c=='B') ++cntB;
        else if(c=='C') ++cntC;
    }
    vector<array<char,3>> orders = {
        array<char,3>{'A','B','C'},
        array<char,3>{'A','C','B'},
        array<char,3>{'B','A','C'},
        array<char,3>{'B','C','A'},
        array<char,3>{'C','A','B'},
        array<char,3>{'C','B','A'}
    };
    bool anyValid=false;
    int best = INT_MAX;
    for(auto ord : orders){
        int len1 = (ord[0]=='A'?cntA: ord[0]=='B'?cntB:cntC);
        int len2 = (ord[1]=='A'?cntA: ord[1]=='B'?cntB:cntC);
        int len3 = (ord[2]=='A'?cntA: ord[2]=='B'?cntB:cntC);
        if(len1+len2+len3 != N) continue;
        bool ok=true;
        for(int p: fixedPos){
            if(p<0 || p>=N){ ok=false; break; }
            char expected = (p < len1) ? ord[0] : (p < len1+len2) ? ord[1] : ord[2];
            if(S[p] != expected){ ok=false; break; }
        }
        if(!ok) continue;
        anyValid = true;
        string T;
        T.append(len1, ord[0]);
        T.append(len2, ord[1]);
        T.append(len3, ord[2]);
        int l = lcs_len(S, T);
        best = min(best, N - l);
    }
    if(!anyValid) cout << "Impossible\n";
    else cout << best << '\n';
    return 0;
}
