#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// ==============================
// 資料結構定義
// ==============================
struct Edge {
    int u, v, w;//起點 終點 權重
};

// 並查集 Disjoint Set Union
class DSU {
private:
    vector<int> parent, rankv;//記錄父節點 後者用於union by rank

public:
    DSU(int n) {
        parent.resize(n + 1);//parent[1]...parent[n+1]
        rankv.resize(n + 1, 0);//rankv[1]=0...rank[n+1]=0
        for (int i = 1; i <= n; i++) {
            parent[i] = i;//初始化parent[i]=i
        }
    }

    int find(int x) {
        if(parent[x]!=x)
            parent[x]=find(parent[x]);// path compression路徑壓縮(如果parent[x]!=root就去找它的root)
        return parent[x];
    }

    bool unite(int a, int b) {
        int rootA=find(a);
        int rootB=find(b);

        if(rootA==rootB)return false;//MST規則中其一是不能形成cycle

        if(rankv[rootA]<rankv[rootB])
            parent[rootA]=rootB;//rankv小的人parent要跟著大的(被納入)
        else if(rankv[rootA]>rankv[rootB])
            parent[rootB]=rootA;
        else{
            parent[rootB]=rootA;//如果A跟B的root一樣
            rankv[rootA]++;//A的rankv就++
        }
        return true;
    }
};

// ==============================
// 工具函式
// ==============================
void printEdge(const Edge& e) {//印出邊的資訊
    cout << e.u << " - " << e.v << " : " << e.w;//分別代表 起點 種點 權重
}

void printMST(const vector<Edge>& mst) {
    int total = 0;
    cout << "\nMST edges:\n";
    for (const auto& e : mst) {
        printEdge(e);
        cout << "\n";
        total += e.w;//把權重(花費的成本)加起來
    }
    cout << "Total weight = " << total << "\n";
}

// ==============================
// 1. Kruskal Algorithm Template
// ==============================
void kruskalMST(int n, vector<Edge> edges) {
    cout << "==============================\n";
    cout << "Kruskal Algorithm\n";
    cout << "==============================\n";

    vector<Edge> mst;

    // TODO:
    // Step 1. 將 edges 依照權重由小到大排序
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w; // 權重小的邊排在前面
    });
    // TODO:
    // Step 2. 建立 DSU 物件
    DSU dsu(n);
    cout << "Selection steps:\n";

    // TODO:
    // Step 3. 逐一檢查排序後的每條邊
    for (const auto& e : edges) {
        // 嘗試合併兩個節點
        if (dsu.unite(e.u, e.v)) { 
            // 若成功合併 (不形成 cycle) → 選入 MST
            mst.push_back(e);
            cout << "Select edge: ";
            printEdge(e);
            cout << "\n";
        }
        // 當 MST 邊數 = n-1 時，停止
        if (mst.size() == n - 1)
            break;
    }
    printMST(mst);
    cout << "\n";
}

// ==============================
// 2. Prim Algorithm Template
//    從 start 開始
// ==============================
void primMST(int n, const vector<vector<pair<int, int>>>& adj, int start = 1) {
    cout << "==============================\n";
    cout << "Prim Algorithm\n";
    cout << "==============================\n";

    vector<bool> inMST(n + 1, false);
    vector<Edge> mst;//存放mst選到的邊 最後會印出來
    //adj pair(int,int)->(鄰居節點,權重)
    // 最小堆: {權重, from, to}
    priority_queue<
        tuple<int, int, int>,
        vector<tuple<int, int, int>>,
        greater<tuple<int, int, int>>
    > pq;

    // TODO:
    inMST[start]=true;
    // Step 1. 將起點 start 設為已加入 MST

    // TODO:
    for(auto & p: adj[start]){
        int to=p.first;//鄰居節點
        int w=p.second;//權重
        pq.push({w,start,to});//加入pq(候選邊集合)
    }
    // Step 2. 把 start 相鄰的邊放入 priority queue

    cout << "Selection steps:\n";

    // TODO:
    while(!pq.empty()&&mst.size()<n-1){
        auto[w,from,to]=pq.top();
        pq.pop();
        if(inMST[to])continue;
        
        mst.push_back({from,to,w});
        cout<<"Select edge"<<from<<"-"<<to<<":"<<w<<"\n";
        inMST[to]=true;//將新頂點標記進MST

        for(auto & p:adj[to]){
            int next=p.first;
            int weight=p.second;
            if(!inMST[next]){
                pq.push({weight,to,next});
            }
        }
    }
    // Step 3. 當 pq 不為空且 mst 邊數 < n-1
    //   - 取出最小邊
    //   - 若 to 已在 MST 中，跳過
    //   - 否則加入此邊到 mst，並把新頂點標記進 MST
    //   - 再將新頂點可到達的候選邊放入 pq

    printMST(mst);
    cout << "\n";
}

