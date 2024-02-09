#include <memory>
#include <iostream>
#include <vector>

class Graph
{
public:
    void add_edge(int id1, int id2)
    {
        std::weak_ptr<Node> ptr = _nodes[id2];
        _nodes[id1]->edges.push_back(ptr);
    }

    void dfs(int start)
    {
        std::weak_ptr<Node> ptr = _nodes[start];
        _dfs(ptr);
    }

    void add_node(int val)
    {
        _nodes.push_back(std::make_shared<Node>(val));
    }

private:

    struct Node
    {
        int val;
        std::vector<std::weak_ptr<Node>> edges;
        Node(int x) : val(x){}
        Node() = default;
    };

    void _dfs(std::weak_ptr<Node> start)
    {
        auto shared = start.lock();
        if (shared)
        {
            std::cout << shared ->val << std::endl;
            for (auto edge: shared->edges)
            {
                _dfs(edge);
            }
        }
    }

    std::vector<std::shared_ptr<Node>> _nodes;
};


int main()
{
    Graph g{};

    for (int i = 0; i <= 10; i++)
    {
        g.add_node(i);
    }

    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 4);
    g.add_edge(2, 5);
    g.add_edge(3, 6);
    g.add_edge(5, 7);
    g.add_edge(7, 8);
    g.add_edge(4, 9);
    g.add_edge(4, 10);

    g.dfs(1);

    // Note that if you run with valgrind, there are no
    // leaks even with cyclic edges.
    g.add_edge(2,1);
}

/*
      1
   /    \
  2      3
 /  \     \
4    5     6
| \   \  
9  10  7  
        \
         8
*/