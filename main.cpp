#include<bits/stdc++.h>
using namespace std;
#define int long long

#define int long long
typedef pair<double, double> POINT;
typedef pair<POINT, POINT> EDGE;
#define epb edgeList.push_back
#define vpb vertices.push_back

const int MAX_SIZE = 10001;
vector<vector<int>> grid(MAX_SIZE, vector<int>(MAX_SIZE, 0));

void getGrid(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }
    for(int i = 0; i < MAX_SIZE; ++i){
        for(int j = 0; j < MAX_SIZE; ++j){
            grid[i][j] = 0;
        }
    }
    int N;
    file >> N;
    for (int _ = 0; _ < N; ++_) {
        int x, y, value;
        file >> x >> y >> value;
        grid[x][y] += value;
    }
    int M;
    file >> M;
    for (int _ = 0; _ < M; ++_) {
        int x, y, value;
        file >> x >> y >> value;
        grid[x][y] -= value;
    }
}

void getGridTranspose(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }
    for(int i = 0; i < MAX_SIZE; ++i){
        for(int j = 0; j < MAX_SIZE; ++j){
            grid[i][j] = 0;
        }
    }
    int N;
    file >> N;
    for (int _ = 0; _ < N; ++_) {
        int x, y, value;
        file >> x >> y >> value;
        grid[y][x] += value;
    }
    int M;
    file >> M;
    for (int _ = 0; _ < M; ++_) {
        int x, y, value;
        file >> x >> y >> value;
        grid[y][x] -= value;
    }
}

int allPositivesSum() {
    int ans = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if (grid[i][j] > 0) {
                ans += grid[i][j];
            }
        }
    }
    return ans;
}

void kadane(const vector<int>& arr, int& start, int& end, int& max_sum) {
    int current_max = 0;
    max_sum = 0;
    start = 0;
    end = -1;
    int temp_start = 0;

    for (int i = 0; i < arr.size(); ++i) {
        current_max += arr[i];
        if (current_max > max_sum) {
            max_sum = current_max;
            start = temp_start;
            end = i;
        }
        if (current_max < 0) {
            current_max = 0;
            temp_start = i + 1;
        }
    }

    if (end == -1) { // All elements were negative
        max_sum = -1e8;
        start = 0;
        end = 0;
    }
}

vector<int> starts1, ends1, starts2, ends2, maxes1, maxes2;
vector<vector<int>> dp0;
vector<vector<int>> dp1;
vector<vector<int>> dp2;

