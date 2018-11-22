#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

//树节点
class Node
{
public:
	char character;
	int weight;
	Node *leftChild;
	Node *rightChild;
	Node() :weight(0), leftChild(NULL), rightChild(NULL) {}
};

//编码项
class CodeTerm
{
public:
	char character;//字符
	int times;//次数
	string bitset;//二进制序列
	CodeTerm() :times(0) {}
	CodeTerm(char c) :times(1)
	{
		character = c;
	}
	CodeTerm(char c,int t) :times(t)
	{
		character = c;
	}
};

//树
class Tree
{
private:
	Node *root;
	int Sum;
	bool isUse;
	void DelNode(Node *node);//删除以node为根的树
	void SetBitSets(vector<CodeTerm> &C, string s,Node *n);
public:
	Tree();
	Tree(int w);
	~Tree();
	friend Tree operator+(Tree t1, Tree t2);
	int GetSum();
	bool IsUsed();
	void SetIsUse(bool i);
	void DelNode();
	void SetBitSets(vector<CodeTerm> &CodingTable);
	void SetRootChar(char c);
	void BitsetToChar(string Bri, string &Re);
};

void Tree::BitsetToChar(string Bri, string &Re)
{
	Node *now = root;
	int Index = 0;
	while (true)
	{
		if (now->leftChild == NULL && now->rightChild == NULL)
		{
			Re += now->character;
			now = root;
		}
		if (Index == Bri.size())
		{
			break;
		}
		if (Bri.at(Index) == '0' && now->leftChild != NULL)
		{
			now = now->leftChild;
			Index++;
			continue;
		}
		if (Bri.at(Index) == '1' && now->rightChild != NULL)
		{
			now = now->rightChild;
			Index++;
		}
	}
}

void Tree::SetRootChar(char c)
{
	root->character = c;
}

void Tree::SetBitSets(vector<CodeTerm> &C, string s, Node *n)
{
	if (n->leftChild == NULL && n->rightChild == NULL)
	{
		for (int i = 0;i < C.size();i++)
		{
			if (n->character == C.at(i).character)
			{
				C.at(i).bitset = s;
				return;
			}
		}
	}
	if (n->leftChild != NULL)
	{
		SetBitSets(C, s + '0', n->leftChild);
	}
	if (n->rightChild != NULL)
	{
		SetBitSets(C, s + '1', n->rightChild);
	}
}

void Tree::SetBitSets(vector<CodeTerm> &C)
{
	string s;
	if (root->leftChild != NULL)
	{
		SetBitSets(C, s + '0', root->leftChild);
	}
	if (root->rightChild != NULL)
	{
		SetBitSets(C, s + '1', root->rightChild);
	}
}

void Tree::DelNode()
{
	DelNode(root);
}

void Tree::SetIsUse(bool i)
{
	isUse = i;
}

bool Tree::IsUsed()
{
	return isUse;
}

//t1 < t2
Tree operator+(Tree t1, Tree t2)
{
	Tree tree(0);
	tree.root->leftChild = t1.root;
	tree.root->rightChild = t2.root;
	tree.root->weight = t1.root->weight + t2.root->weight;
	return tree;
}

int Tree::GetSum()
{
	Sum = root->weight;
	return Sum;
}

Tree::Tree(int w)
{
	root = new Node();
	root->weight = w;
	isUse = false;
}

Tree::Tree()
{
	root = new Node();
	root->weight = 0;
	isUse = false;
}

Tree::~Tree()
{
}

void Tree::DelNode(Node *node)
{
	if (node->leftChild != NULL)
	{
		DelNode(node->leftChild);
	}
	if (node->rightChild != NULL)
	{
		DelNode(node->rightChild);
	}
	delete node;
}


//哈夫曼
class Huffman
{
private:
	vector<CodeTerm> CodingTable;//编码表
	vector<Tree> TreeSet;//所有字符形成一棵树
	Tree HuffmanTree;
	string CompressName;
	string CompressedName;
	string DeCompressName;
	string DeCompressedName;
	void SetCodingtable();//生成编码表，压缩用
public:
	Huffman();
	~Huffman();
	void SetCompressName(string s);//设置需压缩文件名
	void SetCompressedName(string s);//设置压缩后文件名
	void SetDeCompressName(string s);//设置需解压文件名
	void SetDeCompressedName(string s);//设置解压后文件名
	void Compressing();//压缩
	void DeCompressing();//解压
	void ShowCodingTable();//显示编码表
	void PrintCodingTable();//打印编码表
};

