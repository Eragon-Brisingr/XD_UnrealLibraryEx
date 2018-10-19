// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
 /**
 *  由于模板必须暴露实现，单独一个头文件
 */

 //指针数组的转型方法，注意类型安全
template<typename ToType, typename FromType>
static const TArray<ToType>& ArrayCast(const TArray<FromType>& Array)
{
	return *static_cast<const TArray<ToType>*>((void*)(&Array));
}
template<typename ToType, typename FromType>
static TArray<ToType>& ArrayCast(TArray<FromType>& Array)
{
	return *static_cast<TArray<ToType>*>((void*)(&Array));
}

/**
*  访问私有变量、私有函数
*/
//使用方法
//struct A {
//private:
//	void f() const {
//		std::cout << "proof!" << std::endl;
//	}
//
//	int a = 5;
//};
//
//struct Af { typedef void(A::*type)() const; };
//template struct rob<Af, &A::f>;
//
//struct A_a { typedef int A::*type; };
//template struct rob<A_a, &A::a>;
//
//int main() {
//	A a;
//	(a.*result<Af>::ptr)();
//	cout << a.*result<A_a>().ptr << endl;
//}
template<typename Tag>
struct result {
	/* export it ... */
	typedef typename Tag::type type;
	static type ptr;
};

template<typename Tag>
typename result<Tag>::type result<Tag>::ptr;

template<typename Tag, typename Tag::type p>
struct rob : result<Tag> {
	/* fill it ... */
	struct filler {
		filler() { result<Tag>::ptr = p; }
	};
	static filler filler_obj;
};

template<typename Tag, typename Tag::type p>
typename rob<Tag, p>::filler rob<Tag, p>::filler_obj;

template<typename T>
static T& GetObjectMemberByOffset(const void* Object, int32 Offset)
{
	return reinterpret_cast<T&>(*((char*)Object + Offset));
}