pair<int,int> getOptimalValue(int rows) {
    int divisions = MAX_SIZE / rows;
    dp1.assign(divisions, vector<int>(1001, 0));
    dp2.assign(divisions, vector<int>(1001, 0));
    dp0.assign(divisions, vector<int>(1001, 0));

    vector<vector<int>> subgrid(rows + 1, vector<int>(MAX_SIZE, 0));
    for (int r = 0; r <= rows; ++r) {
        for (int c = 0; c < MAX_SIZE; ++c) {
            subgrid[r][c] = grid[r][c];
        }
    }

    for (int r = 1; r <= rows; ++r) {
        for (int c = 0; c < MAX_SIZE; ++c) {
            subgrid[r][c] += subgrid[r-1][c];
        }
    }

    int start, end, max_sum;
    kadane(subgrid.back(), start, end, max_sum);
    starts1.push_back(start);
    ends1.push_back(end);
    maxes1.push_back(max_sum);

    for (int c = start; c <= end; ++c) {
        subgrid.back()[c] = INT_MIN;
    }

    kadane(subgrid.back(), start, end, max_sum);
    starts2.push_back(start);
    ends2.push_back(end);
    maxes2.push_back(max_sum);
    
    for (int i = rows + 1; i < MAX_SIZE; i += rows) {
        vector<vector<int>> subgrid(rows, vector<int>(MAX_SIZE, 0));
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < MAX_SIZE; ++c) {
                subgrid[r][c] = grid[i + r][c];
            }
        }

        for (int r = 1; r < rows; ++r) {
            for (int c = 0; c < MAX_SIZE; ++c) {
                subgrid[r][c] += subgrid[r-1][c];
            }
        }

        int start, end, max_sum;
        kadane(subgrid.back(), start, end, max_sum);
        starts1.push_back(start);
        ends1.push_back(end);
        maxes1.push_back(max_sum);

        for (int c = start; c <= end; ++c) {
            subgrid.back()[c] = INT_MIN;
        }

        kadane(subgrid.back(), start, end, max_sum);
        starts2.push_back(start);
        ends2.push_back(end);
        maxes2.push_back(max_sum);
    }

    for(int i = 0; i < divisions; i++){
        dp1[i][0] = dp1[i][1] = dp1[i][2] = dp1[i][3] = -1e8;
        dp2[i][0] = dp2[i][1] = dp2[i][2] = dp2[i][3] = dp2[i][4] = dp2[i][5] = dp2[i][6] = dp2[i][7] = -1e8;
    }
    dp1[0][4] = maxes1[0];
    dp2[0][8] = maxes1[0] + maxes2[0];

    for (int i = 1; i < divisions; ++i) {
        for (int j = 1; j <= 1000; ++j) {

            // dp0
            dp0[i][j] = max({dp0[i][j], dp0[i-1][j], dp1[i-1][j], dp2[i-1][j]});


            // dp1
            // From dp0[i - 1]
            if (j >= 6) {
                dp1[i][j] = max(dp1[i][j], dp0[i-1][j-6] + maxes1[i]);
            }

            // From dp1[i - 1]
            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])){
                if(j >= 2) dp1[i][j] = max(dp1[i][j], dp1[i - 1][j - 2] + maxes1[i]);
            }
            else if((starts1[i - 1] == starts1[i]) && (ends1[i - 1] == ends1[i])){
                dp1[i][j] = max(dp1[i][j], dp1[i - 1][j] + maxes1[i]);
            }
            else{
                if(j >= 4) dp1[i][j] = max(dp1[i][j], dp1[i - 1][j - 4] + maxes1[i]);
            }

            // From dp2[i - 1]
            if((starts1[i - 1] == starts1[i]) ^ (ends1[i - 1] == ends1[i])){
                if(j >= 2) dp1[i][j] = max(dp1[i][j], dp2[i - 1][j - 2] + maxes1[i]);
            }
            else if((starts1[i - 1] == starts1[i]) && (ends1[i - 1] == ends1[i])){
                dp1[i][j] = max(dp1[i][j], dp2[i - 1][j] + maxes1[i]);
            }
            else{
                if(j >= 4) dp1[i][j] = max(dp1[i][j], dp2[i - 1][j - 4] + maxes1[i]);
            }



            // dp2
            // From dp0[i - 1]
            if (j >= 10) {
                dp2[i][j] = max(dp2[i][j], dp0[i-1][j-10] + maxes1[i] + maxes2[i]);
            }

            // From dp1[i - 1]
            if(starts1[i - 1] == starts1[i] && ends1[i - 1] == ends1[i]){
                if(j >= 6) dp2[i][j] = max(dp2[i][j], dp1[i - 1][j - 6] + maxes1[i] + maxes2[i]);
            }
            else{
                if(j >= 8) dp2[i][j] = max(dp2[i][j], dp1[i - 1][j - 8] + maxes1[i] + maxes2[i]);
            }

            // From dp2[i - 1]
            if(starts1[i - 1] == starts1[i] && ends1[i - 1] == ends1[i]){
                if(j >= 6) dp2[i][j] = max(dp2[i][j], dp2[i - 1][j - 6] + maxes1[i] + maxes2[i]);
            }
            else{
                if(j >= 8) dp2[i][j] = max(dp2[i][j], dp2[i - 1][j - 8] + maxes1[i] + maxes2[i]);
            }
        }
    }

    int optimum = 0;
    int used = -1;
    for (int i=1; i<=1000; i++){
        int temp = max({dp0[divisions - 1][i], dp1[divisions - 1][i], dp2[divisions - 1][i]});
        if (temp >= optimum){
            optimum = temp;
            used = i;
        }
    }

    return {optimum,used};
}

