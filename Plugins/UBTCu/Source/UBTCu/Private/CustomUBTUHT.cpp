#include "CustomUBTUHT.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

class FUnrealTypeCollection :public IUTC
{
	bool GetCodeTypeByFunc(const FString &RowStrong, FFunctionAnalysis &FunctionAnalysis)
	{
		FString R, L;
		//UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
		//L =  "Describe", Group = "SimpleCodeLibrary"))
		//R = UFUNCTION(Meta = (
		RowStrong.Split(TEXT("CodeType"), &R, &L);

		TArray<FString> ElementStr;
		//  "Describe"
		// Group = "SimpleCodeLibrary"))
		L.ParseIntoArray(ElementStr, TEXT(","));

		if (ElementStr[0].Contains("Event"))
		{
			FunctionAnalysis.CodeType = "Event";

			return true;
		}
		else if (ElementStr[0].Contains("Describe"))
		{
			FunctionAnalysis.CodeType = "Describe";

			return true;
		}

		return false;
	}
	
	FParamElement CollectionVariableType(FString Variable, ECollectionParmType CollectionParmType = ECollectionParmType::Type_Parm)
	{
		FParamElement ParamElement;
		ParamElement.Name = "ReturnValue";

		if (Variable.Contains(TEXT("*")))
		{
			Variable.RemoveFromEnd("*");
			ParamElement.bPointer = true;
		}

		if (Variable.Contains(TEXT("&")))
		{
			Variable.RemoveFromEnd("&");
			ParamElement.bReference = true;
		}

		if (Variable.Contains(TEXT("const")))
		{
			Variable.TrimStartAndEndInline();

			Variable.RemoveFromStart("const");
			ParamElement.bConst = true;
		}

		// void 
		Variable.TrimStartAndEndInline();

		ParamElement.Type = *Variable;

		return ParamElement;
	}

	bool GetCodeTypeByProp(const FString &RowStrong, FVariableAnalysis &VariableAnalysis)
	{
		//RowStrong = UPROPERTY(Meta = (CodeType = "Resources", Group = "SimpleCodeLibrary"))
		FString R, L;
		RowStrong.Split(TEXT("CodeType"), &R, &L);
		//R =  UPROPERTY(Meta = (
		//L =  = "Resources", Group = "SimpleCodeLibrary"))

		TArray<FString> ElementStr;
		L.ParseIntoArray(ElementStr, TEXT(","));
		// "Resources" 0 
		// Group = "SimpleCodeLibrary")) 1

		if (ElementStr[0].Contains("Resources"))
		{
			VariableAnalysis.CodeType = "Resources";
			return true;
		}

		return false;
	}
public:

