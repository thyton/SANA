#include "MultiS3.hpp"
#include <cmath>

unsigned MultiS3::denom = 0;
vector<uint> MultiS3::shadowDegrees;

MultiS3::MultiS3(Graph* G1, Graph* G2) : Measure(G1, G2, "ms3")
{
    G1->printStats(0, cout);
    G2->printStats(0, cout);
    initDegrees(*G2);
}

MultiS3::~MultiS3() {
}

void MultiS3::initDegrees(const Graph& G2)
{
    shadowDegrees = vector<uint>(G2.getNumNodes(), 0);
    
    vector<vector<uint>> G2AdjLists;
    G2.getAdjLists(G2AdjLists);
    
    Matrix<MATRIX_UNIT> G2Matrix;
    G2.getMatrix(G2Matrix);
    
    const uint n = G2AdjLists.size();
    uint neighbor;
    
    for (uint i = 0; i < n; ++i)
    {
        for (uint j = 0; j < G2AdjLists[i].size(); ++j)
        {
            neighbor = G2AdjLists[i][j];
            shadowDegrees[i] += G2Matrix[i][neighbor];
        }
    }
}

void MultiS3::getDenom(const Alignment& A, const Graph& G1, const Graph& G2)
{
#if 0
    denom = A.numExposedEdges(G1, G2);
    LaddersUnderG1 = denom;
#else
    vector<vector<uint>> G1EdgeList;
    G1.getEdgeList(G1EdgeList);
    uint node1, node2;
    LaddersUnderG1 = 0;
    unordered_set<uint> pegs;
    
    for (const auto& edge : G1EdgeList)
    {
        node1 = edge[0], node2 = edge[1];
        pegs.insert(A[node1]);
        pegs.insert(A[node2]);
    }
    
    for (const auto& peg : pegs)
    {
        if (shadowDegrees[peg] > 0)
        {
            ++LaddersUnderG1;
        }
    }
    denom = LaddersUnderG1;
#endif
}

double MultiS3::eval(const Alignment& A) {
#if MULTI_PAIRWISE
    getDenom(A, *G1, *G2);
    return double (A.multiS3Numerator(*G1, *G2)) / LaddersUnderG1;
#else
    return 0.0;
#endif
}