vector<int> getSubarrays(int rows, int used) {
    int divisions = 10000 / rows;
    int i = -1, j = divisions - 1, k = used;
    vector<int> selected(divisions);
    int mx = max({dp0[j][k], dp1[j][k], dp2[j][k]});
    if(mx == dp0[j][k]) i = 0;
    else if(mx == dp1[j][k]) i = 1;
    else i = 2;
    
    while (j >= 0) {
        int which = i;
        
        selected[j] = i;
        if(j == 0){
            break;
        }

        if (which == 0) {
            int m = dp0[j][k];
            if (m == dp0[j - 1][k]) {
                i = 0;
            } else if (m == dp1[j - 1][k]) {
                i = 1;
            } else {
                i = 2;
            }
            j -= 1;
            continue;
        }
        
        else if (which == 1){
            int m = dp1[j][k];

            // From dp0[j - 1] ???
            if (k >= 6 && m == dp0[j - 1][k - 6] + maxes1[j]){
                i = 0; j -= 1; k -= 6;
                continue;
            }
            // From dp1[j - 1] ???
            if ((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])) {
                if (k >= 2 && m == dp1[j - 1][k - 2] + maxes1[j]){
                    i = 1; j -= 1; k -= 2;
                    continue;
                }
            }
            else if (starts1[j - 1] == starts1[j] && ends1[j - 1] == ends1[j]) {
                if(m == dp1[j - 1][k] + maxes1[j]){
                    i = 1; j -= 1;
                    continue;
                }
            }
            else {
                if (k >= 4 && m == dp1[j - 1][k - 4] + maxes1[j]){
                    i = 1; j -= 1; k -= 4;
                    continue;
                }                
            }
            // From dp2[j - 1] ???
            if ((starts1[j - 1] == starts1[j]) ^ (ends1[j - 1] == ends1[j])) {
                if (k >= 2 && m == dp2[j - 1][k - 2] + maxes1[j]){
                    i = 2; j -= 1; k -= 2;
                    continue;
                }
            }
            else if (starts1[j - 1] == starts1[j] && ends1[j - 1] == ends1[j]) {
                if(m == dp2[j - 1][k] + maxes1[j]){
                    i = 2; j -= 1;
                    continue;
                }
            }
            else {
                if (k >= 4 && m == dp2[j - 1][k - 4] + maxes1[j]){
                    i = 2; j -= 1; k -= 4;
                    continue;
                }                
            }
        }
        else {
            int m = dp2[j][k];

            // From dp0[j - 1] ???
            if (k >= 10 && m == dp0[j - 1][k - 10] + maxes1[j] + maxes2[j]) {
                i = 0; j -= 1; k -= 10;
                continue;
            }
            // From dp1[j - 1] ???
            if (starts1[j - 1] == starts1[j] && ends1[j - 1] == ends1[j]) {
                if (k >= 6 && m == dp1[j - 1][k - 6] + maxes1[j] + maxes2[j]){
                    i = 1; j -= 1; k -= 6;
                    continue;
                }
            }
            else {
                if (k >= 8 && m == dp1[j - 1][k - 8] + maxes1[j] + maxes2[j]){
                    i = 1; j -= 1; k -= 8;
                    continue;
                }
            }
            // From dp2[j - 1] ???
            if (starts1[j - 1] == starts1[j] && ends1[j - 1] == ends1[j]) {
                if (k >= 6 && m == dp2[j - 1][k - 6] + maxes1[j] + maxes2[j]){
                    i = 2; j -= 1; k -= 6;
                    continue;
                }
            }
            else {
                if (k >= 8 && m == dp2[j - 1][k - 8] + maxes1[j] + maxes2[j]){
                    i = 2; j -= 1; k -= 8;
                    continue;
                }
            }
        }
    }
    return selected;

}


