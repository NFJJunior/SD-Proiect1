#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
//#define int long long
const int Max = 1e8 + 1;

////	Variabile globale;
int arr[Max], aux[Max], copie[Max];
vector < pair < void (*)(int, int), string > > sortari;
pair < int, int > t[8] = {{100000, 100000}, {100000, 1000000000}, {1000000, 100000}, {1000000, 1000000000}, {10000000, 100000}, {10000000, 1000000000}, {100000000, 100000}, {100000000, 1000000000}};

////	MergeSort();
void MergeSort(int left, int right)
{
	if(left < right)
	{
		int mid = left + (right - left) / 2;
		////	Impartim problema in doua subprobleme;
		MergeSort(left, mid);
		MergeSort(mid + 1, right);
		int i = left, j = mid + 1, k = left;
		////	Refacem vectorului cu ajutorul celor doua parti ordonate;
		while(i <= mid and j <= right)
		{
			if(arr[i] < arr[j])
				aux[k ++] = arr[i ++];
			else
				aux[k ++] = arr[j ++];
		}
		while(i <= mid)
			aux[k ++] = arr[i ++];
		while(j <= right)
			aux[k ++] = arr[j ++];
		for(int i=left;i<=right;i++)
			arr[i] = aux[i];
	}
}

////	TimSort();
void insertionSort(int left, int right)
{
	for(int i=left+1;i<=right;i++)
	{
		int j = i - 1, elem = arr[i];
		while(j >= left and elem < arr[j])
		{
			arr[j + 1] = arr[j];
			j --;
		}
		arr[j + 1] = elem;
	}
}
void merge(int left, int mid, int right)
{
	int i = left, j = mid + 1, k = left;
	while(i <= mid and j <= right)
	{
		if(arr[i] < arr[j])
			aux[k ++] = arr[i ++];
		else
			aux[k ++] = arr[j ++];
	}
	while(i <= mid)
		aux[k ++] = arr[i ++];
	while(j <= right)
		aux[k ++] = arr[j ++];
	for(int i=left;i<=right;i++)
		arr[i] = aux[i];
}
void TimSort(int left, int right)
{
	const int Block = 32;
	for(int i=left;i<=right;i+=Block)
		insertionSort(i, min(i+Block-1, right));
	for(int size=Block;size<=right;size*=2)
	{
		for(int i=left;i<=right;i+=2*size)
		{
			int st = i, mid = i + size - 1, dr = min(i+2*size-1, right);
			if(mid < dr)
				merge(st, mid, dr); 
		}
	}
}

////	RadixSort();
int get_max(int left, int right)
{
	int mx = arr[left];
	for(int i=left;i<=right;i++)
		mx = max(mx, arr[i]);
	return mx;
}
void countSort(int left, int right, int exp)
{
	const int base = (1 << 16);
	int poz[base] = {};
	for(int i=left;i<=right;i++)
		poz[arr[i] / exp % base] ++;
	for(int i=1;i<base;i++)
		poz[i] += poz[i - 1];
	for(int i=right;i>=left;i--)
	{
		aux[poz[arr[i] / exp % base]] = arr[i];
		poz[arr[i] / exp % base] --;
	}
	for(int i=left;i<=right;i++)
		arr[i] = aux[i];
}
void RadixSort(int left, int right)
{
	const int base = (1 << 16);
	int mx = get_max(left, right);
	for(int exp = 1 ; mx / exp > 0 ; exp *= base)
	{
		countSort(left, right, exp);
		if(exp == base)
			break;
	}
}

////	ShellSort();
void ShellSort(int left, int right)
{
	vector < int > gaps = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
	while(gaps.back() < (right - left) / 2)
	{
		int gap = gaps.back();
		gap = gap * 9 / 4 + 1;
		gaps.push_back(gap);
	}
	reverse(gaps.begin(), gaps.end());
	for(auto gap : gaps)
	{
		if(gap <= (right - left) / 2)
			for(int i=left+gap+1;i<=right;i++)
			{
				int j = i, elem = arr[i];
				while(j >= left + gap and elem < arr[j - gap])
				{
					arr[j] = arr[j - gap];
					j -= gap;
				}
				arr[j] = elem;
			}
	}
}

////	Generare de array neordonat;
void generate_array(int n, int mx)
{
	srand(time(NULL));
	for(int i=1;i<=n;i++)
		arr[i] = (rand() * rand()) % mx + 1;
}

void test_time(int n, int mx)
{
	generate_array(n, mx);
	cout<<"N = "<<n<<", Max = "<<mx<<'\n';
	for(int i=1;i<=n;i++)
		copie[i] = arr[i];
	for(auto sort : sortari)
	{
		void (*func)(int, int) = sort.first;
		auto start = high_resolution_clock::now();
		func(1, n);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast < microseconds >(stop - start);
		cout<<sort.second<<": ";
		cout<<setprecision(4)<<fixed<<duration.count()/1e6<<'\n';
		for(int i=1;i<=n;i++)
			arr[i] = copie[i];
	}
	cout<<'\n';
}

int main()
{
	sortari.push_back({& MergeSort, "MergeSort"});
	sortari.push_back({& TimSort, "TimSort"});
	sortari.push_back({& RadixSort, "RadixSort"});
	sortari.push_back({& ShellSort, "ShellSort"});
	int n, mx;
	cout << "N = "; cin >> n; cout << '\n';
	cout << "Max = "; cin >> mx; cout << '\n';
	test_time(n, mx);
	return 0;
}