// ==============================
// 3. Sollin / Boruvka Template
// ==============================
void boruvkaMST(int n, const vector<Edge>& edges) {
    cout << "==============================\n";
    cout << "Sollin / Boruvka Algorithm\n";
    cout << "==============================\n";

    vector<Edge> mst;

    // TODO:
    DSU dsu(n);
    // Step 1. 建立 DSU
    // Step 2. 初始 component 數量為 n
    int numComponents = n;
    int round = 1;

    while (numComponents > 1) {
        cout << "Round " << round << ":\n";

        // cheapest[i] = 第 i 個 component 目前找到的最便宜邊的 index
        vector<int> cheapest(n + 1, -1);

        // TODO:
        // Step 3. 掃描所有邊，對每個 component 找 cheapest edge
        for (int i = 0; i < edges.size(); i++) {

            int set1 = dsu.find(edges[i].u);
            int set2 = dsu.find(edges[i].v);

            if (set1 == set2) continue;

            if (cheapest[set1] == -1 ||
            edges[i].w < edges[cheapest[set1]].w)
            cheapest[set1] = i;

            if (cheapest[set2] == -1 ||
            edges[i].w < edges[cheapest[set2]].w)
            cheapest[set2] = i;
        }
        // 提示：
        //   set1 = find(edges[i].u)
        //   set2 = find(edges[i].v)
        //   若 set1 == set2，代表同一 component，跳過
        //   否則更新 cheapest[set1] 與 cheapest[set2]

        bool merged = false;

        // TODO:
        // Step 4. 將每個 component 找到的 cheapest edge 嘗試加入 MST
        for(int i=1;i<=n;i++){
            if(cheapest[i]!=-1){
                Edge e = edges[cheapest[i]];
                int set1 = dsu.find(e.u);
                int set2 = dsu.find(e.v);
                if (set1 == set2) continue;
                if (dsu.unite(set1, set2)) {
                    mst.push_back(e);

                    cout << "Select edge: ";
                    printEdge(e);
                    cout << "\n";

                    numComponents--;
                    merged = true;
                }
            }
        }
        //   - 若兩端點屬於不同 component，則合併
        //   - 加入 mst
        //   - numComponents--
        //   - merged = true

        if (!merged) break;

        cout << "\n";
        round++;
    }

    printMST(mst);
    cout << "\n";
}

// ==============================
// 主程式
// ==============================
int main() {
    int n = 6;

    // 圖中的無向邊
    vector<Edge> edges = {
        {1, 2, 16},
        {1, 5, 19},
        {5, 4, 18},
        {2, 4, 6},
        {2, 3, 5},
        {4, 3, 10},
        {1, 6, 22},
        {5, 6, 33},
        {6, 2, 12},
        {6, 4, 14}
    };

    // adjacency list，供 Prim 使用
    vector<vector<pair<int, int>>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.u].push_back({e.v, e.w});
        adj[e.v].push_back({e.u, e.w});
    }

    kruskalMST(n, edges);
    primMST(n, adj, 1);
    boruvkaMST(n, edges);

    return 0;
}