void Huffman::PrintCodingTable()
{
	fstream fout("Test//CodingTable.txt");
	fout << "character\times\tbitsets\n";
	for (int i = 0;i < CodingTable.size();i++)
	{
		fout << CodingTable.at(i).character << '\t'
			<< CodingTable.at(i).times << '\t'
			<< CodingTable.at(i).bitset << '\n';
	}
	fout.close();
}

void Huffman::SetCompressedName(string s)
{
	CompressedName = s;
}

void Huffman::SetDeCompressedName(string s)
{
	DeCompressedName = s;
}

void Huffman::ShowCodingTable()
{
	cout << "编码表如下：" << endl;
	for (int i = 0;i < CodingTable.size();i++)
	{
		cout << CodingTable.at(i).character << " " << CodingTable.at(i).times << " " << CodingTable.at(i).bitset << endl;
	}
}

Huffman::Huffman()
{
	HuffmanTree = Tree(0);
}

Huffman::~Huffman()
{
	CodingTable.clear();
	HuffmanTree.DelNode();
}

void Huffman::SetCodingtable()
{
	ifstream fin(CompressName, ios::binary);
	int startcount, endcount;
	startcount = fin.tellg();
	fin.seekg(0, ios::end);
	endcount = fin.tellg();
	char *ch = new char[endcount - startcount + 1];
	fin.seekg(0, ios::beg);
	fin.read(ch, endcount - startcount);
	ch[endcount - startcount] = 0;
	string str;
	for (;startcount < endcount;startcount++)
	{
		str += ch[startcount];
	}
	delete[]ch;

	fin.close();
	//生成编码表
	//生成character和times
	for (int i = 0;i < str.size();i++)
	{
		int j = 0;
		for (;j < CodingTable.size();j++)
		{
			if (CodingTable.at(j).character == str.at(i))
			{
				CodingTable.at(j).times++;
				break;
			}
		}
		if (j == CodingTable.size())
		{
			CodingTable.push_back(CodeTerm(str.at(i)));
		}
	}
	//生成TreeSet
	for (int i = 0;i < CodingTable.size();i++)
	{
		TreeSet.push_back(Tree(CodingTable.at(i).times));
		TreeSet.at(i).SetRootChar(CodingTable.at(i).character);
	}
	//生成HuffmanTree
	if (TreeSet.size() == 1)
	{
		Tree temp(TreeSet.at(0).GetSum());
		temp = TreeSet.at(0) + NULL;
		TreeSet.at(0) = temp;
	}
	else
	{
		for (int i = 0;i < TreeSet.size() - 1;i++)
		{
			//找出最小的2棵树
			int minw = INT_MAX;
			int index1 = 0;
			for (int j = 0; j < TreeSet.size();j++)
			{
				if (TreeSet.at(j).IsUsed())
				{
					continue;
				}
				if (minw > TreeSet.at(j).GetSum())
				{
					minw = TreeSet.at(j).GetSum();
					index1 = j;
				}
			}
			TreeSet.at(index1).SetIsUse(true);
			minw = INT_MAX;
			int index2 = 0;
			for (int j = 0; j < TreeSet.size();j++)
			{
				if (TreeSet.at(j).IsUsed())
				{
					continue;
				}
				if (minw > TreeSet.at(j).GetSum())
				{
					minw = TreeSet.at(j).GetSum();
					index2 = j;
				}
			}
			TreeSet.at(index2).SetIsUse(true);
			if (index1 < index2)
			{
				TreeSet.at(index1) = TreeSet.at(index1) + TreeSet.at(index2);
				TreeSet.at(index1).SetIsUse(false);
			}
			else
			{
				TreeSet.at(index2) = TreeSet.at(index1) + TreeSet.at(index2);
				TreeSet.at(index2).SetIsUse(false);
			}
		}
	}
	HuffmanTree = TreeSet.at(0);
	//生成bitsets
	HuffmanTree.SetBitSets(CodingTable);
}

