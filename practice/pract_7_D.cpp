#include <iostream>
using namespace std;




int main() {
    int N, M;
    cin >> N >> M;
    
    int raftsCount = N * M;
    long long rafts[raftsCount];
    
    int idx = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            cin >> rafts[idx++];
        }
    }
    
    int K;
    cin >> K;
    
    long long mesklinits[K];
    for(int i = 0; i < K; i++) {
        cin >> mesklinits[i];
    }
    

    
    for(int i = 0; i < raftsCount - 1; i++) {
        for(int j = 0; j < raftsCount - i - 1; j++) {
            if(rafts[j] > rafts[j + 1]) {
                swap(rafts[j], rafts[j + 1]);
            }
        }
    }
    
    for(int i = 0; i < K - 1; i++) {
        for(int j = 0; j < K - i - 1; j++) {
            if(mesklinits[j] > mesklinits[j + 1]) {
                swap(mesklinits[j], mesklinits[j + 1]);
            }
        }
    }
    
    int count = 0;
    int raftIndex = 0;
    int meskIndex = 0;
    
    while(raftIndex < raftsCount && meskIndex < K) {
        if(mesklinits[meskIndex] <= rafts[raftIndex]) {
            count++;
            raftIndex++;
            meskIndex++;
        } else {
            raftIndex++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}