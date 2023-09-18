#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;
struct m
{
	string value;
	bool care;// false is don't care
	vector<int> no;
	bool operator< (const m &a)const
	{
		if (a.no.size() == no.size())
			for (int i = 0; i < no.size(); i++)
				if (a.no[i] != no[i])
					return a.no[i] > no[i];
		return a.no[0] > no[0];
	}
};
struct pi
{
	int no;
	string value;
	int essential = 0;
	bool choose = false;// true is to be selected
	bool operator< (const pi &a)const
	{
		if (a.essential == essential)
			return a.no > no;
		return a.essential > essential;
	}
};
typedef vector< m > mVec;

void findPI(vector< mVec > input, vector< mVec > &PIs, set< pi > &count);
vector< mVec >::iterator ePI(set< m > &output, set< pi > &count, int no, vector< mVec > &PI);

int main()
{
	int num;// number of cases
	int minterm[16] = { 0, 1, 3, 2, 4, 5, 7, 6, 12, 13, 15, 14, 8, 9, 11, 10 };
	ifstream fin("Case1");
	ofstream fout("output.txt");

	fin >> num;
	for (int i = 0; i < num; i++)
	{
		int kmap[16];
		set< pi > count;
		vector< mVec > minterms;
		minterms.resize(5);// minterm which value == 1 || value == 2

		fout << "#" << i + 1 << endl;
		for (int j = 0; j < 16; j++)
		{
			fin >> kmap[j];
			if (kmap[j])// kmap[j] != 0
			{
				string value = "0000";
				int amount = 0;// number of 1 in minterm
				for (int k = minterm[j], l = value.size() - 1; k; k /= 2, l--)
				{
					value[l] = '0' + k % 2;
					if (k % 2) amount++;// k % 2 == 1
				}
				m buffer = { value, kmap[j] == 1 };// kmap[j] == 1 ? true : false
				buffer.no.push_back(minterm[j]);
				minterms[amount].push_back(buffer);
				if (buffer.care)
				{
					pi temp = { minterm[j], value };
					count.insert(temp);
				}
			}
		}
		vector< mVec > two_PI, four_PI, eight_PI;
		two_PI.resize(5);// 2's prime implicant
		four_PI.resize(5);// 4's prime implicant
		eight_PI.resize(5);// 8's prime implicant
		findPI(minterms, two_PI, count);
		findPI(two_PI, four_PI, count);
		findPI(four_PI, eight_PI, count);
		set< m > output;
		for (int j = 0; j < 5; j++)
			for (set< pi >::iterator sit = count.begin(); sit != count.end();)
				if (!sit->choose)
				{
					if (ePI(output, count, sit->no, eight_PI) == eight_PI.end())
						if (ePI(output, count, sit->no, four_PI) == four_PI.end())
							if (ePI(output, count, sit->no, two_PI) == two_PI.end())
							{
								m buffer = { sit->value, true };
								buffer.no.push_back(sit->no);
								output.insert(buffer);
								pi temp = { sit->no, sit->value, sit->essential, true };
								count.erase(sit);
								count.insert(temp);
							}
					sit = count.begin();
				}
				else ++sit;
		for (set<m>::iterator sit = output.begin(); sit != output.end(); ++sit)
			fout << sit->value << endl;
	}
	fin.close();
	fout.close();
}

void findPI(vector< mVec > input, vector< mVec > &PIs, set< pi > &count)
{
	for (int i = 0; i < 4; i++)
		for (mVec::iterator mit = input[i].begin(); mit != input[i].end(); ++mit)
			for (mVec::iterator cit = input[i + 1].begin(); cit != input[i + 1].end(); ++cit)
				if (mit->care || cit->care)// mit and cit both aren't don't care
				{
					int same = 4;
					string implicant = "2222";
					for (int k = 0; k < 4; k++)
						if (mit->value[k] == cit->value[k]) implicant[k] = mit->value[k];
						else same--;

					mVec::iterator fit;
					for (fit = PIs[i].begin(); fit != PIs[i].end(); ++fit)
						if (fit->value == implicant) break;

					if (same == 3 && fit == PIs[i].end())
					{
						m buffer = { implicant, true, mit->no };
						buffer.no.insert(buffer.no.end(), cit->no.begin(), cit->no.end());
						PIs[i].push_back(buffer);
						for (int j = 0; j < buffer.no.size(); j++)
							for (set< pi >::iterator sit = count.begin(); sit != count.end(); ++sit)
								if (buffer.no[j] == sit->no)
								{
									pi temp = { sit->no, implicant, sit->essential };
									count.erase(sit);
									temp.essential++;
									count.insert(temp); 
									break;
								}
					}
				}// end outer if
}

vector< mVec >::iterator ePI(set< m > &output, set< pi > &count, int no, vector< mVec > &PI)
{
	for (vector< mVec >::iterator pit = PI.begin(); pit != PI.end(); ++pit)
		for (mVec::iterator cit = pit->begin(); cit != pit->end(); ++cit)
			if (find(cit->no.begin(), cit->no.end(), no) != cit->no.end())
			{
				for (int j = 0; j < cit->no.size(); j++)
					for (set< pi >::iterator sit = count.begin(); sit != count.end(); ++sit)
						if (cit->no[j] == sit->no)
						{
							pi temp = { sit->no, sit->value, sit->essential, sit->choose };
							count.erase(sit);
							temp.choose = true;
							count.insert(temp);
							break;
						}
				output.insert(*cit);
				return pit;
			}
	return PI.end();
}