void Huffman::SetCompressName(string s)
{
	ifstream fin(s);
	if (!fin)
	{
		fin.close();
		cout << s << "is not exist!" << endl;
		return;
	}
	CompressName = s;
}

void Huffman::SetDeCompressName(string s)
{
	DeCompressName = s;
}

void Huffman::Compressing()
{
	SetCodingtable();
	//写入编码表
	
	ofstream fout(CompressedName,ios::binary);
	//写入编码表大小
	fout << (char)CodingTable.size();
	char c[4];
	for (int i = 0;i < CodingTable.size();i++)
	{
		fout.write(&CodingTable.at(i).character, sizeof(char));
		for (int j = 0;j < 4;j++)
		{
			c[j] = 0;
			c[j] = c[j] | (CodingTable.at(i).times >> 8*j);
		}
		fout.write(c, 4 * sizeof(char));
	}
	
	ifstream fin(CompressName, ios::binary);
	int startcount, endcount;
	startcount = fin.tellg();
	fin.seekg(0, ios::end);
	endcount = fin.tellg();
	char *ch = new char[endcount - startcount + 1];
	fin.seekg(0, ios::beg);
	fin.read(ch, endcount - startcount);
	ch[endcount - startcount] = 0;
	string str;
	for (;startcount < endcount;startcount++)
	{
		str += ch[startcount];
	}
	delete[]ch;
	fin.close();
	//写入内容
	char Bri = 0;
	int count = 0;
	for (int i = 0;i < str.size();i++)
	{
		for (int j = 0;j < CodingTable.size();j++)
		{
			if (CodingTable.at(j).character == str.at(i))
			{
				for (int k = 0;k < CodingTable.at(j).bitset.size();k++,count++)
				{
					if (CodingTable.at(j).bitset.at(k) == '1')
					{
						Bri = Bri | (unsigned)1;
					}
					if (count == 7)
					{
						fout.write(&Bri, sizeof(Bri));
						count = -1;
						Bri = 0;
						continue;
					}
					Bri <<= 1;
					
				}
				break;
			}
		}
	}
	//读完之后根据count知道有多少最后剩余的
	if (count > 0)
	{
		Bri <<= (7 - count);
	}
	fout << Bri;
	Bri = count;
	fout << Bri;
	fout.close();
}

