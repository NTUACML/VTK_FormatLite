#pragma once

#ifndef MEMTOOL_H
#define MEMTOOL_H

template <typename T>
class MemTool
{
public:
	//--Array Initial interface
	static void ArrayInit(T*& arr);

	//--Array Delete interface
	static void ArrayDelete(T*& arr);

	//--Array New interface
	static void ArrayNew(T*& arr, int S);

	//--Array Distance
	static int ArrayDist(T* arrStart, T* arrEnd);

	//--Matrix Initial interface
	static void MatrixInit(T**& arr);

	//--Matrix New interface(Known Dimension)
	static void MatrixNew(T**& arr, int Row, int Col);

	//--Matrix New interface(UnKnown Col Dimension)
	static void MatrixNew(T**& arr, int Row);

	//--Matrix Delete interface
	static void MatrixDelete(T**& arr);

	//--User Define Array Initial interface
	static void UserArrayInit(T*& arr);

	//--User Define Array Delete interface
	static void UserArrayDelete(T*& arr);

	//--User Define Array New interface
	static void UserArrayNew(T*& arr, int S);

	//--Index of min element
	static int minElementId(T* const & arr, int S);

	//--Index of max element
	static int maxElementId(T* const & arr, int S);
};

template <typename T>
void MemTool<T>::ArrayInit(T*& arr){
	arr = NULL;
}

template <typename T>
void MemTool<T>::ArrayDelete(T*& arr){
	if (arr != NULL){
		delete[] arr;
		arr = NULL;
	}
}

template <typename T>
void MemTool<T>::ArrayNew(T*& arr, int S){
	if (arr != NULL){
		MemTool<T>::ArrayDelete(arr);
	}
	arr = new T[S + 1]();
	arr[0] = T(S);
}

template <typename T>
void MemTool<T>::MatrixInit(T**& arr){
	arr = NULL;
}

template <typename T>
void MemTool<T>::MatrixNew(T**& arr, int Row, int Col){
	if (arr != NULL)
		MemTool<T>::MatrixDelete(arr);
	arr = new T*[(Row + 1)]();
	MemTool<T>::ArrayNew(arr[0], 0); arr[0][0] = T(Row);
	for (int i = 1; i <= Row; ++i)
		MemTool<T>::ArrayNew(arr[i], Col);
}

template <typename T>
void MemTool<T>::MatrixNew(T**& arr, int Row){
	if (arr != NULL)MemTool<T>::MatrixDelete(arr);
	arr = new T*[(Row + 1)]();
	MemTool<T>::ArrayNew(arr[0], 0); arr[0][0] = T(Row);
	for (int i = 1; i <= Row; ++i)
		arr[i] = NULL;
}

template <typename T>
void MemTool<T>::MatrixDelete(T**& arr){
	if (arr != NULL){
		for (int i = 1; i < int(arr[0][0]); ++i)
			MemTool<T>::ArrayDelete(arr[i]);
		MemTool<T>::ArrayDelete(arr[0]);
		MemTool<T*>::ArrayDelete(arr);
	}
}

template <typename T>
void MemTool<T>::UserArrayInit(T*& arr){
	arr = NULL;
}

template <typename T>
void MemTool<T>::UserArrayDelete(T*& arr){
	if (arr != NULL){
		delete[] arr;
		arr = NULL;
	}
}

template <typename T>
void MemTool<T>::UserArrayNew(T*& arr, int S){
	if (arr != NULL){
		MemTool<T>::UserArrayDelete(arr);
	}
	arr = new T[S + 1]();
}

template <typename T>
int MemTool<T>::minElementId(T* const & arr, int S)
{
	int SmallestIndex = 1;
	T TmpMin = arr[1];
	for (int i = 1; i <= S; i++) {
		if (arr[i] <= TmpMin) {
			SmallestIndex = i;
			TmpMin = arr[i];
		}
	}
	return SmallestIndex;
}

template <typename T>
int MemTool<T>::maxElementId(T* const & arr, int S)
{
	int LargestIndex = 1;
	T TmpMax = arr[1];
	for (int i = 1; i <= S; i++) {
		if (arr[i] >= TmpMax) {
			LargestIndex = i;
			TmpMax = arr[i];
		}
	}
	return LargestIndex;
}

template <typename T>
int MemTool<T>::ArrayDist(T* arrStart, T* arrEnd)
{
	return int(arrEnd - arrStart);
}

#endif