vector<EDGE> getEdges(vector<int> selected, int len){
    
    vector<EDGE> edgeList;

    int n = starts1.size();

    int initial = 0;

    while(selected[initial] == 0){
        initial++;
    }

    int x = initial*len;
    POINT preStart, preEnd;

    if(selected[initial]==1){
        POINT A,B;
        A = {x-0.6,starts1[initial]-0.6};
        B = {x-0.6,ends1[initial]+0.6};
        
        epb({A,B});
        preStart = A;
        preEnd = B;
    }
    
    else if(selected[initial]==2){

        POINT A = {x+len-1+0.2,starts2[initial]-0.6}; // right side start of small block
        POINT B = {x+len-1+0.2,ends2[initial]+0.6}; // right side end of small block
        
        epb({A, B});

        // extreme are extended by 0.6 and others by 0.4
        POINT C,D,E,F;
        if(starts2[initial] > ends1[initial]){
            C = {x-0.6,starts1[initial]-0.6};
            D = {x-0.6,ends2[initial]+0.6};
            E = {x-0.4,starts2[initial]-0.6};
            F = {x-0.4,ends1[initial]+0.6};

            epb({C, D}); epb({E, F}); epb({B, D}); epb({A, E});
            preStart = C;
            preEnd = F;
        }
        else{
            C = {x-0.6,starts2[initial]-0.6};
            D = {x-0.6,ends1[initial]+0.6};
            E = {x-0.4,starts1[initial]-0.6};
            F = {x-0.4,ends2[initial]+0.6};
            
            epb({C, D}); epb({E, F}); epb({F, B}); epb({C, A});
            preStart = E;
            preEnd = D;
        }
    }

    int previndex = initial;

    bool iszero = 0;

    for(int i = initial+1;i<n;i++){
        int curr = selected[i];
        if(curr == 0){
            iszero = 1;
            continue;
        }

        int x = i*len;
        int prevx = previndex*len;

        if(iszero){
            iszero = 0;
            // previous large box's rightmost vertices and edges
            POINT X = {prevx+len-1+0.6,starts1[previndex]-0.6};
            POINT Y = {prevx+len-1+0.6,ends1[previndex]+0.4}; //0.2 for pipeline
            
            // previous box remaining 2 edges
            epb({X,Y});
            epb({X, preStart});


            if(curr == 2){

                // right side of small block
                POINT A = {x+len-1+0.2,starts2[i]-0.6};
                POINT B = {x+len-1+0.2,ends2[i]+0.6};
                
                epb({A,B});

                POINT C,D,E,F,G,H;

                if(ends1[previndex] > max(ends1[i],ends2[i])){
                    C = {x-0.4,ends1[previndex]+0.6};
                    D = {x-0.6,ends1[previndex]+0.4};
                    E = {x-0.4, max(ends1[i], ends2[i]) + 0.6};
                    G = {x-0.4, min(ends1[i], ends2[i]) + 0.6};
                    F = {x-0.4,max(starts1[i], starts2[i])-0.6};
                    H = {x-0.6,min(starts1[i], starts2[i])-0.6};

                    epb({F,G}); epb({C,E}); epb({D,H}); epb({D,Y}); epb({C,preEnd});


                    if(ends2[i]>ends1[i]){
                        epb({E,B}); epb({F,A});
                        preEnd = G;
                        preStart = H;
                    }
                    else{
                        epb({G,B}); epb({H,A});
                        preEnd = E;
                        preStart = F;
                    }

                }
                else if(ends1[previndex] < min(starts1[i],starts2[i])){
                    C = {x-0.4,ends1[previndex]+0.4};
                    D = {x-0.6,ends1[previndex]+0.6};
                    E = {x-0.4,min(starts1[i], starts2[i]) -0.6};
                    F = {x-0.4,min(ends1[i], ends2[i]) +0.6};
                    G = {x-0.4,max(starts1[i], starts2[i]) -0.6};
                    H = {x-0.6,max(ends1[i], ends2[i])+0.6};
                    
                    epb({D, preEnd});  epb({Y,C}); epb({E,C}); epb({D,H}); epb({G,F});


                    if(ends2[i]>ends1[i]){
                        epb({B,H}); epb({A,G});
                        preEnd = F;
                        preStart = E;
                    }
                    else{
                        epb({F,B}); epb({E,A});
                        preEnd = H;
                        preStart = G;
                    }

                }
                else{
                    C = {x-0.6,max(ends1[i], ends2[i])+0.6};
                    F = {x-0.6,min(starts1[i], starts2[i])-0.6};
                    E = {x-0.4,min(ends1[i], ends2[i])+0.6};
                    D = {x-0.4,max(starts1[i],starts2[i])-0.6};
                    H = {x-0.6,ends1[previndex]+0.6};
                    G = {x-0.6,ends1[previndex]+0.4};

                    epb({preEnd,H}); epb({Y,G}); epb({H,C}); epb({F,G}); epb({D,E}); 


                    if(ends2[i]>ends1[i]){
                        epb({C,B}); epb({D,A});
                        preEnd = E;
                        preStart = F;
                    }
                    else{
                        epb({E,B}); epb({F,A});
                        preEnd = C;
                        preStart = D;
                    }

                }

            }

            if(curr == 1){
                POINT C,D,E,F;
                if(ends1[previndex] > ends1[i]){
                    C = {x-0.6,ends1[previndex]+0.4};
                    D = {x-0.4,ends1[previndex]+0.6};
                    E = {x-0.4,ends1[i]+0.6};
                    F = {x-0.6,starts1[i]-0.6};

                    epb({preEnd,D}); epb({Y,C}); epb({D,E}); epb({C,F});
                    preEnd = E;
                    preStart = F;

                }
                else if(ends1[previndex] < starts1[i]){
                    C = {x-0.4,ends1[previndex]+0.4};
                    D = {x-0.6,ends1[previndex]+0.6};
                    E = {x-0.4,starts1[i]-0.6};
                    F = {x-0.6,ends1[i]+0.6};

                    epb({preEnd,D}); epb({Y,C}); epb({C,E}); epb({D,F});
                    preEnd = F;
                    preStart = E;

                }
                else{
                    C = {x-0.6,ends1[i]+0.6};
                    D = {x-0.6,ends1[previndex]+0.6};
                    E = {x-0.6,ends1[previndex]+0.4};
                    F = {x-0.6,starts1[i]-0.6};

                    epb({preEnd,D}); epb({Y,E}); epb({D,C}); epb({E,F});
                    preEnd = C;
                    preStart = F;

                }
            }

        }
        
        else{
            if(curr==1){
                POINT C,D,E,F;

                if(starts1[previndex] == starts1[i] && (ends1[previndex] == ends1[i])){
                    previndex = i;
                    continue;
                }

                if(starts1[previndex] == starts1[i]){
                    if(ends1[i]>ends1[previndex]){
                        C = {x-0.6, ends1[i]+0.6};
                        D = {x-0.6, ends1[previndex]+0.6};
                    }
                    else{
                        C = {x-0.4, ends1[i]+0.6};
                        D = {x-0.4, ends1[previndex]+0.6};
                    }

                    epb({C,D}); epb({preEnd,D});
                    preEnd = C;

                }
                else if(ends1[previndex] == ends1[i]){
                    if(starts1[i]>starts1[previndex]){
                        C = {x-0.4, starts1[i]-0.6};
                        D = {x-0.4, starts1[previndex]-0.6};
                    }
                    else{
                        C = {x-0.6, starts1[i]-0.6};
                        D = {x-0.6, starts1[previndex]-0.6};
                    }
                    
                    epb({C,D}); epb({D, preStart});
                    preStart = C;
                }
                else{
                    if(ends1[i] > ends1[previndex]){
                        C = {x-0.6, ends1[i]+0.6};
                        D = {x-0.6, ends1[previndex]+0.6};

                        if(starts1[i] > starts1[previndex]){
                            E = {x-0.4, starts1[i]-0.6};
                            F = {x-0.4, starts1[previndex]-0.6};
                        }
                        else{
                            E = {x-0.6, starts1[i]-0.6};
                            F = {x-0.6, starts1[previndex]-0.6};
                        }

                        
                        epb({C,D}); epb({E,F}); epb({preEnd,D}); epb({preStart,F});
                        preStart = E;
                        preEnd = C;
                    }
                    else{
                        C = {x-0.4, ends1[previndex]+0.6};
                        D = {x-0.4, ends1[i]+0.6};

                        if(starts1[i] > starts1[previndex]){
                            E = {x-0.4, starts1[previndex]-0.6};
                            F = {x-0.4, starts1[i]-0.6};
                        }
                        else{
                            E = {x-0.6, starts1[previndex]-0.6};
                            F = {x-0.6, starts1[i]-0.6};
                        }

                        epb({C,D}); epb({E,F}); epb({preEnd,C}); epb({preStart,E});
                        preStart = F;
                        preEnd = D;
                    }

                }

            }

            else{
                // curr = 2;
                POINT A,B,C,D,E,F,G,H;
                A = {x+len-1+0.2, starts2[i]-0.6};
                B = {x+len-1+0.2, ends2[i]+0.6};

                C = {x-0.4, max(starts1[i], starts2[i]) -0.6};
                D = {x-0.4, min(ends1[i], ends2[i]) +0.6};

                epb({A,B}); epb({C,D});

                double newStart = min(starts1[i], starts2[i]);
                double newEnd = max(ends1[i], ends2[i]);

                if(newEnd > ends1[previndex]){
                    E = {x-0.6, newEnd+0.6};
                    F = {x-0.6, ends1[previndex]+0.6};

                    if(newStart > starts1[previndex]){
                        G = {x-0.4, newStart-0.6};
                        H = {x-0.4, starts1[previndex]-0.6};
                    }
                    else{
                        G = {x-0.6, newStart-0.6};
                        H = {x-0.6, starts1[previndex]-0.6};
                    }

                    epb({preEnd,F}); epb({preStart,H});  epb({G,H}); epb({E,F});
                    
                    if(ends2[i]>ends1[i]){
                        epb({E,B}); epb({C,A});
                        preStart = G;
                        preEnd = D;
                    }
                    else{
                        epb({D,B}); epb({G,A});
                        preStart = C;
                        preEnd = E;
                    }
                }
                else{
                    E = {x-0.4, ends1[previndex]+0.6};
                    F = {x-0.4, newEnd+0.6};

                    if(newStart > starts1[previndex]){
                        G = {x-0.4, newStart-0.6};
                        H = {x-0.4, starts1[previndex]-0.6};
                    }
                    else{
                        G = {x-0.6, newStart-0.6};
                        H = {x-0.6, starts1[previndex]-0.6};
                    }

                    epb({preEnd,E}); epb({preStart,H});  epb({G,H}); epb({E,F});

                    if(ends2[i]>ends1[i]){
                        epb({F,B}); epb({C,A});
                        preStart = G;
                        preEnd = D;
                    }
                    else{
                        epb({D,B}); epb({G,A});
                        preStart = C;
                        preEnd = F;
                    }


                }

            }
        }

        previndex = i;
    }
    POINT lastS, lastE;
    int lastX = len*previndex + len - 1 + 0.6;
    lastS = {lastX, starts1[previndex]-0.6};
    lastE = {lastX, ends1[previndex]+0.6};
    epb({lastE, lastS});
    epb({preEnd, lastE});
    epb({preStart, lastS});


    return edgeList;
}


