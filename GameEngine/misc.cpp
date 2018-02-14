#include "misc.h"
#include "framework.h"
/*
template<class T>
T Clamp(T count, T floor, T ceiling) {
	if (count < floor)
		return floor;
	if (count > ceiling)
		return ceiling;
	return count;
}

template<class T>
T Clamp_Floor(T count, T floor) {
	if (count < floor)
		return floor;
	return count;
}

template<class T>
T Clamp_Ceiling(T count, T ceiling) {
	if (count > ceiling)
		return ceiling;
	return count;
}

template<class T> 
IndexedArray<T>::IndexedArray(uint maxn, uint off) :
data(nullptr),
count	(0),
maxn(maxn),
off	(off),
ord	(false) {
	if (maxn == FASTARRAY_INVALID)
		Die("IndexedArray.IndexedArray: invalid size");
	data = new T*[maxn];
}

template<class T>
IndexedArray<T>::IndexedArray(uint maxn, uint off, bool ord) :
data(nullptr),
count	(0),
maxn(maxn),
off	(off),
ord	(ord) {
	if (maxn == FASTARRAY_INVALID)
		Die("IndexedArray.IndexedArray: invalid size");
	data = new T*[maxn];
	for (int i = 0; i < maxn; i++)
		data[i] = nullptr;
}

template<class T>
IndexedArray<T>::~IndexedArray() {
	delete[] data;
}

template<class T>
T* IndexedArray<T>::Get(uint i) {
	if(i >= count)
		Die("IndexedArray.Get: invalid index");
	return data[i];
}

template<class T>
T* IndexedArray<T>::operator[](uint i) {
	if (i >= count)
		Die("IndexedArray.Get: invalid index");
	return data[i];
}

template<class T>
uint IndexedArray<T>::Size() {
	return count;
}

template<class T>
void IndexedArray<T>::Set(T* item, uint i) {
	if (i >= count)
		Die("IndexedArray.Set: invalid index");
	data[i] = item;
	Set_Index(i);
}

template<class T>
uint IndexedArray<T>::Get_Index(T* item) {
	return *(uint*)(item + off);
}

template<class T>
void IndexedArray<T>::Set_Index(uint i) {
	*(uint*)(data[i] + off) = i;
}

template<class T>
void IndexedArray<T>::Nullify_Index(T* item) {
	*(uint*)(item + off) = FASTARRAY_INVALID;
}

template<class T>
void IndexedArray<T>::Add(T* item) {
	if (count == maxn)
		Die("IndexedArray.Add: overflow");
	data[count] = item;
	Set_Index(count);
	count++;
}

template<class T>
void IndexedArray<T>::Remove(T* item) {
	uint i = Get_Index(item);
	Nullify_Index(item);
	count--;
	if (ord) {
		for (int j = i; j < count; j++) {
			data[j] = data[j + 1];
			Set_Index(j);
		}
		data[count] = nullptr;
	}
	else {
		data[i] = data[count];
		Set_Index(i);
		data[count] = nullptr;
	}
}

template<class T>
void IndexedArray<T>::Insert(T* item, uint i) {
	if (count == maxn)
		Die("IndexedArray.Insert: overflow");
	for (int j = count; j > i; j--) {
		data[j] = data[j - 1];
		Set_Index(j);
	}
	data[i] = item;
	Set_Index(i);
	count++;
}

template<class T>
void IndexedArray<T>::Swap(T* item1, T* item2) {
	uint i1 = Get_Index(item1);
	uint i2 = Get_Index(item2);
	if (i1 >= count || i2 >= count)
		Die("IndexedArray.Swap: invalid index");
	data[i1] = item2;
	data[i2] = item1;
	Set_Index(i1);
	Set_Index(i2);
}*/

string Double_To_String(double d, int precision) {
	stringstream ss;
	ss << fixed << std::setprecision(precision) << d;
	return ss.str();
}

/*
void Die(std::string error) {
	OutputDebugStringA("\count\count");
	OutputDebugStringA(error.c_str());
	OutputDebugStringA("\count");
	exit(-1);
}*/