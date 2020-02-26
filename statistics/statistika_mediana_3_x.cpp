#include <iostream>

void swap(int* a, int* b)
{
	if (*a != * b){
		int c = *a;
		*a = *b;
		*b = c;
	}
};

int median(int* &arr, int i0, int size)
{
	int a = arr[i0];
	int b = arr[i0 + (size / 2)];
	int c = arr[i0 + (size - 1)];
	int i1 = i0 + (size / 2);
	int i2 = i0 + (size - 1);

	if (a < b)
	{
		if (b < c) return i1;
		if (a < c) return i2;
		return i0;
	}else{
		if (a < c) return i0;
		if (b < c) return i2;
		return i1;
	}
}

//Находит k-ую порядковую статистику.
//Делит массив на две части, меньшую и большую опорного.
//Выбор опорного элемента медианой из трёх.
int sort_merge_median(int* a, int n, int k)
{
	int i0 = 0;
	int size = n;
	int i_p;
	int pivot;
	int i, j;

	while (size >= 4)
	{
		i_p = median(a, i0, size);
		swap(a + i0, a + i_p);
		pivot = a[i0];

		i = i0 + size;
		j = i0 + size - 1;
		while (j >= i0)
		{
			if (a[j] < pivot) {
				--i;
				swap(a + j, a + i);
			}
			j--;
		}

		if (size - (i - i0) > k ) {
			size = size - (i - i0);
			i0 = i;
			//return sort_merge_median(a + i, size - i, k);
		}
		else if (size - (i - i0) == k) { return pivot; }
		else if (size - (i - i0) < k) {
			k = k - (size - (i - i0) + 1) ; 
			size = (i - i0) - 1;
			++i0;
			//sort_merge_median(a + 1, i - 1, k - (size - i + 1)); }
		}
	}

	if (size == 2)
	{
		if (a[i0] < a[i0+1]) return a[i0+k];
		else return a[i0+1 - k];
	}
	if (size == 3)
	{
		if (a[i0] > a[i0+1]) swap(a+ i0, a + i0 + 1);
		if (a[i0+2] < a[i0+1]) swap(a + i0 + 1, a + i0 + 2);
		if (a[i0] > a[i0+1]) swap(a+i0, a + i0 + 1);
		return a[i0+k];
	}
	return a[i0];
}

int call_partition()
{
	int n, k;
	std::cin >> n >> k;

	int* a = new int[n];
	for (int j = 0; j < n; j++)
		std::cin >> a[j];

	int result = sort_merge_median(a, n, k);
	std::cout << result;
	delete[]a;
	return 0;
}