#pragma once

#include "BSTh.h"
int Tree::Count;
myiter Tree::begin()const {
	MyStack St;
	Node * p(root);
	if (p) {
		while (p->L[0]) {
			St.push(make_pair(p, 0));
			p = p->L[0];
		}
	}
	return myiter(p, move(St));
}
myiter& myiter::operator++()
{
	if (!Ptr) {
		return *this;
	}
	if (Ptr->L[1]) {
		St.push(make_pair(Ptr, 1));
		Ptr = Ptr->L[1];
		while (Ptr->L[0]) {
			St.push(make_pair(Ptr, 0));
			Ptr = Ptr->L[0];
		}
	}
	else {
		pair<Node*, int> pp(Ptr, 1);
		while (!St.empty() && pp.second) { pp = St.top(); St.pop(); }
		if (pp.second) {
			Ptr = nullptr;
		}
		else Ptr = pp.first;
	}
	return (*this);
}

const int FIRSTROW = 0, FIRSTCOL = 60,
MAXCOL = 120, OFFSET[] = { 60, 23, 12, 6, 3, 2, 1 },
MAXROW = FIRSTROW + 9,
MAXOUT = FIRSTROW + 6, SHIFT = 2;

string SCREEN[MAXROW];

void clrscr(int f = 1)
{
	for (auto i = 0; i < MAXROW; ++i) {
		SCREEN[i].clear();
		SCREEN[i].resize(MAXCOL + 20, '.');
	}
}

void showscr()
{
	for (auto i = 0; i < MAXROW; ++i) {
		SCREEN[i].resize(MAXCOL, '.');
		cout << SCREEN[i] << '\n';
	}
}

int setval(string &s, int pos, int val) {
	string t(to_string(val));
	for (auto p : t) s[pos++] = p;
	return t.size();
}

void Tree::Display(int first)
{
	clrscr(first);
	SCREEN[0] = "BSTh (H=" + to_string(H()) + " n=" + to_string(n)
		+ ") --------->";
	if (root) {
		SCREEN[0].resize(MAXCOL, '.');
		root->Display(0, FIRSTCOL, 1);
	}
	else SCREEN[0] += "<Empty!>";
	showscr();
}

void Node::Display(int row, int col, int depth)
{
	if ((row > MAXROW) || (col < 0) || (col > MAXCOL)) return;

	if (row > MAXOUT) {
		SCREEN[row].replace(col, 3, "+++");
		return;
	}

	try {
		setval(SCREEN[row], col, key);
		setval(SCREEN[row + 1], col, h);
	}
	catch (exception &e) {
		cout << e.what() << key << ' ' << row << ' ' << col << endl;
		cin.get();
	}
	catch (...) { cout << "Unknown error\n"; cin.get(); }
	if (L[0]) L[0]->Display(row + 1, col - OFFSET[depth], depth + 1);
	if (L[1]) L[1]->Display(row + 1, col + OFFSET[depth], depth + 1);
}
myiter Tree::find(int k)const
{
	Node*p(root);
	while (p && p->key != k) p = p->L[p->key > k];
	return myiter(p);
}
pair<myiter, bool> Tree::insert(int k, myiter where)
{
	Node *p(root), *q(nullptr);
	int a{ 0 };
	MyStack St;

	if (!where.Ptr) {
		if (!root) {
			root = new Node(k);
			n = 1;
			return make_pair(myiter(root, move(St)), true);
		}
	}
	else {
		p = where.Ptr;
		St = move(where.St);
	}


	while (p)
	{
		a = k > p->key ? 1 : 0;
		St.push(make_pair(p, a));
		q = p->L[a];
		if (q) {
			p = q;
		}
		else {
			p->L[a] = q = new Node(k);
			++n;


			break;
		}
	}


	int b_old{0};
	while (!St.empty())
	{
		auto pa = St.top(); St.pop();
		p = pa.first; a = pa.second;
		int b(p->balancefactor());
		if(b) {
			if ((b == 2) || (b == -2))
			{
				--p->h;
				b /= 2;
				if (b == b_old) {
					p->L[a] = q->L[1 - a];
					q->L[1 - a] = p;

					if (p == root)p = root = q;
				    else St.top().first->L[St.top().second] = p = q;
					p->fixheight();
					break;
			    }
				else {
					Node *r(q->L[1 - a]);
					p->L[a] = r->L[1 - a];
					q->L[1 - a] = r->L[a];
					r->L[1 - a] = p;
					r->L[a] = q;

					if (p == root) p = root = r;
					else St.top().first->L[St.top().second] = p = r;
					p->fixheight();
					break;
				}
			}
			b_old = b;
			p->fixheight();
			q = p;
		}
		else break;
	}
	return make_pair(myiter(p, move(St)), true);
}