	//收集指定场景内符合标准的所以代码
	virtual bool Collection(const FString &Paths, FClassAnalysis &ClassAnalysis) override
	{
		TArray<FString> StringArray;
		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*Paths))
		{
			return false;
		}
		FFileHelper::LoadANSITextFileToStrings(*(Paths), NULL, StringArray);

		//开始进行解析
		for (int32 i = 0; i < StringArray.Num(); i++)
		{
			FString Row = StringArray[i];
			//获取反射数据行数
			if (Row.Contains("GENERATED_BT_BODY"))
			{
				ClassAnalysis.CodeLine = i + 1;
			}

			//获取类名和继承名
			if ((Row.Contains("\tclass") || Row.Contains("class")) &&
				Row.Contains(":") &&
				(Row.Contains("protected") || Row.Contains("private") || Row.Contains("public")))
			{
				Row.RemoveFromStart("class ");
				Row.RemoveFromStart("\tclass ");

				if (Row.Contains("_API"))
				{
					FString R, L;

					//修剪前面的空格
					//L = UObject_C :public UObject
					Row.TrimStartInline();
					Row.Split(TEXT(" "), &R, &L);
					Row = L;
				}

				////L = UObject_C :public UObject
				// UObject_C 
				//public UObject ,public Interxx
				TArray<FString> ElementStr;
				Row.ParseIntoArray(ElementStr, TEXT(":"));

				ElementStr[0].TrimStartAndEndInline();
				ClassAnalysis.ClassName = *ElementStr[0];

				//考虑到多继承问题
				//public UObject ,public Interxx
				if (ElementStr.IsValidIndex(1))
				{
					//public UObject 
					//public Interxx
					TArray<FString> InheritElement;
					ElementStr[1].ParseIntoArray(InheritElement, TEXT(","));
					for (auto &Tmp : InheritElement)
					{
						Tmp.TrimStartInline();
						Tmp.RemoveFromStart("public");
						Tmp.RemoveFromStart("protected");
						Tmp.RemoveFromStart("private");

						Tmp.TrimStartInline();
						ClassAnalysis.InheritName.Add(*Tmp);
					}
				}
			}
			//获取标记的成员函数
			if (Row.Contains("UFUNCTION"))
			{
				//UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
				if (Row.Contains("CodeType"))
				{
					FFunctionAnalysis FunctionAnalysis;
					if (GetCodeTypeByFunc(Row, FunctionAnalysis))
					{
						Row = StringArray[i + 1];
						//static void Hello1(UObject *Context, int32 &A,float b,bool C);

						if (Row.Contains("\tstatic") || Row.Contains("static "))
						{
							FString R, L;
							FunctionAnalysis.bStatic = true;
							Row.Split(TEXT(" "), &R, &L);
							//R = static
							//L = void Hello1(UObject *Context, int32 &A,float b,bool C);

							Row = L;
						}
						else if (Row.Contains("virtual "))
						{
							FunctionAnalysis.bVirtual = true;
							FString R, L;
							Row.Split(TEXT(" "), &R, &L);

							Row = L;
						}

						//确定我们函数的返回类型
						FString Tmp;
						{
							//Row =  void Hello1(UObject *Context, int32 &A,float b,bool C);
							FString R;
							Row.TrimStartInline();
							Row.Split(TEXT(" "), &R, &Tmp);
							//R = void 
							//Tmp = Hello1(UObject *Context, int32 &A,float b,bool C);  {}
							FunctionAnalysis.Return = CollectionVariableType(R, ECollectionParmType::Type_Return);

							{
								Tmp.RemoveFromEnd("{}");
								Tmp.TrimEndInline();
								Tmp.RemoveFromEnd(";");
								Tmp.RemoveFromEnd(")");
								//Tmp = Hello1(UObject *Context, int32 &A,float b,bool C
							}

							FString RStr, LStr;
							Tmp.Split(TEXT("("), &RStr, &LStr);
							//RStr =  Hello1
							//LStr = UObject *Context, int32 &A,float b,bool C

							//函数名
							FunctionAnalysis.FunctionName = *RStr;

							//解析参数和参数名
							TArray<FString> ElementStr;
							LStr.ParseIntoArray(ElementStr, TEXT(","));
							//UObject *Context
							//int32 &A
							//float b
							//bool C

							//收集变量
							for (FString &Ele : ElementStr)
							{
								//移除前后空格
								Ele.TrimStartAndEndInline();

								FParamElement ParamElement;
								FString InR, L;
								if (Ele.Contains("*"))
								{
									ParamElement.bPointer = true;
									//UObject *Context
									Ele.Split(TEXT("*"), &InR, &L);
									//R = UObject
									//L = Context
								}
								else if (Ele.Contains("&"))
								{
									ParamElement.bReference = true;
									Ele.Split(TEXT("&"), &InR, &L);
								}
								else
								{
									Ele.Split(TEXT(" "), &InR, &L);
								}

								if (InR.Contains("const"))
								{
									ParamElement.bConst = true;
									InR.RemoveFromStart("const");
								}

								InR.TrimStartAndEndInline();
								L.TrimStartAndEndInline();
								ParamElement.Type = *InR;

								ParamElement.Name = *L;
								FunctionAnalysis.ParamArray.Add(ParamElement);
							}

							ClassAnalysis.Function.Add(FunctionAnalysis);
						}
					}
				}
			}
			////获取标记的成员变量
			if (Row.Contains("UPROPERTY"))
			{
				if (Row.Contains("CodeType"))
				{
					FVariableAnalysis VariableAnalysis;
					if (GetCodeTypeByProp(Row, VariableAnalysis))
					{
						FString RStr, LStr;
						Row = StringArray[i + 1];
						//Row = \tTSubclassOf<UStaticMesh> Mesh;
						Row.RemoveFromStart(TEXT("\t"));
						Row.RemoveFromEnd(TEXT(";"));
						//Row = TSubclassOf<UStaticMesh> Mesh

						if (Row.Contains(TEXT("*")))
						{
							VariableAnalysis.bPointer = true;
							Row.Split(TEXT("*"), &RStr, &LStr);
						}
						if (Row.Contains(TEXT("&")))
						{
							VariableAnalysis.bReference = true;
							Row.Split(TEXT("&"), &RStr, &LStr);
						}
						if (Row.Contains(TEXT(" ")))
						{
							Row.Split(TEXT(" "), &RStr, &LStr);
						}

						if (RStr.Contains("const"))
						{
							VariableAnalysis.bConst = true;
							RStr.RemoveFromStart("const");
						}

						Row.TrimStartAndEndInline();

						VariableAnalysis.Type = *Row;
						VariableAnalysis.Name = *LStr;

						ClassAnalysis.Variable.Add(VariableAnalysis);
					}
				}
			}
		}

		return true;
	}
};