void Huffman::DeCompressing()
{
	ifstream fin(DeCompressName,ios::binary);
	//读编码表
	int _size;
	char c1,c2[4];
	fin.read(&c1, sizeof(char));
	//如果读入0，则是256
	int size = c1,times;
	if (size == 0)
	{
		size = 256;
	}
	CodingTable.clear();
	int correct = 255;
	for (int i = 0;i < size;i++)
	{
		fin.read(&c1, sizeof(char));
		fin.read(c2, 4 * sizeof(char));
		times = 0;
		for (int i = 3;i > -1;i--)
		{
			times = times | (c2[i] & correct);
			if (i > 0)
			{
				times <<= 8;
			}
		}
		CodingTable.push_back(CodeTerm(c1,times));
	}
	//生成TreeSet
	for (int i = 0;i < CodingTable.size();i++)
	{
		TreeSet.push_back(Tree(CodingTable.at(i).times));
		TreeSet.at(i).SetRootChar(CodingTable.at(i).character);
	}
	//生成HuffmanTree
	if (TreeSet.size() == 1)
	{
		Tree temp(TreeSet.at(0).GetSum());
		temp = TreeSet.at(0) + NULL;
		TreeSet.at(0) = temp;
	}
	else
	{
		for (int i = 0;i < TreeSet.size() - 1;i++)
		{
			//找出最小的2棵树
			int minw = INT_MAX;
			int index1 = 0;
			for (int j = 0; j < TreeSet.size();j++)
			{
				if (TreeSet.at(j).IsUsed())
				{
					continue;
				}
				if (minw > TreeSet.at(j).GetSum())
				{
					minw = TreeSet.at(j).GetSum();
					index1 = j;
				}
			}
			TreeSet.at(index1).SetIsUse(true);
			minw = INT_MAX;
			int index2 = 0;
			for (int j = 0; j < TreeSet.size();j++)
			{
				if (TreeSet.at(j).IsUsed())
				{
					continue;
				}
				if (minw > TreeSet.at(j).GetSum())
				{
					minw = TreeSet.at(j).GetSum();
					index2 = j;
				}
			}
			TreeSet.at(index2).SetIsUse(true);
			if (index1 < index2)
			{
				TreeSet.at(index1) = TreeSet.at(index1) + TreeSet.at(index2);
				TreeSet.at(index1).SetIsUse(false);
			}
			else
			{
				TreeSet.at(index2) = TreeSet.at(index1) + TreeSet.at(index2);
				TreeSet.at(index2).SetIsUse(false);
			}
		}
	}
	HuffmanTree = TreeSet.at(0);
	//生成bitsets
	HuffmanTree.SetBitSets(CodingTable);

	//读内容
	int startcount, endcount;
	startcount = fin.tellg();
	fin.seekg(0, ios::end);
	endcount = fin.tellg();
	char *ch = new char[endcount - startcount];
	fin.seekg(startcount, ios::beg);
	fin.read(ch, endcount - startcount);
	string str;
	for (int i = 0;i < endcount-startcount;i++)
	{
		str += ch[i];
	}
	delete[]ch;
	fin.close();

	string Bri,Re;
	//赋值到Re
	int spare = str.at(str.size() - 1);
	for (int i = 0;i < str.size() - 1;i++)
	{
		int len = 8;
		if (i == str.size() - 2)
		{
			len = spare;
		}
		char temp = str.at(i);
		for (int j = 0;j < len;j++)
		{
			Bri += ((((temp & (char)128) >> 7) & 1) + '0');
			temp = temp << 1;
		}
	}

	HuffmanTree.BitsetToChar(Bri, Re);
	
	char *temp = new char[Re.size()];
	for (int i = 0;i < Re.size();i++)
	{
		temp[i] = Re.at(i);
	}
	ofstream fout(DeCompressedName, ios::binary);
	fout.write(temp, Re.size()*sizeof(char));
	fout.close();
}


int main()
{
	LARGE_INTEGER time_start, time_end, frequency;
	QueryPerformanceFrequency(&frequency);
	double quadpart = (double)frequency.QuadPart;

	/*
	Huffman hm;
	hm.SetCompressName("cacm.all");
	hm.SetCompressedName("Test//Bridge");
	QueryPerformanceCounter(&time_start);
	hm.Compressing();
	QueryPerformanceCounter(&time_end);
	cout << "本程序对cacm.all压缩用时" << (time_end.QuadPart - time_start.QuadPart) / quadpart << "秒" << endl << endl;
	*/
	Huffman hm2;
	hm2.SetDeCompressName("Test//Bridge");
	hm2.SetDeCompressedName("Test//DeCompressed");
	QueryPerformanceCounter(&time_start);
	hm2.DeCompressing();
	QueryPerformanceCounter(&time_end);
	cout << "本程序对cacm.all解压用时" << (time_end.QuadPart - time_start.QuadPart) / quadpart << "秒" << endl << endl;
	
	string file2 = "cacm.all";
	string file1 = "cacm.zip";
	string rar1 = "D://WinRAR//WinRAR.exe a " + file1 + " " + file2 + " ";
	string rar2 = "D://WinRAR//WinRAR.exe e " + file1;
	/*
	QueryPerformanceCounter(&time_start);
	system(rar1.c_str());
	QueryPerformanceCounter(&time_end);
	cout << "WinRAR对cacm.all压缩用时" << (time_end.QuadPart - time_start.QuadPart) / quadpart << "秒" << endl << endl;
	*/
	QueryPerformanceCounter(&time_start);
	system(rar2.c_str());
	QueryPerformanceCounter(&time_end);
	cout << "WinRAR对cacm.all解压用时" << (time_end.QuadPart - time_start.QuadPart) / quadpart << "秒" << endl << endl;
	
	system("pause");
	return 0;
}