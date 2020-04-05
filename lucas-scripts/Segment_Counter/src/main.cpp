#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

int num_segment;
vector< vector< pair<int,int>> > adjList;
vector<int> visit;

void printGraph ()
{
    for (int i = 0; i < (int)adjList.size(); i++)
    {
        printf("|| %d ||",i+1);
        for (int j = 0; j < (int)adjList[i].size(); j++)
            printf(" --> || %d ||",adjList[i][j].first+1);
        printf("\n");
    }
}

void dfs (int u)
{
    int contDFS = 0;
    visit[u] = 1;
    for (int j = 0; j < (int)adjList[u].size(); j++)
    {
        pair<int,int> v = adjList[u][j];
        if (visit[v.first] == 0)
        {
            contDFS++;
            dfs(v.first);
        }
    }
    if (adjList[u].size() == 0)
        num_segment++;
    if (contDFS == 2)
        num_segment++;
}

int calcTotalSize ()
{
    int total_size = 0;
    for (int i = 0; i < (int)adjList.size(); i++)
    {
        for (int j = 0; j < (int)adjList[i].size(); j++)
            total_size += adjList[i][j].second;
    }
    return total_size;
}

int main ()
{
    int num_nodes, num_edges;
    scanf("%d %d",&num_nodes,&num_edges);
    adjList.assign(num_nodes,vector< pair<int,int> >());
    for (int i = 0; i < num_edges; i++)
    {
        int u, v, w;
        scanf("%d %d %d",&u,&v,&w);
        u--; v--;
        adjList[u].push_back(make_pair(v,w));
    }
    int total_size = calcTotalSize();
    visit.assign(num_nodes,0);
    dfs(0);
    printf("=================================================================\n");
    printf("Number of segments = %d\n",num_segment);
    printf("Mean size of the segments = %.5lf\n",total_size / (double)num_segment);
    printf("Total size of tree = %d\n",total_size);
    printf("=================================================================\n");

    //printGraph();
}