class FUnrealTypeBuilder :public IUTB
{
	void Generate_h_Code(TArray<FString> &AnalysisRaw, const FClassAnalysis &ClassAnalysis, TArray<FString> &StaticRegistration)
	{
		//头定义
		{
			AnalysisRaw.Add("#pragma once");
			AnalysisRaw.Add("");
			AnalysisRaw.Add("#include \"ScriptMacro.h\"");
			AnalysisRaw.Add("#include \"BTScript.h\"");
			AnalysisRaw.Add("");
		}

		//Z_BT_USimpleCodeLibrary
		FString MClassName = TEXT(" Z_BT_") + ClassAnalysis.ClassName.ToString();

		/*#define  Z_BT_USimpleCodeLibrary \*/
		AnalysisRaw.Add(FString("#define ") + MClassName + ((ClassAnalysis.Function.Num() > 0) ? TEXT(" \\") : TEXT("")));
	
		//类名
		FString ClearClassName = ClassAnalysis.ClassName.ToString();
		{
			ClearClassName.TrimStartAndEndInline();
			ClearClassName.RemoveFromStart(TEXT("U"));
			//ClearClassName = SimpleCodeLibrary
		}

		if (ClassAnalysis.Function.Num() > 0)
		{
			for (const FFunctionAnalysis &Function : ClassAnalysis.Function)
			{
				if (Function.CodeType == "Describe")
				{
					//VMC_Hello1
					FString VMString = (TEXT("VMC_") + Function.FunctionName.ToString());
					/*BT_VM(VMC_Hello1) \*/
					AnalysisRaw.Add(TEXT("BT_VM(") + VMString + TEXT(") \\"));
					/*{ \*/
					AnalysisRaw.Add(TEXT("{ \\"));

					FString VariableAdd;
					if (Function.ParamArray.Num() > 0)
					{
						for (const FParamElement &Variable : Function.ParamArray)
						{
							FString StackString = TEXT(" Stack.GetParmAddr(); \\");

							//Z_Context_Name
							FString VariableName = TEXT(" Z_") + Variable.Name.ToString() + TEXT("_Name");

							bool bUObject = false;
							{
								//UObject
								FString VarString = Variable.Type.ToString();
								VarString.TrimStartAndEndInline();
								FString U_String = TEXT("U");
								FString A_String = TEXT("A");
								int32 U_Index = 0;
								bUObject = (VarString.FindChar(**U_String, U_Index) || VarString.FindChar(**A_String, U_Index));
							}

							//添加获取具体值的语句
							/*	UObject* Z_Context_Name = (UObject*) Stack.GetParmAddr(); \*/
							AnalysisRaw.Add(FString(TEXT("\t")) +
								Variable.Type.ToString() +
								((Variable.bPointer || Variable.bReference) ? TEXT("*") : TEXT("")) +
								VariableName +
								TEXT(" = ") +
								FString((Variable.bPointer || Variable.bReference) ? (bUObject ? TEXT("*") : TEXT("")) : TEXT("*")) +
								TEXT("(") +
								Variable.Type.ToString() +
								((bUObject ? TEXT("**)") : TEXT("*)"))) +
								StackString);

							//取得变量的名字
							// , Z_Context_Name, Z_A_Name, Z_b_Name, Z_C_Name  *&
							VariableAdd += (TEXT(",") + FString((Variable.bReference && Variable.bPointer) ? "" : Variable.bReference ? "*" : "") + VariableName);
						}
						VariableAdd.RemoveFromStart(TEXT(","));
					}

					//赋值
					if (Function.bStatic)
					{
						if (Function.Return.Type == "void")
						{
							/*USimpleCodeLibrary::Hello1( Z_Context_Name, Z_A_Name, Z_b_Name, Z_C_Name); \*/
							AnalysisRaw.Add(FString(TEXT("\t")) + ClassAnalysis.ClassName.ToString() +
								TEXT("::") +
								Function.FunctionName.ToString() +
								TEXT("(") +
								VariableAdd +
								TEXT("); \\"));
						}
						else
						{
							/**(FString*)RefData = USimpleCodeLibrary::Hello3( Z_Context_Name); \*/
							AnalysisRaw.Add(FString(TEXT("\t")) + (Function.Return.bPointer ? TEXT("") : TEXT("*")) +
								FString(TEXT("(") +
									Function.Return.Type.ToString() +
									TEXT("*)")) +
								TEXT("RefData = ") +
								ClassAnalysis.ClassName.ToString() +
								TEXT("::") +
								Function.FunctionName.ToString() +
								TEXT("(") +
								VariableAdd +
								TEXT("); \\"));
						}
					}

					AnalysisRaw.Add(TEXT("} \\"));

					/*	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeLibrary"),TEXT("Hello1"), USimpleCodeLibrary::VMC_Hello1));*/
					StaticRegistration.Add(FString(TEXT("\t")) + TEXT("FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT(\"") +
						ClearClassName + TEXT("\"),TEXT(\"") + Function.FunctionName.ToString() + TEXT("\"), ") +
						ClassAnalysis.ClassName.ToString() + TEXT("::") + VMString + TEXT("));"));
				}
			}
		}

		if (AnalysisRaw.IsValidIndex(AnalysisRaw.Num() - 1))
		{
			AnalysisRaw[AnalysisRaw.Num() - 1].RemoveFromEnd(TEXT("\\"));
		}

		AnalysisRaw.Add(TEXT(""));

		/*#define USimpleCodeLibrary_12_GENERATED_BODY_BT \*/
		AnalysisRaw.Add(TEXT("#define ") + ClassAnalysis.ClassName.ToString() + TEXT("_") + FString::FromInt(ClassAnalysis.CodeLine) + TEXT("_GENERATED_BODY_BT") + TEXT(" \\"));
		//Z_BT_USimpleCodeLibrary
		AnalysisRaw.Add(MClassName);

		AnalysisRaw.Add(TEXT(""));

		//#define CURRENT_FILE_ID_BT USimpleCodeLibrary
		AnalysisRaw.Add(TEXT("#define ") + FString("CURRENT_FILE_ID_BT ") + ClassAnalysis.ClassName.ToString());
		//#define NewLine 12
		AnalysisRaw.Add(TEXT("#define ") + FString("NewLine ") + FString::FromInt(ClassAnalysis.CodeLine));
	}

