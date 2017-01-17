#include <iostream>
#include <string>
#include<fstream>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <stack>
#include <queue>


using namespace std;
struct Point
{
	Point *parent;
	int x;
	int y;
	int value;
	bool operator==(Point &other)
	{
		return (this->x == other.x) && (this->y == other.y) && (this->value == other.value);
	}
};

class Table
{
public:
	Table();
	void fill_random(int size);
	void fill_file(string path_file);
	void travers_and_print();
	void show_bfs_result();
	bool find_way_BFS();
	bool stack_order();

private:
	Point **table;
	bool ** color;
	int n;
	Point final_p;
	void print_path(Point *p);
	Point left(int x, int y);
	Point right(int x, int y);
	Point up(int x, int y);
	Point down(int x, int y);
	Point ur(int x, int y);
	Point ul(int x, int y);
	Point dr(int x, int y);
	Point dl(int x, int y);
	bool successor(Point p, stack<Point> &st);
};

Table::Table()
{
	table = nullptr;
	n = 0;
}

void Table::fill_random(int size)
{
	n = size;
	srand(time(NULL));
	table = new Point*[size + 2];
	color = new bool*[size + 2];
	for (int i = 0; i <= size + 1; i++)
	{
		table[i] = new Point[size + 2];
		color[i] = new bool[size + 2]{ false };
	}
	for (int i = size; i >= 1; i--)
	{
		for (int j = 1; j <= size; j++)
		{
			int inf = rand() % 2;
			table[i][j].value = inf;
			table[i][j].x = i;
			table[i][j].y = j;
			table[i][j].parent = nullptr;
		}
	}
	for (int i = 0; i <= size + 1; i++)
	{
		table[0][i].value = 1;
		table[0][i].x = 0;
		table[0][i].y = i;
		table[0][i].parent = nullptr;

		table[i][0].value = 1;
		table[i][0].x = i;
		table[i][0].y = 0;
		table[i][0].parent = nullptr;

		table[size + 1][i].value = 1;
		table[size + 1][i].x = size;
		table[size + 1][i].y = i;
		table[size + 1][i].parent = nullptr;

		table[i][size + 1].value = 1;
		table[i][size + 1].x = i;
		table[i][size + 1].y = size;
		table[i][size + 1].parent = nullptr;
	}
	int x_of_final = rand() % (size + 1);
	int y_of_final = rand() % (size + 1);
	while (y_of_final == 0 || x_of_final == 0 || y_of_final == size + 1 || x_of_final == size + 1)
	{
		x_of_final = rand() % (size + 1);
		y_of_final = rand() % (size + 1);
	}
	table[1][1].value = 5;
	table[x_of_final][y_of_final].value = 2;
	final_p = table[x_of_final][y_of_final];
}

void Table::fill_file(string path_file)
{
	fstream file(path_file);
	if (!file.is_open())
	{
		cout << "Cant open file";
		return;
	}
	int size;
	file >> size;
	n = size;
	table = new Point*[size + 2];
	color = new bool*[size + 2];
	for (int i = 0; i <= size + 1; i++)
	{
		table[i] = new Point[size + 2];
		color[i] = new bool[size + 2]{ false };
	}
	for (int i = size; i >= 1; i--)
	{
		for (int j = 1; j <= size; j++)
		{
			int inf;
			file >> inf;
			table[i][j].value = inf;
			table[i][j].x = i;
			table[i][j].y = j;
			table[i][j].parent = nullptr;
			if (inf == 2)
			{
				final_p = table[i][j];
			}
		}
	}
	for (int i = 0; i <= size + 1; i++)
	{
		table[0][i].value = 1;
		table[0][i].x = 0;
		table[0][i].y = i;
		table[0][i].parent = nullptr;

		table[i][0].value = 1;
		table[i][0].x = i;
		table[i][0].y = 0;
		table[i][0].parent = nullptr;

		table[size + 1][i].value = 1;
		table[size + 1][i].x = size;
		table[size + 1][i].y = i;
		table[size + 1][i].parent = nullptr;

		table[i][size + 1].value = 1;
		table[i][size + 1].x = i;
		table[i][size + 1].y = size;
		table[i][size + 1].parent = nullptr;
	}
}

