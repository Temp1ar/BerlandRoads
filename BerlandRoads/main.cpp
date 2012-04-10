#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <stack>

using namespace std;

void addPairToGraph(map<int, vector<int>>& graph, int x, int y) {
	map<int, vector<int>>::iterator it;
	if((it = graph.find(x)) != graph.end()) {
		it->second.push_back(y);
	} else {
		graph.insert(make_pair(x, vector<int>(1,y)));
	}
}

void addThreesome(map<int, map<int, set<int>>>& tree, int a, int b, int c) {
	map<int, map<int, set<int>>>::iterator it;
	if((it = tree.find(a)) != tree.end()) {
		map<int, set<int>>::iterator it2;
		if((it2 = it->second.find(b)) != it->second.end()) {
			it2->second.insert(c);
		} else {
			set<int> set_;
			set_.insert(c);
			it->second.insert(make_pair(b, set_));
		}
	} else {
		set<int> set_;
		set_.insert(c);
		map<int, set<int>> map_;
		map_.insert(make_pair(b, set_));
		tree.insert(make_pair(a, map_));
	}
}

bool isProhibited(map<int, map<int, set<int>>>& tree, int a, int b, int c) {
	map<int, map<int, set<int>>>::iterator it;
	map<int, set<int>>::iterator it2;
	if((it = tree.find(a)) != tree.end()) {
		if((it2 = it->second.find(b)) != it->second.end()) {
			if(it2->second.find(c) != it2->second.end()) {
				return true;
			}
		}
	}
	return false;
}

bool isVisited(const set<pair<int, int>>& visited, const pair<int, int>& x) {
	return visited.find(x) != visited.end();
}

void addVisited(set<pair<int, int>>& visited, const pair<int, int>& x) {
	visited.insert(x);
}

int main() {
	int n, m, k;
	cin >> n;
	cin >> m;
	cin >> k;

	// I will store graph in map of vectors
	map<int, vector<int>> graph;
	for (int i = 0; i != m; ++i) {
		int x, y;
		cin >> x >> y;
		addPairToGraph(graph, x, y);
		addPairToGraph(graph, y, x);
	}

	// And the prohibited threesomes in set of char[3]
	map<int, map<int, set<int>>> prohibited;
	for (int i = 0; i != k; ++i) {
		int a, b, c;
		cin >> a >> b >> c;
		addThreesome(prohibited, a, b, c);
	}

	// Starting traverse (BFS-like)
	deque<pair<int, int>> queue_;
	map<pair<int, int>, pair<int, int>> previous;
	set<pair<int, int>> visited;
	// Starting point is city 1
	queue_.push_back(make_pair(0, 1));
	previous[make_pair(0, 1)] = make_pair(0, 0);

	int required = n;
	bool isFound = false;
	vector<int> answer;
	while (!isFound && queue_.size() != 0) {
		pair<int,int> current = queue_.front();
		queue_.pop_front();
		if(current.second == required) {
			stack<int> tmp;
			while(current.second != 0) {
				tmp.push(current.second);
				current = previous[current];
			}

			cout << tmp.size() - 1 << endl;
			while(!tmp.empty()) {
				cout << tmp.top() << " ";
				tmp.pop();
			}
			isFound = true;
			break;
		}

		vector<int>::iterator it = graph[current.second].begin();
		for(; it!= graph[current.second].end(); ++it) {
			pair<int, int> edge = make_pair(current.second, *it);
			if (!isProhibited(prohibited, current.first, current.second, *it) 
				&& !isVisited(visited, edge)) 
			{
				addVisited(visited, edge);
				queue_.push_back(edge);
				previous[edge] = current;
			}
		}
	}

	if(!isFound) 
		cout << -1;
	
	return 0;
}