signed main(){
    // ios::sync_with_stdio(0); cin.tie(0);

    int testcases;
    cout << "Enter the number of testcases : ";
    cin >> testcases;

    for(int tc = 0; tc < testcases; tc++){
        cout<<endl<<"Case: "<<tc<<endl;
        string filename = "input" + string(tc < 10 ? "0" : "" ) + (to_string(tc)) + ".txt";
        getGrid(filename);

        int totPos = allPositivesSum();
        int bestWidth = -1;
        int ans = 0;
        int used;
        vector<int> widths = {1, 40, 50};
        cout<< "Normal grid: " <<endl;
        for(int width : widths){
            starts1.clear();
            ends1.clear();
            starts2.clear();
            ends2.clear();
            maxes1.clear();
            maxes2.clear();
            dp0.clear();
            dp1.clear();
            dp2.clear();
            pair<int, int> temp = getOptimalValue(width);
            cout << width << " : " << (temp.first * 100.00)/totPos << "\n";
            if(temp.first > ans){
                ans = temp.first;
                used = temp.second;
                bestWidth = width;
            }
        }
        bool transpose = false;
        getGridTranspose(filename);

        cout<<endl<< "Transpose grid: "<<endl;
        for(int width : widths){
            starts1.clear();
            ends1.clear();
            starts2.clear();
            ends2.clear();
            maxes1.clear();
            maxes2.clear();
            dp0.clear();
            dp1.clear();
            dp2.clear();
            pair<int, int> temp = getOptimalValue(width);
            cout << width << " : " << (temp.first * 100.00)/totPos << "\n";
            if(temp.first > ans){
                ans = temp.first;
                used = temp.second;
                bestWidth = width;
                transpose = true;
            }
        }

        if(!transpose) getGrid(filename);

        starts1.clear();
        ends1.clear();
        starts2.clear();
        ends2.clear();
        maxes1.clear();
        maxes2.clear();
        dp0.clear();
        dp1.clear();
        dp2.clear();
        pair<int, int> temp = getOptimalValue(bestWidth);
        ans = temp.first;
        used = temp.second;

        vector<int> selected = getSubarrays(bestWidth,used);

        int veri=0;
        for(int i = 0; i < selected.size(); ++i){
            if(selected[i] == 1) veri += maxes1[i];
            else if(selected[i] == 2){
                veri += maxes1[i];
                veri += maxes2[i];
            }
        };


        cout << endl<< "Bestwidth: " << bestWidth<< ", Transpose = "<<transpose << "\n";
        cout << ans << endl;
        cout << veri << endl;

        cout<<  (ans * 100.0) / totPos<<endl;
        cout<<  (veri * 100.0) / totPos<<endl;

        cout << totPos << endl;

        vector<EDGE> edgeList = getEdges(selected, bestWidth);
        int numEdges = edgeList.size();

        cout << "Edges used : " << numEdges << "\n";

        ofstream edgeOut("57_optimization_output0" + (to_string(tc)) + ".txt");
        edgeOut << ans << "\n";
        edgeOut << numEdges << ", " << numEdges << "\n";
        for (int i = 0; i < numEdges; i++)
        {
            if(edgeList[i].first.first > 0){
                edgeList[i].first.first++;
            }
            if(edgeList[i].second.first > 0){
                edgeList[i].second.first++;
            }
            if(transpose){
                swap(edgeList[i].first.first , edgeList[i].first.second);
                swap(edgeList[i].second.first , edgeList[i].second.second);
            }
            edgeOut << "(" << edgeList[i].first.first<<", "<<edgeList[i].first.second<<"), ("<<
            edgeList[i].second.first<<", "<<edgeList[i].second.second<<")\n";

        }
    }

    return 0;
}

// END OF CODE