pair<myiter, bool> Tree::erase(int k)
{

	MyStack St;
	Node *p(nullptr), *q(root), *r(nullptr);
	int a(0), B[] = { -1, +1 };
	bool cont = (q != nullptr);

	while (cont && (k != q->key)) {
		St.push(make_pair(p, a));
		p = q;
		a = k > q->key;
		q = q->L[a];
		if (q == nullptr) cont = false;
	}
	if (cont) {
		--n;
		if (r = q->L[1]) {
			if (r->L[0]) {
				St.push(make_pair(p, a));
				p = q;
				a = 1;
				do {
					St.push(make_pair(p, a));
					p = r;
					r = r->L[a = 0];
				} while (r->L[0]);
				q->key = r->key;
				p->L[0] = r->L[1];
				p->fixheight();
				q = r;
			}
			else {
				r->L[0] = q->L[0];
				if (p) p->L[a] = r;
				else root = r;
				St.pop();
				p = r;
				a = 1;
			}
		}
		else {
			if (p) {
				p->L[a] = q->L[0];
				p->fixheight();
			}
			else {
				root = q->L[0];
				root->fixheight();
			}
		}
		q->L[0] = q->L[1] = nullptr;
		delete q;

		while (cont) {
			cont = false;
			if (!p) break;
			int b(p->balancefactor());
			if (b != 0) {
				cont = true;
				if (abs(b) < 2) {
					auto pp = St.top();
					St.pop();
					p = pp.first; a = pp.second;
				}
				else {
					q = r = p->L[1 - a];
					int rb(r->balancefactor());
					if (rb == -b/2) {
						r = r->L[a];
						p->L[1 - a] = r->L[a];
						q->L[a] = r->L[1 - a];
						r->L[a] = p;
						r->L[1 - a] = q;
					}
					else {
						p->L[1 - a] = r->L[a];
						r->L[a] = p;
						if (rb == 0) cont = false;
					}
					--p->h;
					r->fixheight();
					auto pp = St.top();
					St.pop();
					p = pp.first; a = pp.second;
					if (p) p->L[a] = r;
					else root = r;
				}
			}
		}
		return make_pair(myiter(q), true);
	}
	else return make_pair(myiter(q), false);
}

Tree & Tree::operator |= (const Tree & rgt) {
	Tree temp;
	set_union(begin(), end(), rgt.begin(), rgt.end(), outinserter(temp, myiter(nullptr)));
	swap(temp);
	return *this;
}

Tree & Tree::operator &= (const Tree & rgt) {
	Tree temp;
	set_intersection(begin(), end(), rgt.begin(), rgt.end(), outinserter(temp, myiter(nullptr)));
	swap(temp);
	return *this;
}

Tree& Tree::operator -= (const Tree & rgt) {
	Tree temp;
	set_difference(begin(), end(), rgt.begin(), rgt.end(), outinserter(temp, myiter(nullptr)));
	swap(temp);
	return *this;
}
Tree& Tree::operator ^= (const Tree & rgt) {
	Tree temp;
	set_symmetric_difference(begin(), end(), rgt.begin(), rgt.end(), outinserter(temp, myiter(nullptr)));
	swap(temp);
	return *this;
}
size_t Tree::tags = 0;