void Table::travers_and_print()
{
	for (int i = 0; i <= n + 1; i++)
	{
		for (int j = 0; j <= n + 1; j++)
		{
			cout << table[i][j].value;
			if (table[i][j].parent)
				cout << table[i][j].parent->value;
			cout << "\t";
		}
		cout << "\n";
	}
}
void Table::show_bfs_result()
{
	print_path(final_p.parent);
	cout << "Food point is ( " << final_p.x << " , " << final_p.y << " )\n";

}
/*For each element in table we have 4 neighbours: up, down, right and left. We study this 4 neighbours.*/
bool Table::find_way_BFS()
{
	/*Initilize first element in table (0,0) point for start bfs algorithm*/
	color[1][1] = true;
	table[1][1].parent = &table[1][1];
	/*Empty Queue for bfs algorithm whit two elements in pair data structure: width and height. First element show 'x' and second element show 'y'. */
	queue<pair<int, int>> q;
	pair<int, int> p, w;
	p.first = 1;
	p.second = 1;
	/*Insert first element in table (0,0) point.
	Let imaging our table like this:
	0 1 2 3
	^ ^ ^ ^
	| | | |
	| | | |
	| | | |

	1 1 1 2 ------>3
	1 1 1 0 ------>2
	0 1 1 0 ------>1
	0 0 0 0 ------>0

	that cordinate (3,3) with 2 value is our goal point. For example left neighbour of (0,0) point is not exict. For this reasons we fence our table with elements that have value=1 and we started bfs algorithm from (1,1).
	0 1 2 3 4
	^ ^ ^ ^ ^
	| | | | |
	| | | | |
	| | | | |

	1 1 1 1 1 1
	1 1 1 1 2 1 ------>4
	1 1 1 1 0 1 ------>3
	1 0 1 1 0 1 ------>2
	1 0 0 0 0 1 ------>1
	1 1 1 1 1 1 ------>0
	*/
	q.push(p);
	while (!q.empty())
	{
		/*Dequeue form queue*/
		pair<int, int> u = q.front();
		q.pop();
		/*We arrived to goal point*/
		if (table[u.first][u.second].value == 2)
		{
			final_p = table[u.first][u.second];
			return true;
		}
		/*Check UP neighbour */
		if (table[u.first + 1][u.second].value != 1 && color[u.first + 1][u.second] == false)
		{
			color[u.first + 1][u.second] = true;
			table[u.first + 1][u.second].parent = &table[u.first][u.second];
			w.first = u.first + 1;
			w.second = u.second;
			q.push(w);
		}

		/*Check Left neighbour*/
		if (table[u.first][u.second - 1].value != 1 && color[u.first][u.second - 1] == false)
		{
			color[u.first][u.second - 1] = true;
			table[u.first][u.second - 1].parent = &table[u.first][u.second];
			w.first = u.first;
			w.second = u.second - 1;
			q.push(w);
		}
		/*Check Down neighbour*/
		if (table[u.first - 1][u.second].value != 1 && color[u.first - 1][u.second] == false)
		{

			color[u.first - 1][u.second] = true;
			table[u.first - 1][u.second].parent = &table[u.first][u.second];
			w.first = u.first - 1;
			w.second = u.second;
			q.push(w);
		}
		/*Check Right neighbour*/
		if (table[u.first][u.second + 1].value != 1 && color[u.first][u.second + 1] == false)
		{

			color[u.first][u.second + 1] = true;
			table[u.first][u.second + 1].parent = &table[u.first][u.second];
			w.first = u.first;
			w.second = u.second + 1;
			q.push(w);
		}
	}
	cout << "\n\n\n";
	return false;
}

void Table::print_path(Point *p)
{
	if (p->parent != p)
		print_path(p->parent);
	else
	{
		cout << "Mouse Point is ( " << p->x << " , " << p->y << " )\n";
		return;
	}
	cout << "Way Point is ( " << p->x << " , " << p->y << " )\n";
}

Point Table::left(int x, int y)
{
	Point res;
	res = table[x][y - 1];

	return res;
}

Point Table::right(int x, int y)
{
	Point res;
	res = table[x][y + 1];

	return res;
}

Point Table::up(int x, int y)
{
	Point res;
	res = table[x + 1][y];

	return res;
}

Point Table::down(int x, int y)
{
	Point res;
	res = table[x - 1][y];

	return res;
}

Point Table::ur(int x, int y)
{
	Point res;
	res = table[x + 1][y + 1];

	return res;
}

Point Table::ul(int x, int y)
{
	Point res;
	res = table[x + 1][y - 1];

	return res;
}

Point Table::dr(int x, int y)
{
	Point res;
	res = table[x - 1][y + 1];

	return res;
}

Point Table::dl(int x, int y)
{
	Point res;
	res = table[x - 1][y - 1];

	return res;
}

bool Table::successor(Point p, stack<Point> &st)
{
	bool ret = false;
	if (right(p.x, p.y).value != 1 && color[p.x][p.y + 1] == false)
	{
		st.push(table[p.x][p.y + 1]);
		color[p.x][p.y + 1] = true;
		ret = true;
	}
	else if (ur(p.x, p.y).value != 1 && color[p.x + 1][p.y + 1] == false)
	{
		st.push(table[p.x + 1][p.y + 1]);
		color[p.x + 1][p.y + 1] = true;
		ret = true;
	}
	else if (up(p.x, p.y).value != 1 && color[p.x + 1][p.y] == false)
	{
		st.push(table[p.x + 1][p.y]);
		color[p.x + 1][p.y] = true;
		ret = true;

	}
	else if (ul(p.x, p.y).value != 1 && color[p.x + 1][p.y - 1] == false)
	{
		st.push(table[p.x + 1][p.y - 1]);
		color[p.x + 1][p.y - 1] = true;
		ret = true;
	}
	else if (left(p.x, p.y).value != 1 && color[p.x][p.y - 1] == false)
	{
		st.push(table[p.x][p.y - 1]);
		color[p.x][p.y - 1] = true;
		ret = true;

	}
	else if (dl(p.x, p.y).value != 1 && color[p.x - 1][p.y - 1] == false)
	{
		st.push(table[p.x - 1][p.y - 1]);
		color[p.x - 1][p.y - 1] = true;
		ret = true;
	}
	else if (down(p.x, p.y).value != 1 && color[p.x - 1][p.y] == false)
	{
		st.push(table[p.x - 1][p.y]);
		color[p.x - 1][p.y] = true;
		ret = true;
	}
	else if (dr(p.x, p.y).value != 1 && color[p.x - 1][p.y + 1] == false)
	{
		st.push(table[p.x - 1][p.y + 1]);
		color[p.x - 1][p.y + 1] = true;
		ret = true;
	}
	return ret;
}

bool Table::stack_order()
{
	stack<Point> st;
	bool is_avalabe = false;
	st.push(table[1][1]);
	while (!st.empty())
	{
		Point temp = st.top();
		if (temp == final_p)
		{
			is_avalabe = true;
			break;
		}
		if (!successor(temp, st))
		{
			st.pop();
		}
		if (!st.empty())
		{
			if (st.top() == table[1][1])
			{
				is_avalabe = false;
				break;
			}
		}

	}
	if (is_avalabe)
	{
		Point p = st.top();
		cout << "\nFood Point is ( " << p.x << " , " << p.y << " )\n";
		st.pop();
		while (!st.empty())
		{
			p = st.top();
			cout << "Way Point is ( " << p.x << " , " << p.y << " )\n";
			st.pop();
		}
	}
	else
	{
		cout << "Not solved\n";
	}
	return true;
}

int main()
{
	Table t;
	t.fill_random(4);
	t.travers_and_print();
	if (t.find_way_BFS() == true)
	{
		t.show_bfs_result();
	}
	else {
		cout << "There is no way to goal point.\n\n";
	}
	system("pause");
	return 0;
}