	void Generate_cpp_Code(TArray<FString> &AnalysisRaw, const FClassAnalysis &ClassAnalysis, TArray<FString> &StaticRegistration)
	{
		//头定义
		{
			AnalysisRaw.Add("// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.");
			AnalysisRaw.Add("/*===========================================================================");
			AnalysisRaw.Add("	Generated code exported from BT UBT.");
			AnalysisRaw.Add("	DO NOT modify this manually! Edit the corresponding .h files instead!");
			AnalysisRaw.Add("===========================================================================*/");
			AnalysisRaw.Add("");
			AnalysisRaw.Add("#include \"SimpleCodeLibrary.h\"");
			AnalysisRaw.Add("#include \"FunctionManage.h\"");
			AnalysisRaw.Add("#include \"UObject/GeneratedCppIncludes.h\"");
			AnalysisRaw.Add("");
			AnalysisRaw.Add("#ifdef _MSC_VER");
			AnalysisRaw.Add("#pragma warning (push)");
			AnalysisRaw.Add("#pragma warning (disable : 4883)");
			AnalysisRaw.Add("#endif");
			AnalysisRaw.Add("PRAGMA_DISABLE_DEPRECATION_WARNINGS");
		}

		if (ClassAnalysis.Function.Num() > 0)
		{
			for (const FFunctionAnalysis &Function : ClassAnalysis.Function)
			{
				if (Function.CodeType == "Event")
				{
					//Name_EventEntryA
					FString FunctionName = TEXT("Name_") +
						Function.FunctionName.ToString();

					//为后面声明做准备
					TArray<FString> ParamStr;
					auto GetParmString = [&Function](TArray<FString> &ParamArray)->FString
					{
						if (Function.ParamArray.Num() == 0)
						{
							return "";
						}
						else
						{
							FString ParamString;
							for (auto &Param : Function.ParamArray)
							{
								//int32 A 
								FString NewParam = (Param.bConst ? TEXT("const") : TEXT("")) +
									Param.Type.ToString() +
									(Param.bPointer ? TEXT("* ") : (Param.bReference ? TEXT("& ") : TEXT(" "))) +
									Param.Name.ToString();

								//int32 A,int32 B.int32 c
								ParamString += TEXT(",") + NewParam;

								ParamArray.Add(NewParam);
							}
							ParamString.RemoveFromStart(TEXT(","));

							return ParamString;
						}
					};

					//static FName Name_EventEntryA = FName(TEXT("EventEntryA")); 
					AnalysisRaw.Add(
						TEXT("static FName ") +
						FunctionName +
						TEXT(" = FName(TEXT(\"") +
						Function.FunctionName.ToString() +
						TEXT("\")); "));

					//添加结构实现
					//void USimpleCodeLibrary::EventEntryB(int32 A)
					AnalysisRaw.Add(Function.Return.Type.ToString() +
						(Function.Return.bPointer ? TEXT("*") : (Function.Return.bReference ? TEXT("&") : TEXT(" "))) +
						ClassAnalysis.ClassName.ToString() + TEXT("::") + Function.FunctionName.ToString() +
						TEXT("(") + GetParmString(ParamStr) + TEXT(")"));

					AnalysisRaw.Add(TEXT("{"));

					//添加类结构
					//FParm_EventEntryB
					if (Function.ParamArray.Num() > 0)
					{
						//TSharedPtr<FFunctionParameter> FunctionParameter = nullptr;
						AnalysisRaw.Add(FString("\tTSharedPtr<FFunctionParameter> FunctionParameter = nullptr;"));
						
						//赋值
						//	AddParaAddr(StaticMeshComponent, FunctionParameter);
						for (auto &Param : Function.ParamArray)
						{
							AnalysisRaw.Add(FString::Printf(TEXT("\tAddParaAddr(%s, FunctionParameter);"), Param.bPointer ? *Param.Name.ToString() : *(TEXT("&") + Param.Name.ToString())));
						}
					}
						
					//	ExecutionScript(FindScriptFuntion(Name_EventEntryB),&Parm_EventEntryB);
					AnalysisRaw.Add(FString(TEXT("\t")) + TEXT("ExecutionScript(FindScriptFuntion(") +
						FunctionName + TEXT("),") + (Function.ParamArray.Num() == 0 ? TEXT("NULL") : (TEXT("&") + FString("FunctionParameter"))) + TEXT(");"));

					AnalysisRaw.Add(TEXT("} "));
				}
			}
		}

		AnalysisRaw.Add(TEXT(""));
		AnalysisRaw.Add(TEXT("/* As an executable function pointer for global registration"));
		AnalysisRaw.Add(TEXT("we skip the UE4 UFund local function registration because there is no bytecode in it.*/"));
	
		//Register_USimpleCodeLibrary()
		FString Register_Func = TEXT("Register_") + ClassAnalysis.ClassName.ToString() + TEXT("()");
		//uint8 Register_USimpleCodeLibrary()
		AnalysisRaw.Add(TEXT("uint8 ") + Register_Func);
		AnalysisRaw.Add(TEXT("{"));
		{
			AnalysisRaw.Append(StaticRegistration);
			AnalysisRaw.Add(TEXT(""));
			AnalysisRaw.Add(TEXT("\treturn 0;"));
		}
		AnalysisRaw.Add(TEXT("}"));

		//激活静态收集
		//static uint8 USimpleCodeLibrary_Index = Register_USimpleCodeLibrary();
		AnalysisRaw.Add(TEXT("static uint8 ") + ClassAnalysis.ClassName.ToString() + TEXT("_Index = ") + Register_Func + TEXT(";"));
		AnalysisRaw.Add("");

		{
			AnalysisRaw.Add("PRAGMA_ENABLE_DEPRECATION_WARNINGS");
			AnalysisRaw.Add("#ifdef _MSC_VER");
			AnalysisRaw.Add("#pragma warning (pop)");
			AnalysisRaw.Add("#endif");
		}
	}

public:
	virtual bool Builder(const FClassAnalysis &ClassAnalysis)override
	{
		TArray<FString> AnalysisRaw_h;
		TArray<FString> AnalysisRaw_cpp;
		TArray<FString> StaticRegistration;

		//生成反射数据
		Generate_h_Code(AnalysisRaw_h, ClassAnalysis, StaticRegistration);
		Generate_cpp_Code(AnalysisRaw_cpp, ClassAnalysis, StaticRegistration);

		//Just Test
		FFileHelper::SaveStringArrayToFile(AnalysisRaw_h, TEXT("C:/Users/Smartuil/Desktop/SomeTest/Plugins/BlueprintTool/Source/BlueprintToolCode/Public/TTTSimpleCodeLibrary.BT.h"));
		FFileHelper::SaveStringArrayToFile(AnalysisRaw_cpp, TEXT("C:/Users/Smartuil/Desktop/SomeTest/Plugins/BlueprintTool/Source/BlueprintToolCode/Public/TTTSimpleCodeLibrary.BTG.cpp"));
	
		return true;
	}
};

void FConstructionManagement::Builder()
{
	FString TestPaths = "C:/Users/Smartuil/Desktop/SomeTest/Plugins/BlueprintTool/Source/BlueprintToolCode/Public/SimpleCodeLibrary.h";

	//收集我们的类信息
	FUnrealTypeCollection UnrealTypeCollection;
	FClassAnalysis ClassAnalysis;
	UnrealTypeCollection.Collection(TestPaths, ClassAnalysis);

	//构建我们的反射数据代码
	FUnrealTypeBuilder UnrealTypeBuilder;
	UnrealTypeBuilder.Builder(ClassAnalysis);
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif