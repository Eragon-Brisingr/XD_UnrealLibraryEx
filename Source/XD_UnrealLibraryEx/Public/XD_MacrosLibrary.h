﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct XD_UNREALLIBRARYEX_API XD_Macros_Helper
{
	//允许前向声明
	template<typename T, bool IsCppName, unsigned long long Length>
	static constexpr const char* GetTypeName(const char(&Type)[Length])
	{
		static_assert(Length > 0, "Type name must not empty !");

#define ClassForwardDeclaration "class "
#define StructForwardDeclaration "struct "

#define ClassForwardDeclaration_Length sizeof(ClassForwardDeclaration) - 1
#define StructForwardDeclaration_Length sizeof(StructForwardDeclaration) - 1

		return Length > ClassForwardDeclaration_Length && StringCompare(ClassForwardDeclaration_Length, Type, ClassForwardDeclaration) ? GetTypeNameImpl(Type, IsCppName) + ClassForwardDeclaration_Length :
			Length > StructForwardDeclaration_Length && StringCompare(StructForwardDeclaration_Length, Type, StructForwardDeclaration) ? GetTypeNameImpl(Type, IsCppName) + StructForwardDeclaration_Length :
			GetTypeNameImpl(Type, IsCppName);

#undef ClassForwardDeclaration_Length
#undef StructForwardDeclaration_Length

#undef ClassForwardDeclaration
#undef StructForwardDeclaration
	}

private:
	//由于虚幻命名规则，向后移动一个字符才是类型的名字
	static constexpr const char* GetTypeNameImpl(const char* Type, bool IsCppName) 
	{
		return IsCppName ? Type : Type + 1;
	}

	static constexpr bool StringCompare(const long long CompareLength, const char* LHS, const char* RHS)
	{
		return StringCompareImpl(CompareLength - 1, LHS, RHS);
	}

	static constexpr bool StringCompareImpl(const long long CompareIndex, const char* LHS, const char* RHS)
	{
		return CompareIndex == -1 ? true : LHS[CompareIndex] != RHS[CompareIndex] ? false : StringCompareImpl(CompareIndex - 1, LHS, RHS);
	}
};

//获取类型名，允许前向声明
#define GET_TYPE_NAME_STRING_CHECKED(Type) FString(XD_Macros_Helper::GetTypeName<Type, false>(#Type))

//获取类型名，允许前向声明
#define GET_TYPE_NAME_CHECKED(Type) FName(XD_Macros_Helper::GetTypeName<Type, false>(#Type))

//获取c++中定义时的类型名，允许前向声明
#define GET_TYPE_CPP_NAME_STRING_CHECKED(Type) FString(XD_Macros_Helper::GetTypeName<Type, true>(#Type))

//获取c++中定义时的类型名，允许前向声明
#define GET_TYPE_CPP_NAME_CHECKED(Type) FName(XD_Macros_Helper::GetTypeName<Type, true>(#Type))

//用来查看私有变量的偏移地址
#define LookMemberOffset(type, member) (size_t)&(((type*)0)->member)

//用于调用虚幻的接口
#define CallUnrealInterface(InterfaceName,FunctionName) InterfaceName##::Execute_##FunctionName
