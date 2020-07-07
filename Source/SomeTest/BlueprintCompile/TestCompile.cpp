#define ISUSE 0

#if ISUSE

//Blueprinteditor.cpp 3362行
void FBlueprintEditor::Compile()
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_FUNC()

	UBlueprint* BlueprintObj = GetBlueprintObj();
	if (BlueprintObj)
	{
		FMessageLog BlueprintLog("BlueprintLog");

		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("BlueprintName"), FText::FromString(BlueprintObj->GetName()));
		BlueprintLog.NewPage(FText::Format(LOCTEXT("CompilationPageLabel", "Compile {BlueprintName}"), Arguments));

		FCompilerResultsLog LogResults;
		LogResults.SetSourcePath(BlueprintObj->GetPathName());
		LogResults.BeginEvent(TEXT("Compile"));
		LogResults.bLogDetailedResults = GetDefault<UBlueprintEditorSettings>()->bShowDetailedCompileResults;
		LogResults.EventDisplayThresholdMs = GetDefault<UBlueprintEditorSettings>()->CompileEventDisplayThresholdMs;
		EBlueprintCompileOptions CompileOptions = EBlueprintCompileOptions::None;
		if (bSaveIntermediateBuildProducts)
		{
			CompileOptions |= EBlueprintCompileOptions::SaveIntermediateProducts;
		}
		FKismetEditorUtilities::CompileBlueprint(BlueprintObj, CompileOptions, &LogResults);

		LogResults.EndEvent();

		const bool bForceMessageDisplay = ((LogResults.NumWarnings > 0) || (LogResults.NumErrors > 0)) && !BlueprintObj->bIsRegeneratingOnLoad;
		DumpMessagesToCompilerLog(LogResults.Messages, bForceMessageDisplay);

		UBlueprintEditorSettings const* BpEditorSettings = GetDefault<UBlueprintEditorSettings>();
		if ((LogResults.NumErrors > 0) && BpEditorSettings->bJumpToNodeErrors)
		{
			if (UEdGraphNode* NodeWithError = BlueprintEditorImpl::FindNodeWithError(LogResults))
			{
				JumpToNode(NodeWithError, /*bRequestRename =*/false);
			}
		}

		if (BlueprintObj->UpgradeNotesLog.IsValid())
		{
			CompilerResultsListing->AddMessages(BlueprintObj->UpgradeNotesLog->Messages);
		}

		AppendExtraCompilerResults(CompilerResultsListing);

		// send record when player clicks compile and send the result
		// this will make sure how the users activity is
		AnalyticsTrackCompileEvent(BlueprintObj, LogResults.NumErrors, LogResults.NumWarnings);
	}
}

//Kismet.cpp 750行
void FKismetEditorUtilities::CompileBlueprint(UBlueprint* BlueprintObj, EBlueprintCompileOptions CompileFlags, FCompilerResultsLog* pResults)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_FUNC()

	FBlueprintCompilationManager::CompileSynchronously(FBPCompileRequest(BlueprintObj, CompileFlags, pResults));
}

//BlueprintCompilationManager.cpp 2932行
void FBlueprintCompilationManager::CompileSynchronously(const FBPCompileRequest& Request)
{
	if (BPCMImpl)
	{
		BPCMImpl->CompileSynchronouslyImpl(Request);
	}
}

//BlueprintCompilationManager.cpp 221行
void FBlueprintCompilationManagerImpl::CompileSynchronouslyImpl(const FBPCompileRequestInternal& Request)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_FUNC()

	Request.UserData.BPToCompile->bQueuedForCompilation = true;

	const bool bIsRegeneratingOnLoad = (Request.UserData.CompileOptions & EBlueprintCompileOptions::IsRegeneratingOnLoad) != EBlueprintCompileOptions::None;
	const bool bRegenerateSkeletonOnly = (Request.UserData.CompileOptions & EBlueprintCompileOptions::RegenerateSkeletonOnly) != EBlueprintCompileOptions::None;
	const bool bSkipGarbageCollection = (Request.UserData.CompileOptions & EBlueprintCompileOptions::SkipGarbageCollection) != EBlueprintCompileOptions::None
		|| bRegenerateSkeletonOnly;
	const bool bBatchCompile = (Request.UserData.CompileOptions & EBlueprintCompileOptions::BatchCompile) != EBlueprintCompileOptions::None;
	const bool bSkipReinstancing = (Request.UserData.CompileOptions & EBlueprintCompileOptions::SkipReinstancing) != EBlueprintCompileOptions::None;
	const bool bSkipSaving = (Request.UserData.CompileOptions & EBlueprintCompileOptions::SkipSave) != EBlueprintCompileOptions::None;

	ensure(!bIsRegeneratingOnLoad); // unexpected code path, compile on load handled with different function call
	ensure(!bSkipReinstancing); // This is an internal option, should not go through CompileSynchronouslyImpl
	ensure(QueuedRequests.Num() == 0);

	// Wipe the PreCompile log, any generated messages are now irrelevant
	Request.UserData.BPToCompile->PreCompileLog.Reset();

	// Reset the flag, so if the user tries to use PIE it will warn them if the BP did not compile
	Request.UserData.BPToCompile->bDisplayCompilePIEWarning = true;

	// Do not want to run this code without the editor present nor when running commandlets.
	// We do not want to regenerate a search Guid during loads, nothing has changed in the Blueprint
	// and it is cached elsewhere.
	// We would like to regenerated it when a skeleton changes, but it is too expensive:
	if (GEditor && GIsEditor && !bIsRegeneratingOnLoad && !bRegenerateSkeletonOnly)
	{
		FFindInBlueprintSearchManager::Get().AddOrUpdateBlueprintSearchMetadata(Request.UserData.BPToCompile);
	}

	QueuedRequests.Add(Request);

	// We suppress normal compilation broadcasts because the old code path 
	// did this after GC and we want to match the old behavior:
	const bool bSuppressBroadcastCompiled = true;
	TArray<UBlueprint*> CompiledBlueprints;
	TArray<UBlueprint*> SkeletonCompiledBlueprints;
	FlushCompilationQueueImpl(bSuppressBroadcastCompiled, &CompiledBlueprints, &SkeletonCompiledBlueprints, nullptr);
	FlushReinstancingQueueImpl();

	if (Request.UserData.ClientResultsLog && Request.UserData.ClientResultsLog->bLogDetailedResults)
	{
		Request.UserData.ClientResultsLog->Note(
			*FText::Format(
				LOCTEXT("TotalCompiledBPs", "Compiled {0} total blueprints"),
				CompiledBlueprints.Num()
			).ToString()
		);
	}

	if (FBlueprintEditorUtils::IsLevelScriptBlueprint(Request.UserData.BPToCompile) && !bRegenerateSkeletonOnly)
	{
		// When the Blueprint is recompiled, then update the bound events for level scripting
		ULevelScriptBlueprint* LevelScriptBP = CastChecked<ULevelScriptBlueprint>(Request.UserData.BPToCompile);

		// ULevel::OnLevelScriptBlueprintChanged needs to be run after the CDO has
		// been updated as it respawns the actor:
		if (ULevel* BPLevel = LevelScriptBP->GetLevel())
		{
			// Newly created levels don't need this notification:
			if (BPLevel->GetLevelScriptBlueprint(true))
			{
				BPLevel->OnLevelScriptBlueprintChanged(LevelScriptBP);
			}
		}
	}

	if (GEditor && !bRegenerateSkeletonOnly)
	{
		DECLARE_SCOPE_HIERARCHICAL_COUNTER(BroadcastBlueprintReinstanced)

		// Make sure clients know they're being reinstanced as part of blueprint compilation. After this point
		// compilation is completely done:
		TGuardValue<bool> GuardTemplateNameFlag(GCompilingBlueprint, true);
		GEditor->BroadcastBlueprintReinstanced();
	}

	ensure(Request.UserData.BPToCompile->bQueuedForCompilation == false);

	if (!bSkipGarbageCollection)
	{
		DECLARE_SCOPE_HIERARCHICAL_COUNTER(CollectGarbage)

		TGuardValue<bool> GuardTemplateNameFlag(GIsGCingAfterBlueprintCompile, true);
		CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
	}

	if (!bRegenerateSkeletonOnly)
	{
		DECLARE_SCOPE_HIERARCHICAL_COUNTER(BroadcastChanged)

		for (UBlueprint* BP : SkeletonCompiledBlueprints)
		{
			BP->BroadcastChanged();
		}
	}
	else
	{
		ensure(SkeletonCompiledBlueprints.Num() == 1);
	}

	if (!bBatchCompile && !bRegenerateSkeletonOnly)
	{
		DECLARE_SCOPE_HIERARCHICAL_COUNTER(BroadCastCompiled)

		for (UBlueprint* BP : SkeletonCompiledBlueprints)
		{
			BP->BroadcastCompiled();
		}

		if (GEditor)
		{
			GEditor->BroadcastBlueprintCompiled();
		}
	}

	if (CompiledBlueprintsToSave.Num() > 0 && !bRegenerateSkeletonOnly)
	{
		if (!bSkipSaving)
		{
			TArray<UPackage*> PackagesToSave;
			for (UBlueprint* BP : CompiledBlueprintsToSave)
			{
				PackagesToSave.Add(BP->GetOutermost());
			}

			FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, /*bCheckDirty =*/true, /*bPromptToSave =*/false);
		}
		CompiledBlueprintsToSave.Empty();
	}

	// We've done our GC, so release old CDO references
	OldCDOs.Empty();
}

//BlueprintCompilationManager.cpp 495行
void FBlueprintCompilationManagerImpl::FlushCompilationQueueImpl(bool bSuppressBroadcastCompiled, TArray<UBlueprint*>* BlueprintsCompiled, TArray<UBlueprint*>* BlueprintsCompiledOrSkeletonCompiled, FUObjectSerializeContext* InLoadContext)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_FUNC()

	TGuardValue<bool> GuardTemplateNameFlag(GCompilingBlueprint, true);
	ensure(bGeneratedClassLayoutReady);

	if (QueuedRequests.Num() == 0)
	{
		return;
	}

	//当前正在编译的BP
	TArray<FCompilerData> CurrentlyCompilingBPs;
	{ // begin GTimeCompiling scope 
		FScopedDurationTimer SetupTimer(GTimeCompiling);

		// STAGE I: Add any related blueprints that were not compiled, then add any children so that they will be relinked:
		//编译第一阶段:添加任何未编译的相关蓝图,然后添加子项,以便重新链接:  
		//首先添加正在编译的宏库的任何依赖:  
		//我们会将重新编译的蓝图都放在这个数组中
		TArray<UBlueprint*> BlueprintsToRecompile;

		// First add any dependents of macro libraries that are being compiled:
		for (const FBPCompileRequestInternal& CompileJob : QueuedRequests)
		{
			if ((CompileJob.UserData.CompileOptions &
				(EBlueprintCompileOptions::RegenerateSkeletonOnly |
					EBlueprintCompileOptions::IsRegeneratingOnLoad)
				) != EBlueprintCompileOptions::None)
			{
				continue;
			}

			if (CompileJob.UserData.BPToCompile->BlueprintType == BPTYPE_MacroLibrary)
			{
				TArray<UBlueprint*> DependentBlueprints;
				FBlueprintEditorUtils::GetDependentBlueprints(CompileJob.UserData.BPToCompile, DependentBlueprints);
				for (UBlueprint* DependentBlueprint : DependentBlueprints)
				{
					if (!IsQueuedForCompilation(DependentBlueprint))
					{
						DependentBlueprint->bQueuedForCompilation = true;
						CurrentlyCompilingBPs.Emplace(
							FCompilerData(
								DependentBlueprint,
								ECompilationManagerJobType::Normal,
								nullptr,
								EBlueprintCompileOptions::None,
								FBPCompilationManagerCPPResults(),
								false // full compile
							)
						);
						BlueprintsToRecompile.Add(DependentBlueprint);
					}
				}
			}
		}

		// then make sure any normal blueprints have their bytecode dependents recompiled, this is in case a function signature changes:
		for (const FBPCompileRequestInternal& CompileJob : QueuedRequests)
		{
			if ((CompileJob.UserData.CompileOptions & EBlueprintCompileOptions::RegenerateSkeletonOnly) != EBlueprintCompileOptions::None)
			{
				continue;
			}

			// Add any dependent blueprints for a bytecode compile, this is needed because we 
			// have no way to keep bytecode safe when a function is renamed or parameters are
			// added or removed. Below (Stage VIII) we skip further compilation for blueprints 
			// that are being bytecode compiled, but their dependencies have not changed:
			TArray<UBlueprint*> DependentBlueprints;
			FBlueprintEditorUtils::GetDependentBlueprints(CompileJob.UserData.BPToCompile, DependentBlueprints);
			for (UBlueprint* DependentBlueprint : DependentBlueprints)
			{
				if (!IsQueuedForCompilation(DependentBlueprint))
				{
					DependentBlueprint->bQueuedForCompilation = true;
					// Because we're adding this as a bytecode only blueprint compile we don't need to 
					// recursively recompile dependencies. The assumption is that a bytecode only compile
					// will not change the class layout. @todo: add an ensure to detect class layout changes
					CurrentlyCompilingBPs.Emplace(
						FCompilerData(
							DependentBlueprint,
							ECompilationManagerJobType::Normal,
							nullptr,
							EBlueprintCompileOptions::None,
							FBPCompilationManagerCPPResults(),
							true
						)
					);
					BlueprintsToRecompile.Add(DependentBlueprint);
				}
			}
		}

		// STAGE II: Filter out data only and interface blueprints:
		//编译第二阶段:过滤数据和接口蓝图:
		for (int32 I = 0; I < QueuedRequests.Num(); ++I)
		{
			bool bSkipCompile = false;
			FBPCompileRequestInternal& QueuedJob = QueuedRequests[I];
			UBlueprint* QueuedBP = QueuedJob.UserData.BPToCompile;

			ensure(!QueuedBP->GeneratedClass ||
				!QueuedBP->GeneratedClass->ClassDefaultObject ||
				!(QueuedBP->GeneratedClass->ClassDefaultObject->HasAnyFlags(RF_NeedLoad)));
			bool bDefaultComponentMustBeAdded = false;
			bool bHasPendingUberGraphFrame = false;

			if (UBlueprintGeneratedClass* BPGC = Cast<UBlueprintGeneratedClass>(QueuedBP->GeneratedClass))
			{
				if (BPGC->SimpleConstructionScript &&
					BPGC->SimpleConstructionScript->GetSceneRootComponentTemplate() == nullptr)
				{
					bDefaultComponentMustBeAdded = true;
				}

#if USE_UBER_GRAPH_PERSISTENT_FRAME
				bHasPendingUberGraphFrame = BPGC->UberGraphFramePointerProperty || BPGC->UberGraphFunction;
#endif//USE_UBER_GRAPH_PERSISTENT_FRAME
			}

			if (FBlueprintEditorUtils::IsDataOnlyBlueprint(QueuedBP) && !QueuedBP->bHasBeenRegenerated && QueuedBP->GetLinker() && !bDefaultComponentMustBeAdded && !bHasPendingUberGraphFrame)
			{
				const UClass* ParentClass = QueuedBP->ParentClass;
				if (ParentClass && ParentClass->HasAllClassFlags(CLASS_Native))
				{
					bSkipCompile = true;
				}
				else if (const UClass* CurrentClass = QueuedBP->GeneratedClass)
				{
					if (FStructUtils::TheSameLayout(CurrentClass, CurrentClass->GetSuperStruct()))
					{
						bSkipCompile = true;
					}
				}
			}

			if (bSkipCompile)
			{
				CurrentlyCompilingBPs.Emplace(
					FCompilerData(
						QueuedBP,
						ECompilationManagerJobType::SkeletonOnly,
						QueuedJob.UserData.ClientResultsLog,
						QueuedJob.UserData.CompileOptions,
						FBPCompilationManagerCPPResults(),
						false
					)
				);
				if (QueuedBP->GeneratedClass != nullptr)
				{
					// set bIsRegeneratingOnLoad so that we don't reset loaders:
					QueuedBP->bIsRegeneratingOnLoad = true;
					FBlueprintEditorUtils::RemoveStaleFunctions(Cast<UBlueprintGeneratedClass>(QueuedBP->GeneratedClass), QueuedBP);
					QueuedBP->bIsRegeneratingOnLoad = false;
				}

				// No actual compilation work to be done, but try to conform the class and fix up anything that might need to be updated if the native base class has changed in any way
				FKismetEditorUtilities::ConformBlueprintFlagsAndComponents(QueuedBP);

				if (QueuedBP->GeneratedClass)
				{
					FBlueprintEditorUtils::RecreateClassMetaData(QueuedBP, QueuedBP->GeneratedClass, true);
				}

				QueuedRequests.RemoveAtSwap(I);
				--I;
			}
			else
			{
				ECompilationManagerJobType JobType = ECompilationManagerJobType::Normal;
				if ((QueuedJob.UserData.CompileOptions & EBlueprintCompileOptions::RegenerateSkeletonOnly) != EBlueprintCompileOptions::None)
				{
					JobType = ECompilationManagerJobType::SkeletonOnly;
				}

				CurrentlyCompilingBPs.Emplace(
					FCompilerData(
						QueuedBP,
						JobType,
						QueuedJob.UserData.ClientResultsLog,
						QueuedJob.UserData.CompileOptions,
						QueuedJob.CPPResults,
						false
					)
				);

				BlueprintsToRecompile.Add(QueuedBP);
			}
		}

		for (UBlueprint* BP : BlueprintsToRecompile)
		{
			// make sure all children are at least re-linked:
			if (UClass* OldSkeletonClass = BP->SkeletonGeneratedClass)
			{
				TArray<UClass*> SkeletonClassesToReparentList;
				// Has to be recursive gather of children because instances of a UClass will cache information about
				// classes that are above their immediate parent (e.g. ClassConstructor):
				GetDerivedClasses(OldSkeletonClass, SkeletonClassesToReparentList);

				for (UClass* ChildClass : SkeletonClassesToReparentList)
				{
					if (UBlueprint* ChildBlueprint = UBlueprint::GetBlueprintFromClass(ChildClass))
					{
						if (!IsQueuedForCompilation(ChildBlueprint))
						{
							ChildBlueprint->bQueuedForCompilation = true;
							ensure(ChildBlueprint->bHasBeenRegenerated);
							CurrentlyCompilingBPs.Emplace(
								FCompilerData(
									ChildBlueprint,
									ECompilationManagerJobType::RelinkOnly,
									nullptr,
									EBlueprintCompileOptions::None,
									FBPCompilationManagerCPPResults(),
									false
								)
							);
						}
					}
				}
			}
		}

		BlueprintsToRecompile.Empty();
		QueuedRequests.Empty();

		// STAGE III: Sort into correct compilation order. We want to compile root types before their derived (child) types:
		// 编译第三阶段:整理成正确的编译顺序。我们要在根类型派生(子)类型之前编译它们：
		auto HierarchyDepthSortFn = [](const FCompilerData& CompilerDataA, const FCompilerData& CompilerDataB)
		{
			UBlueprint& A = *(CompilerDataA.BP);
			UBlueprint& B = *(CompilerDataB.BP);

			bool bAIsInterface = FBlueprintEditorUtils::IsInterfaceBlueprint(&A);
			bool bBIsInterface = FBlueprintEditorUtils::IsInterfaceBlueprint(&B);

			if (bAIsInterface && !bBIsInterface)
			{
				return true;
			}
			else if (bBIsInterface && !bAIsInterface)
			{
				return false;
			}

			return FBlueprintCompilationManagerImpl::ReinstancerOrderingFunction(A.GeneratedClass, B.GeneratedClass);
		};
		CurrentlyCompilingBPs.Sort(HierarchyDepthSortFn);

		// STAGE IV: Set UBlueprint flags (bBeingCompiled, bIsRegeneratingOnLoad)
		//编译第四阶段:设置 UBlueprint标记(bBeingCompiled, bIsRegeneratingOnLoad)并且重新设置错误状态
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			if (!CompilerData.ShouldSetTemporaryBlueprintFlags())
			{
				continue;
			}

			UBlueprint* BP = CompilerData.BP;
			BP->bBeingCompiled = true;
			BP->CurrentMessageLog = CompilerData.ActiveResultsLog;
			BP->bIsRegeneratingOnLoad = !BP->bHasBeenRegenerated && BP->GetLinker();
			if (BP->bIsRegeneratingOnLoad)
			{
				// we may have cached dependencies before being fully loaded:
				BP->bCachedDependenciesUpToDate = false;
			}

			if (CompilerData.ShouldResetErrorState())
			{
				TArray<UEdGraph*> AllGraphs;
				BP->GetAllGraphs(AllGraphs);
				for (UEdGraph* Graph : AllGraphs)
				{
					for (UEdGraphNode* GraphNode : Graph->Nodes)
					{
						if (GraphNode)
						{
							GraphNode->ClearCompilerMessage();
						}
					}
				}
			}
		}

		// STAGE V: Validate Variable Names
		//编译第五阶段:验证变量名称
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			if (!CompilerData.ShouldValidateVariableNames())
			{
				continue;
			}

			CompilerData.Compiler->ValidateVariableNames();
		}

		// STAGE VI: Purge null graphs, could be done only on load
		//编译第六阶段:清楚空蓝图
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			UBlueprint* BP = CompilerData.BP;
			FBlueprintEditorUtils::PurgeNullGraphs(BP);
		}

		// STAGE VII: safely throw away old skeleton CDOs:
		//编译第七阶段:通过安全的方式丢弃掉旧的骨架
		{
			TMap<UClass*, UClass*> NewSkeletonToOldSkeleton;
			for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
			{
				UBlueprint* BP = CompilerData.BP;
				UClass* OldSkeletonClass = BP->SkeletonGeneratedClass;
				if (OldSkeletonClass)
				{
					MoveSkelCDOAside(OldSkeletonClass, NewSkeletonToOldSkeleton);
				}
			}


			// STAGE VIII: recompile skeleton

			// if any function signatures have changed in this skeleton class we will need to recompile all dependencies, but if not
			// then we can avoid dependency recompilation:
			//注意：第八阶段是非常重要的阶段尤其是下面列出的函数FastGenerateSkeletonClass，因为我们要编译这个骨架类SkeletonGeneratedClass可以加个断电，详细看看里面的原理这一阶段是我们的核心阶段一
			//可以加个断点，详细看看里边的原理
				//编译的第八阶段：重新编译我们的骨架
				//如果在这个骨架类中有任何函数标签发生了变化，我们需要重新编译所有的依赖关系。
				// 当然，我们可以避免依赖重新编译：
				//1.此函数完全取代了kismet编译器中的“仅使用骨架”编译传递。
				//2.客户端将被重定向到此函数。
				//注意：任何uObject必须标记为transient
				//否则你就出错！
			const UBlueprintEditorProjectSettings* EditorProjectSettings = GetDefault<UBlueprintEditorProjectSettings>();
			bool bSkipUnneededDependencyCompilation = !EditorProjectSettings->bForceAllDependenciesToRecompile;
			TSet<UObject*> OldFunctionsWithSignatureChanges;

			for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
			{
				UBlueprint* BP = CompilerData.BP;

				if (CompilerData.ShouldRegenerateSkeleton())
				{
					DECLARE_SCOPE_HIERARCHICAL_COUNTER(RecompileSkeleton)

					if (BlueprintsCompiledOrSkeletonCompiled)
					{
						BlueprintsCompiledOrSkeletonCompiled->Add(BP);
					}

					BP->SkeletonGeneratedClass = FastGenerateSkeletonClass(BP, *(CompilerData.Compiler), CompilerData.IsSkeletonOnly(), CompilerData.SkeletonFixupData);
					//这部分数上边FastGenerateSkeletonClass函数的实现体 2191行
					{
						/*
							This function completely replaces the 'skeleton only' compilation pass in the Kismet compiler. Long
							term that code path will be removed and clients will be redirected to this function.

							Notes to maintainers: any UObject created here and outered to the resulting class must be marked as transient
							or you will create a cook error!
						*/
						UClass* FBlueprintCompilationManagerImpl::FastGenerateSkeletonClass(UBlueprint* BP, FKismetCompilerContext& CompilerContext, bool bIsSkeletonOnly, TArray<FSkeletonFixupData>& OutSkeletonFixupData)
						{
							const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

							FCompilerResultsLog MessageLog;

							UClass* ParentClass = BP->ParentClass;
							if (ParentClass == nullptr)
							{
								return nullptr;
							}

							if (ParentClass->ClassGeneratedBy)
							{
								if (UBlueprint* ParentBP = Cast<UBlueprint>(ParentClass->ClassGeneratedBy))
								{
									if (ParentBP->SkeletonGeneratedClass)
									{
										ParentClass = ParentBP->SkeletonGeneratedClass;
									}
								}
							}

							UBlueprintGeneratedClass* Ret = nullptr;
							UBlueprintGeneratedClass* OriginalNewClass = CompilerContext.NewClass;
							FString SkelClassName = FString::Printf(TEXT("SKEL_%s_C"), *BP->GetName());

							// Temporarily set the compile type to indicate that we're generating the skeleton class.
							TGuardValue<EKismetCompileType::Type> GuardCompileType(CompilerContext.CompileOptions.CompileType, EKismetCompileType::SkeletonOnly);

							if (BP->SkeletonGeneratedClass == nullptr)
							{
								// This might exist in the package because we are being reloaded in place
								BP->SkeletonGeneratedClass = FindObject<UBlueprintGeneratedClass>(BP->GetOutermost(), *SkelClassName);
							}

							//如果骨架类为空就生成一个,如果有就用父类初始化骨架类
							if (BP->SkeletonGeneratedClass == nullptr)
							{
								CompilerContext.SpawnNewClass(SkelClassName);
								Ret = CompilerContext.NewClass;
								Ret->SetFlags(RF_Transient);
								CompilerContext.NewClass = OriginalNewClass;
							}
							else
							{
								Ret = CastChecked<UBlueprintGeneratedClass>(*(BP->SkeletonGeneratedClass));
								CompilerContext.CleanAndSanitizeClass(Ret, Ret->ClassDefaultObject);
								//CleanAndSanitizeClass  KismetCompiler.cpp 224行
								{
									void FKismetCompilerContext::CleanAndSanitizeClass(UBlueprintGeneratedClass* ClassToClean, UObject*& InOldCDO)
									{
										BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_CleanAndSanitizeClass);

										const bool bRecompilingOnLoad = Blueprint->bIsRegeneratingOnLoad;
										FString TransientClassString = FString::Printf(TEXT("TRASHCLASS_%s"), *Blueprint->GetName());
										FName TransientClassName = MakeUniqueObjectName(GetTransientPackage(), UBlueprintGeneratedClass::StaticClass(), FName(*TransientClassString));
										UClass* TransientClass = NewObject<UBlueprintGeneratedClass>(GetTransientPackage(), TransientClassName, RF_Public | RF_Transient);

										UClass* ParentClass = Blueprint->ParentClass;

										if (CompileOptions.CompileType == EKismetCompileType::SkeletonOnly)
										{
											if (UBlueprint* BlueprintParent = Cast<UBlueprint>(Blueprint->ParentClass->ClassGeneratedBy))
											{
												ParentClass = BlueprintParent->SkeletonGeneratedClass;
											}
										}

										if (ParentClass == NULL)
										{
											ParentClass = UObject::StaticClass();
										}
										TransientClass->ClassAddReferencedObjects = ParentClass->AddReferencedObjects;
										TransientClass->ClassGeneratedBy = Blueprint;
										TransientClass->ClassFlags |= CLASS_CompiledFromBlueprint;

										SetNewClass(ClassToClean);
										InOldCDO = ClassToClean->ClassDefaultObject; // we don't need to create the CDO at this point

										const ERenameFlags RenFlags = REN_DontCreateRedirectors | ((bRecompilingOnLoad) ? REN_ForceNoResetLoaders : 0) | REN_NonTransactional | REN_DoNotDirty;

										if (InOldCDO)
										{
											FString TransientCDOString = FString::Printf(TEXT("TRASH_%s"), *InOldCDO->GetName());
											FName TransientCDOName = MakeUniqueObjectName(GetTransientPackage(), TransientClass, FName(*TransientCDOString));
											InOldCDO->Rename(*TransientCDOName.ToString(), GetTransientPackage(), RenFlags);
											FLinkerLoad::InvalidateExport(InOldCDO);
										}

										// Purge all subobjects (properties, functions, params) of the class, as they will be regenerated
										TArray<UObject*> ClassSubObjects;
										GetObjectsWithOuter(ClassToClean, ClassSubObjects, false);

										{
											// Save subobjects, that won't be regenerated.
											FSubobjectCollection SubObjectsToSave;
											SaveSubObjectsFromCleanAndSanitizeClass(SubObjectsToSave, ClassToClean);

											ClassSubObjects.RemoveAllSwap(SubObjectsToSave);
										}

										UClass* InheritableComponentHandlerClass = UInheritableComponentHandler::StaticClass();

										for (UObject* CurrSubObj : ClassSubObjects)
										{
											// ICH and ICH templates do not need to be destroyed in this way.. doing so will invalidate
											// transaction buffer references to these UObjects. The UBlueprint may not have a reference to
											// the ICH at the moment, and therefore might not have added it to SubObjectsToSave (and
											// removed the ICH from ClassSubObjects):
											if (Cast<UInheritableComponentHandler>(CurrSubObj) || CurrSubObj->IsInA(InheritableComponentHandlerClass))
											{
												continue;
											}

											FName NewSubobjectName = MakeUniqueObjectName(TransientClass, CurrSubObj->GetClass(), CurrSubObj->GetFName());
											CurrSubObj->Rename(*NewSubobjectName.ToString(), TransientClass, RenFlags);
											if (UProperty* Prop = Cast<UProperty>(CurrSubObj))
											{
												FKismetCompilerUtilities::InvalidatePropertyExport(Prop);
											}
											else
											{
												FLinkerLoad::InvalidateExport(CurrSubObj);
											}
										}

										// Purge the class to get it back to a "base" state
										bool bLayoutChanging = ClassToClean->HasAnyClassFlags(CLASS_LayoutChanging);
										ClassToClean->PurgeClass(bRecompilingOnLoad);

										// Set properties we need to regenerate the class with
										//用父类初始化我们的骨架类
										ClassToClean->PropertyLink = ParentClass->PropertyLink;
										ClassToClean->SetSuperStruct(ParentClass);
										ClassToClean->ClassWithin = ParentClass->ClassWithin ? ParentClass->ClassWithin : UObject::StaticClass();
										ClassToClean->ClassConfigName = ClassToClean->IsNative() ? FName(ClassToClean->StaticConfigName()) : ParentClass->ClassConfigName;
										ClassToClean->DebugData = FBlueprintDebugData();

										if (bLayoutChanging)
										{
											ClassToClean->ClassFlags |= CLASS_LayoutChanging;
										}
									}

								}
							}

							Ret->ClassGeneratedBy = BP;

							// This is a version of PrecompileFunction that does not require 'terms' and graph cloning:
							const auto MakeFunction = [Ret, ParentClass, Schema, BP, &MessageLog, &OutSkeletonFixupData]
							(FName FunctionNameFName,
								UField**& InCurrentFieldStorageLocation,
								UField**& InCurrentParamStorageLocation,
								EFunctionFlags InFunctionFlags,
								const TArray<UK2Node_FunctionResult*>& ReturnNodes,
								const TArray<UEdGraphPin*>& InputPins,
								bool bIsStaticFunction,
								bool bForceArrayStructRefsConst,
								UFunction* SignatureOverride) -> UFunction*
							{
								if (!ensure(FunctionNameFName != FName())
									|| FindObjectFast<UField>(Ret, FunctionNameFName))
								{
									return nullptr;
								}
								//考虑到我们使用的是蓝图，蓝图函数，没有具体的执行C++函数，所以我们需要创建一个
								//并且将函数放在我们Ret里面，这个Ret就是我们前面声明的Class，并且完善我们的函数链
								UFunction* NewFunction = NewObject<UFunction>(Ret, FunctionNameFName, RF_Public | RF_Transient);

								Ret->AddFunctionToFunctionMap(NewFunction, NewFunction->GetFName());

								*InCurrentFieldStorageLocation = NewFunction;
								InCurrentFieldStorageLocation = &NewFunction->Next;

								if (bIsStaticFunction)
								{
									NewFunction->FunctionFlags |= FUNC_Static;
								}

								UFunction* ParentFn = ParentClass->FindFunctionByName(NewFunction->GetFName());
								if (ParentFn == nullptr)
								{
									// check for function in implemented interfaces:
									for (const FBPInterfaceDescription& BPID : BP->ImplementedInterfaces)
									{
										// we only want the *skeleton* version of the function:
										UClass* InterfaceClass = BPID.Interface;
										// We need to null check because FBlueprintEditorUtils::ConformImplementedInterfaces won't run until 
										// after the skeleton classes have been generated:
										if (InterfaceClass)
										{
											if (UBlueprint* Owner = Cast<UBlueprint>(InterfaceClass->ClassGeneratedBy))
											{
												if (ensure(Owner->SkeletonGeneratedClass))
												{
													InterfaceClass = Owner->SkeletonGeneratedClass;
												}
											}

											if (UFunction* ParentInterfaceFn = InterfaceClass->FindFunctionByName(NewFunction->GetFName()))
											{
												ParentFn = ParentInterfaceFn;
												break;
											}
										}
									}
								}
								//重新设置函数父类，并且开始完善我们的属性链
								NewFunction->SetSuperStruct(ParentFn);

								InCurrentParamStorageLocation = &NewFunction->Children;

								// params:
								if (ParentFn || SignatureOverride)
								{
									UFunction* SignatureFn = ParentFn ? ParentFn : SignatureOverride;
									NewFunction->FunctionFlags |= (SignatureFn->FunctionFlags & (FUNC_FuncInherit | FUNC_Public | FUNC_Protected | FUNC_Private | FUNC_BlueprintPure));
									for (TFieldIterator<UProperty> PropIt(SignatureFn); PropIt && (PropIt->PropertyFlags & CPF_Parm); ++PropIt)
									{
										UProperty* ClonedParam = CastChecked<UProperty>(StaticDuplicateObject(*PropIt, NewFunction, PropIt->GetFName(), RF_AllFlags, nullptr, EDuplicateMode::Normal, EInternalObjectFlags::AllFlags & ~(EInternalObjectFlags::Native)));
										ClonedParam->PropertyFlags |= CPF_BlueprintVisible | CPF_BlueprintReadOnly;
										ClonedParam->Next = nullptr;
										*InCurrentParamStorageLocation = ClonedParam;
										InCurrentParamStorageLocation = &ClonedParam->Next;
									}
									UMetaData::CopyMetadata(SignatureFn, NewFunction);
								}
								else
								{
									NewFunction->FunctionFlags |= InFunctionFlags;
									for (UEdGraphPin* Pin : InputPins)
									{
										if (Pin->Direction == EEdGraphPinDirection::EGPD_Output && !Schema->IsExecPin(*Pin) && Pin->ParentPin == nullptr && Pin->GetFName() != UK2Node_Event::DelegateOutputName)
										{
											// Reimplementation of FKismetCompilerContext::CreatePropertiesFromList without dependence on 'terms'
											UProperty* Param = FKismetCompilerUtilities::CreatePropertyOnScope(NewFunction, Pin->PinName, Pin->PinType, Ret, CPF_BlueprintVisible | CPF_BlueprintReadOnly, Schema, MessageLog);
											if (Param)
											{
												Param->SetFlags(RF_Transient);
												Param->PropertyFlags |= CPF_Parm;
												if (Pin->PinType.bIsReference)
												{
													Param->PropertyFlags |= CPF_ReferenceParm | CPF_OutParm;
												}

												if (Pin->PinType.bIsConst || (bForceArrayStructRefsConst && (Pin->PinType.IsArray() || Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct) && Pin->PinType.bIsReference))
												{
													Param->PropertyFlags |= CPF_ConstParm;
												}

												if (UObjectProperty* ObjProp = Cast<UObjectProperty>(Param))
												{
													UClass* EffectiveClass = nullptr;
													if (ObjProp->PropertyClass != nullptr)
													{
														EffectiveClass = ObjProp->PropertyClass;
													}
													else if (UClassProperty* ClassProp = Cast<UClassProperty>(ObjProp))
													{
														EffectiveClass = ClassProp->MetaClass;
													}

													if ((EffectiveClass != nullptr) && (EffectiveClass->HasAnyClassFlags(CLASS_Const)))
													{
														Param->PropertyFlags |= CPF_ConstParm;
													}
												}
												else if (UArrayProperty* ArrayProp = Cast<UArrayProperty>(Param))
												{
													Param->PropertyFlags |= CPF_ReferenceParm;

													// ALWAYS pass array parameters as out params, so they're set up as passed by ref
													Param->PropertyFlags |= CPF_OutParm;
												}
												// Delegate properties have a direct reference to a UFunction that we may currently be generating, so we're going
												// to track them and fix them after all UFunctions have been generated. As you can tell we're tightly coupled
												// to the implementation of CreatePropertyOnScope
												else if (UDelegateProperty* DelegateProp = Cast<UDelegateProperty>(Param))
												{
													OutSkeletonFixupData.Add({
														Pin->PinType.PinSubCategoryMemberReference,
														DelegateProp
														});
												}
												else if (UMulticastDelegateProperty* MCDelegateProp = Cast<UMulticastDelegateProperty>(Param))
												{
													OutSkeletonFixupData.Add({
														Pin->PinType.PinSubCategoryMemberReference,
														MCDelegateProp
														});
												}

												*InCurrentParamStorageLocation = Param;
												InCurrentParamStorageLocation = &Param->Next;
											}
										}
									}

									if (ReturnNodes.Num() > 0)
									{
										// Gather all input pins on these nodes, these are 
										// the outputs of the function:
										TSet<FName> UsedPinNames;
										static const FName RetValName = FName(TEXT("ReturnValue"));
										for (UK2Node_FunctionResult* Node : ReturnNodes)
										{
											for (UEdGraphPin* Pin : Node->Pins)
											{
												if (!Schema->IsExecPin(*Pin) && Pin->ParentPin == nullptr)
												{
													if (!UsedPinNames.Contains(Pin->PinName))
													{
														UsedPinNames.Add(Pin->PinName);

														UProperty* Param = FKismetCompilerUtilities::CreatePropertyOnScope(NewFunction, Pin->PinName, Pin->PinType, Ret, CPF_None, Schema, MessageLog);
														if (Param)
														{
															Param->SetFlags(RF_Transient);
															// we only tag things as CPF_ReturnParm if the value is named ReturnValue.... this is *terrible* behavior:
															if (Param->GetFName() == RetValName)
															{
																Param->PropertyFlags |= CPF_ReturnParm;
															}
															Param->PropertyFlags |= CPF_Parm | CPF_OutParm;
															*InCurrentParamStorageLocation = Param;
															InCurrentParamStorageLocation = &Param->Next;
														}
													}
												}
											}
										}
									}
								}

								// We're linking the skeleton function because TProperty::LinkInternal
								// will assign add TTypeFundamentals::GetComputedFlagsPropertyFlags()
								// to PropertyFlags. PropertyFlags must (mostly) match in order for 
								// functions to be compatible:
								//链接并且返回
								NewFunction->StaticLink(true);
								return NewFunction;
							};


							// helpers:
							const auto AddFunctionForGraphs = [Schema, &MessageLog, ParentClass, Ret, BP, MakeFunction, &CompilerContext](const TCHAR* FunctionNamePostfix, const TArray<UEdGraph*>& Graphs, UField**& InCurrentFieldStorageLocation, bool bIsStaticFunction, bool bAreDelegateGraphs)
							{
								for (const UEdGraph* Graph : Graphs)
								{
									TArray<UK2Node_FunctionEntry*> EntryNodes;
									Graph->GetNodesOfClass(EntryNodes);
									if (EntryNodes.Num() > 0)
									{
										TArray<UK2Node_FunctionResult*> ReturnNodes;
										Graph->GetNodesOfClass(ReturnNodes);
										UK2Node_FunctionEntry* EntryNode = EntryNodes[0];
										FName NewFunctionName = (EntryNode->CustomGeneratedFunctionName != NAME_None) ? EntryNode->CustomGeneratedFunctionName : Graph->GetFName();

										UField** CurrentParamStorageLocation = nullptr;
										UFunction* NewFunction = MakeFunction(
											FName(*(NewFunctionName.ToString() + FunctionNamePostfix)),
											InCurrentFieldStorageLocation,
											CurrentParamStorageLocation,
											(EFunctionFlags)(EntryNode->GetFunctionFlags() & ~FUNC_Native),
											ReturnNodes,
											EntryNode->Pins,
											bIsStaticFunction,
											false,
											nullptr
										);

										if (NewFunction)
										{
											if (bAreDelegateGraphs)
											{
												NewFunction->FunctionFlags |= FUNC_Delegate;
											}

											// locals:
											for (const FBPVariableDescription& BPVD : EntryNode->LocalVariables)
											{
												if (UProperty* LocalVariable = FKismetCompilerContext::CreateUserDefinedLocalVariableForFunction(BPVD, NewFunction, Ret, CurrentParamStorageLocation, Schema, MessageLog))
												{
													LocalVariable->SetFlags(RF_Transient);
												}
											}

											// __WorldContext:
											if (bIsStaticFunction)
											{
												if (FindField<UObjectProperty>(NewFunction, TEXT("__WorldContext")) == nullptr)
												{
													FEdGraphPinType WorldContextPinType(UEdGraphSchema_K2::PC_Object, NAME_None, UObject::StaticClass(), EPinContainerType::None, false, FEdGraphTerminalType());
													UProperty* Param = FKismetCompilerUtilities::CreatePropertyOnScope(NewFunction, TEXT("__WorldContext"), WorldContextPinType, Ret, CPF_None, Schema, MessageLog);
													if (Param)
													{
														Param->SetFlags(RF_Transient);
														Param->PropertyFlags |= CPF_Parm;
														*CurrentParamStorageLocation = Param;
														CurrentParamStorageLocation = &Param->Next;
													}
												}

												// set the metdata:
												NewFunction->SetMetaData(FBlueprintMetadata::MD_WorldContext, TEXT("__WorldContext"));
											}

											CompilerContext.SetCalculatedMetaDataAndFlags(NewFunction, EntryNode, Schema);
										}
									}
								}
							};

							UField** CurrentFieldStorageLocation = &Ret->Children;

							// Helper function for making UFunctions generated for 'event' nodes, e.g. custom event and timelines
							const auto MakeEventFunction = [&CurrentFieldStorageLocation, MakeFunction, Schema](FName InName, EFunctionFlags ExtraFnFlags, const TArray<UEdGraphPin*>& InputPins, const TArray< TSharedPtr<FUserPinInfo> >& UserPins, UFunction* InSourceFN, bool bInCallInEditor, bool bIsDeprecated, const FString& DeprecationMessage, FKismetUserDeclaredFunctionMetadata* UserDefinedMetaData = nullptr)
							{
								UField** CurrentParamStorageLocation = nullptr;

								UFunction* NewFunction = MakeFunction(
									InName,
									CurrentFieldStorageLocation,
									CurrentParamStorageLocation,
									ExtraFnFlags | FUNC_BlueprintCallable | FUNC_BlueprintEvent,
									TArray<UK2Node_FunctionResult*>(),
									InputPins,
									false,
									true,
									InSourceFN
								);

								if (NewFunction)
								{
									FKismetCompilerContext::SetDefaultInputValueMetaData(NewFunction, UserPins);

									if (bIsDeprecated)
									{
										NewFunction->SetMetaData(FBlueprintMetadata::MD_DeprecatedFunction, TEXT("true"));
										if (!DeprecationMessage.IsEmpty())
										{
											NewFunction->SetMetaData(FBlueprintMetadata::MD_DeprecationMessage, *DeprecationMessage);
										}
									}

									if (bInCallInEditor)
									{
										NewFunction->SetMetaData(FBlueprintMetadata::MD_CallInEditor, TEXT("true"));
									}

									if (UserDefinedMetaData)
									{
										NewFunction->SetMetaData(FBlueprintMetadata::MD_FunctionKeywords, *(UserDefinedMetaData->Keywords).ToString());
									}

									NewFunction->Bind();
									NewFunction->StaticLink(true);
								}
							};

							Ret->SetSuperStruct(ParentClass);

							Ret->ClassFlags |= (ParentClass->ClassFlags & CLASS_Inherit);
							Ret->ClassCastFlags |= ParentClass->ClassCastFlags;

							if (FBlueprintEditorUtils::IsInterfaceBlueprint(BP))
							{
								Ret->ClassFlags |= CLASS_Interface;
							}

							// link in delegate signatures, variables will reference these 
							AddFunctionForGraphs(HEADER_GENERATED_DELEGATE_SIGNATURE_SUFFIX, BP->DelegateSignatureGraphs, CurrentFieldStorageLocation, false, true);

							// handle event entry ponts (mostly custom events) - this replaces
							// the skeleton compile pass CreateFunctionStubForEvent call:
							TArray<UEdGraph*> AllEventGraphs;

							for (UEdGraph* UberGraph : BP->UbergraphPages)
							{
								AllEventGraphs.Add(UberGraph);
								UberGraph->GetAllChildrenGraphs(AllEventGraphs);
							}

							//生成函数的具体调用位置，构造所有Event函数
							for (const UEdGraph* Graph : AllEventGraphs)
							{
								TArray<UK2Node_Event*> EventNodes;
								Graph->GetNodesOfClass(EventNodes);
								for (UK2Node_Event* Event : EventNodes)
								{
									FString DeprecationMessage;
									bool bIsDeprecated = false;
									bool bCallInEditor = false;
									FKismetUserDeclaredFunctionMetadata* UserMetaData = nullptr;
									if (UK2Node_CustomEvent* CustomEvent = Cast<UK2Node_CustomEvent>(Event))
									{
										bCallInEditor = CustomEvent->bCallInEditor;
										bIsDeprecated = CustomEvent->bIsDeprecated;
										if (bIsDeprecated)
										{
											DeprecationMessage = CustomEvent->DeprecationMessage;
										}
										UserMetaData = &(CustomEvent->GetUserDefinedMetaData());
									}
									MakeEventFunction(
										CompilerContext.GetEventStubFunctionName(Event),
										(EFunctionFlags)Event->FunctionFlags,
										Event->Pins,
										Event->UserDefinedPins,
										Event->FindEventSignatureFunction(),
										bCallInEditor,
										bIsDeprecated,
										DeprecationMessage,
										UserMetaData
									);
								}
							}

							//如果有TimeLine就创建Timeline
							for (UTimelineTemplate* Timeline : BP->Timelines)
							{
								if (Timeline)
								{
									// If the timeline hasn't gone through post load that means that the cache isn't up to date, so force an update on it
									if (Timeline->HasAllFlags(RF_NeedPostLoad) && Timeline->GetLinkerCustomVersion(FFortniteMainBranchObjectVersion::GUID) < FFortniteMainBranchObjectVersion::StoreTimelineNamesInTemplate)
									{
										FUpdateTimelineCachedNames::Execute(Timeline);
									}

									for (const FTTEventTrack& EventTrack : Timeline->EventTracks)
									{
										MakeEventFunction(EventTrack.GetFunctionName(), EFunctionFlags::FUNC_None, TArray<UEdGraphPin*>(), TArray< TSharedPtr<FUserPinInfo> >(), nullptr, false, false, FString());
									}

									MakeEventFunction(Timeline->GetUpdateFunctionName(), EFunctionFlags::FUNC_None, TArray<UEdGraphPin*>(), TArray< TSharedPtr<FUserPinInfo> >(), nullptr, false, false, FString());
									MakeEventFunction(Timeline->GetFinishedFunctionName(), EFunctionFlags::FUNC_None, TArray<UEdGraphPin*>(), TArray< TSharedPtr<FUserPinInfo> >(), nullptr, false, false, FString());
								}
							}

							{
								CompilerContext.NewClass = Ret;
								TGuardValue<bool> GuardAssignDelegateSignatureFunction(CompilerContext.bAssignDelegateSignatureFunction, true);
								TGuardValue<bool> GuardGenerateLinkedAnimGraphVariables(CompilerContext.bGenerateLinkedAnimGraphVariables, true);
								//创建类变量
								CompilerContext.CreateClassVariablesFromBlueprint();
								CompilerContext.NewClass = OriginalNewClass;
							}
							UField* Iter = Ret->Children;
							while (Iter)
							{
								CurrentFieldStorageLocation = &Iter->Next;
								Iter = Iter->Next;
							}

							//他会帮我们生成函数并且链接到属性，参数，元数据等，如果函数是静态的会特别处理
							AddFunctionForGraphs(TEXT(""), BP->FunctionGraphs, CurrentFieldStorageLocation, BPTYPE_FunctionLibrary == BP->BlueprintType, false);

							// Add interface functions, often these are added by normal detection of implemented functions, but they won't be
							// if the interface is added but the function is not implemented:
							for (const FBPInterfaceDescription& BPID : BP->ImplementedInterfaces)
							{
								UClass* InterfaceClass = BPID.Interface;
								// Again, once the skeleton has been created we will purge null ImplementedInterfaces entries,
								// but not yet:
								if (InterfaceClass)
								{
									if (UBlueprint* Owner = Cast<UBlueprint>(InterfaceClass->ClassGeneratedBy))
									{
										if (ensure(Owner->SkeletonGeneratedClass))
										{
											InterfaceClass = Owner->SkeletonGeneratedClass;
										}
									}

									AddFunctionForGraphs(TEXT(""), BPID.Graphs, CurrentFieldStorageLocation, BPTYPE_FunctionLibrary == BP->BlueprintType, false);

									for (TFieldIterator<UFunction> FunctionIt(InterfaceClass, EFieldIteratorFlags::ExcludeSuper); FunctionIt; ++FunctionIt)
									{
										UFunction* Fn = *FunctionIt;

										UField** CurrentParamStorageLocation = nullptr;

										// Note that MakeFunction will early out if the function was created above:
										MakeFunction(
											Fn->GetFName(),
											CurrentFieldStorageLocation,
											CurrentParamStorageLocation,
											Fn->FunctionFlags & ~FUNC_Native,
											TArray<UK2Node_FunctionResult*>(),
											TArray<UEdGraphPin*>(),
											false,
											false,
											nullptr
										);
									}
								}
							}

							// Add the uber graph frame just so that we match the old skeleton class's layout. This will be removed in 4.20:
							if (CompilerContext.UsePersistentUberGraphFrame() && AllEventGraphs.Num() != 0)
							{
								//UBER GRAPH PERSISTENT FRAME
								FEdGraphPinType Type(TEXT("struct"), NAME_None, FPointerToUberGraphFrame::StaticStruct(), EPinContainerType::None, false, FEdGraphTerminalType());
								CompilerContext.NewClass = Ret;
								UProperty* Property = CompilerContext.CreateVariable(UBlueprintGeneratedClass::GetUberGraphFrameName(), Type);
								CompilerContext.NewClass = OriginalNewClass;
								Property->SetPropertyFlags(CPF_DuplicateTransient | CPF_Transient);
							}

							//开始晚上我们类的编译，比如这个类里面需要执行的函数，参数等，最终将这些内容汇聚到CompilerContext里面，做下一步的编译工作
							CompilerContext.NewClass = Ret;
							CompilerContext.bAssignDelegateSignatureFunction = true;
							CompilerContext.FinishCompilingClass(Ret);
							CompilerContext.bAssignDelegateSignatureFunction = false;
							CompilerContext.NewClass = OriginalNewClass;

							Ret->GetDefaultObject()->SetFlags(RF_Transient);

							return Ret;
						}
					}

					UBlueprintGeneratedClass* AuthoritativeClass = Cast<UBlueprintGeneratedClass>(BP->GeneratedClass);
					if (AuthoritativeClass && bSkipUnneededDependencyCompilation)
					{
						if (CompilerData.InternalOptions.CompileType == EKismetCompileType::Full)
						{
							for (TFieldIterator<UFunction> FuncIt(AuthoritativeClass, EFieldIteratorFlags::ExcludeSuper); FuncIt; ++FuncIt)
							{
								UFunction* OldFunction = *FuncIt;

								if (!OldFunction->HasAnyFunctionFlags(EFunctionFlags::FUNC_BlueprintCallable))
								{
									continue;
								}

								// We assume that if the func is FUNC_BlueprintCallable that it will be present in the Skeleton class.
								// If it is not in the skeleton class we will always think that this blueprints public interface has 
								// changed. Not a huge deal, but will mean we recompile dependencies more often than necessary.
								UFunction* NewFunction = BP->SkeletonGeneratedClass->FindFunctionByName((OldFunction)->GetFName());
								if (NewFunction == nullptr ||
									!NewFunction->IsSignatureCompatibleWith(OldFunction) ||
									// If a function changes its net flags, callers may now need to do a full EX_FinalFunction/EX_VirtualFunction 
									// instead of a EX_LocalFinalFunction/EX_LocalVirtualFunction:
									NewFunction->HasAnyFunctionFlags(FUNC_NetFuncFlags) != OldFunction->HasAnyFunctionFlags(FUNC_NetFuncFlags))
								{
									OldFunctionsWithSignatureChanges.Add(OldFunction);
									break;
								}
							}
						}
					}
				}
				else
				{
					DECLARE_SCOPE_HIERARCHICAL_COUNTER(RelinkSkeleton)

					// Just relink, note that UProperties that reference *other* types may be stale until
					// we fixup below:
					UClass* SkeletonToRelink = BP->SkeletonGeneratedClass;

					// CDO needs to be moved aside already:
					ensure(SkeletonToRelink->ClassDefaultObject == nullptr);
					ensure(!SkeletonToRelink->GetSuperClass()->HasAnyClassFlags(CLASS_NewerVersionExists));

					SkeletonToRelink->ClassConstructor = nullptr;
					SkeletonToRelink->ClassVTableHelperCtorCaller = nullptr;
					SkeletonToRelink->ClassAddReferencedObjects = nullptr;
					SkeletonToRelink->Bind();
					SkeletonToRelink->ClearFunctionMapsCaches();
					SkeletonToRelink->StaticLink(true);
					SkeletonToRelink->GetDefaultObject();
				}

				if (CompilerData.ShouldMarkUpToDateAfterSkeletonStage())
				{
					// Flag data only blueprints as being up-to-date
					BP->Status = BP->bHasBeenRegenerated ? CompilerData.OriginalBPStatus : BS_UpToDate;
					BP->bHasBeenRegenerated = true;
					if (BP->GeneratedClass)
					{
						DECLARE_SCOPE_HIERARCHICAL_COUNTER(ClearFunctionMapsCaches)
						BP->GeneratedClass->ClearFunctionMapsCaches();
					}
				}
			}

			// Fix up delegate parameters on skeleton class UFunctions, as they have a direct reference to a UFunction*
			// that may have been created as part of skeleton generation:
			for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
			{
				DECLARE_SCOPE_HIERARCHICAL_COUNTER(FixUpDelegateParameters)

				UBlueprint* BP = CompilerData.BP;
				TArray< FSkeletonFixupData >& ParamsToFix = CompilerData.SkeletonFixupData;
				for (const FSkeletonFixupData& FixupData : ParamsToFix)
				{
					if (UDelegateProperty* DelegateProperty = Cast<UDelegateProperty>(FixupData.DelegateProperty))
					{
						DelegateProperty->SignatureFunction = FMemberReference::ResolveSimpleMemberReference<UFunction>(FixupData.MemberReference, BP->SkeletonGeneratedClass);
					}
					else if (UMulticastDelegateProperty* MCDelegateProperty = Cast<UMulticastDelegateProperty>(FixupData.DelegateProperty))
					{
						MCDelegateProperty->SignatureFunction = FMemberReference::ResolveSimpleMemberReference<UFunction>(FixupData.MemberReference, BP->SkeletonGeneratedClass);
					}
				}
			}

			// Skip further compilation for blueprints that are being bytecode compiled as a dependency of something that has
			// not had a change in its function parameters:
			if (bSkipUnneededDependencyCompilation)
			{
				const auto HasNoReferencesToChangedFunctions = [&OldFunctionsWithSignatureChanges](FCompilerData& Data)
				{
					if (!Data.ShouldSkipIfDependenciesAreUnchanged())
					{
						return false;
					}

					// if our parent is still being compiled, then we still need to be compiled:
					UClass* Iter = Data.BP->ParentClass;
					while (Iter)
					{
						if (UBlueprint* BP = Cast<UBlueprint>(Iter->ClassGeneratedBy))
						{
							if (BP->bBeingCompiled)
							{
								return false;
							}
						}
						Iter = Iter->GetSuperClass();
					}

					// look for references to a function with a signature change
					// in the old class, if it has none, we can skip bytecode recompile:
					bool bHasNoReferencesToChangedFunctions = true;
					UBlueprintGeneratedClass* BPGC = Cast<UBlueprintGeneratedClass>(Data.BP->GeneratedClass);
					if (BPGC)
					{
						for (UFunction* CalledFunction : BPGC->CalledFunctions)
						{
							if (OldFunctionsWithSignatureChanges.Contains(CalledFunction))
							{
								bHasNoReferencesToChangedFunctions = false;
								break;
							}
						}
					}

					if (bHasNoReferencesToChangedFunctions)
					{
						// This BP is not actually going to be compiled, clean it up:
						Data.BP->bBeingCompiled = false;
						Data.BP->CurrentMessageLog = nullptr;
						if (UPackage* Package = Data.BP->GetOutermost())
						{
							Package->SetDirtyFlag(Data.bPackageWasDirty);
						}
						if (Data.ResultsLog)
						{
							Data.ResultsLog->EndEvent();
						}
						Data.BP->bQueuedForCompilation = false;
					}

					return bHasNoReferencesToChangedFunctions;
				};

				// Order very much matters, but we could RemoveAllSwap and re-sort:
				CurrentlyCompilingBPs.RemoveAll(HasNoReferencesToChangedFunctions);
			}
		}

		// STAGE IX: Reconstruct nodes and replace deprecated nodes, then broadcast 'precompile
		//编译第九阶段：重建节点并替换丢弃的节点，然后广播“预编译”
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			if (!CompilerData.ShouldReconstructNodes())
			{
				continue;
			}

			DECLARE_SCOPE_HIERARCHICAL_COUNTER(ReconstructNodes)

			UBlueprint* BP = CompilerData.BP;

			// Some nodes are set up to do things during reconstruction only when this flag is NOT set.
			if (BP->bIsRegeneratingOnLoad)
			{
				FBlueprintEditorUtils::ReconstructAllNodes(BP);
				FBlueprintEditorUtils::ReplaceDeprecatedNodes(BP);
			}
			else
			{
				// matching existing behavior, when compiling a BP not on load we refresh nodes
				// before compiling:
				FBlueprintCompileReinstancer::OptionallyRefreshNodes(BP);
				TArray<UBlueprint*> DependentBlueprints;
				FBlueprintEditorUtils::GetDependentBlueprints(BP, DependentBlueprints);

				for (UBlueprint* CurrentBP : DependentBlueprints)
				{
					const EBlueprintStatus OriginalStatus = CurrentBP->Status;
					UPackage* const Package = CurrentBP->GetOutermost();
					const bool bStartedWithUnsavedChanges = Package != nullptr ? Package->IsDirty() : true;

					FBlueprintEditorUtils::RefreshExternalBlueprintDependencyNodes(CurrentBP, BP->GeneratedClass);

					CurrentBP->Status = OriginalStatus;
					if (Package != nullptr && Package->IsDirty() && !bStartedWithUnsavedChanges)
					{
						Package->SetDirtyFlag(false);
					}
				}
			}

			// Broadcast pre-compile
			{
				if (GEditor && GIsEditor)
				{
					GEditor->BroadcastBlueprintPreCompile(BP);
				}
			}

			if (CompilerData.ShouldUpdateBlueprintSearchMetadata())
			{
				// Do not want to run this code without the editor present nor when running commandlets.
				if (GEditor && GIsEditor)
				{
					// We do not want to regenerate a search Guid during loads, nothing has changed in the Blueprint and it is cached elsewhere
					if (!BP->bIsRegeneratingOnLoad)
					{
						FFindInBlueprintSearchManager::Get().AddOrUpdateBlueprintSearchMetadata(BP);
					}
				}
			}

			// we are regenerated, tag ourself as such so that
			// old logic to 'fix' circular dependencies doesn't
			// cause redundant regeneration (e.g. bForceRegenNodes
			// in ExpandTunnelsAndMacros):
			BP->bHasBeenRegenerated = true;
		}

		// STAGE X: reinstance every blueprint that is queued, note that this means classes in the hierarchy that are *not* being 
		// compiled will be parented to REINST versions of the class, so type checks (IsA, etc) involving those types
		// will be incoherent!
		//编译第十阶段:检查队列中的每一个蓝图,注意这个检查只是检查继承的层次结构中的检查。  
		//被编译的类将会被父类重新设置该类的版本,我们可能会用IsA这种方式来检查该类
		{
			for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
			{
				// we including skeleton only compilation jobs for reinstancing because we need UpdateCustomPropertyListForPostConstruction
				// to happen (at the right time) for those generated classes as well. This means we *don't* need to reinstance if 
				// the parent is a native type (unless we hot reload, but that should not need to be handled here):
				if (CompilerData.ShouldSkipReinstancerCreation())
				{
					continue;
				}

				DECLARE_SCOPE_HIERARCHICAL_COUNTER(ReinstanceQueuedBlueprint)

				// no need to reinstance skeleton or relink jobs that are not in a hierarchy that has had reinstancing initiated:
				bool bRequiresReinstance = CompilerData.ShouldInitiateReinstancing();
				if (!bRequiresReinstance)
				{
					UClass* Iter = CompilerData.BP->GeneratedClass;
					if (!Iter)
					{
						bRequiresReinstance = true;
					}
					while (Iter)
					{
						if (Iter->HasAnyClassFlags(CLASS_NewerVersionExists))
						{
							bRequiresReinstance = true;
							break;
						}

						Iter = Iter->GetSuperClass();
					}
				}

				if (!bRequiresReinstance)
				{
					continue;
				}

				UBlueprint* BP = CompilerData.BP;

				if (BP->GeneratedClass)
				{
					OldCDOs.Add(BP, BP->GeneratedClass->ClassDefaultObject);
				}

				EBlueprintCompileReinstancerFlags CompileReinstancerFlags =
					EBlueprintCompileReinstancerFlags::AutoInferSaveOnCompile;
				if (!CompilerData.IsCppCompileType())
				{
					CompileReinstancerFlags |= EBlueprintCompileReinstancerFlags::AvoidCDODuplication;
				}

				CompilerData.Reinstancer = TSharedPtr<FBlueprintCompileReinstancer>(
					new FBlueprintCompileReinstancer(
						BP->GeneratedClass,
						CompileReinstancerFlags
					)
					);

				if (CompilerData.Compiler.IsValid())
				{
					CompilerData.Compiler->OldClass = Cast<UBlueprintGeneratedClass>(CompilerData.Reinstancer->DuplicatedClass);
				}

				if (BP->GeneratedClass)
				{
					BP->GeneratedClass->ClassFlags |= CLASS_LayoutChanging;
				}
			}
		}

		// STAGE XI: Reinstancing done, lets fix up child->parent pointers
		//编译的第十一阶段:如果我们的类有标记 CLASS NewerVersionExists,那么我们会重新设置 Generatedclas父类
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			UBlueprint* BP = CompilerData.BP;
			if (BP->GeneratedClass && BP->GeneratedClass->GetSuperClass()->HasAnyClassFlags(CLASS_NewerVersionExists))
			{
				BP->GeneratedClass->SetSuperStruct(BP->GeneratedClass->GetSuperClass()->GetAuthoritativeClass());
			}
		}

		// STAGE XII: Recompile every blueprint
		//编译第十二阶段：重新编译我们的每个蓝图，这里会重新编译我们蓝图的GeneratedClass，我们还需要编译我们类的层级
		//第十二阶段是我们的第二个核心编译阶段
		//拿到我们的编译环境后就开始编译我们的类层级，最重要的是编译我们的函数
		bGeneratedClassLayoutReady = false;
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			UBlueprint* BP = CompilerData.BP;
			if (CompilerData.ShouldCompileClassLayout())
			{
				DECLARE_SCOPE_HIERARCHICAL_COUNTER(CompileClassLayout)

				ensure(BP->GeneratedClass == nullptr ||
					BP->GeneratedClass->ClassDefaultObject == nullptr ||
					BP->GeneratedClass->ClassDefaultObject->GetClass() != BP->GeneratedClass);
				// default value propagation occurs in ReinstaneBatch, CDO will be created via CompileFunctions call:
				if (BP->ParentClass)
				{
					if (BP->GeneratedClass && !CompilerData.IsCppCompileType())
					{
						BP->GeneratedClass->ClassDefaultObject = nullptr;
					}

					// Reset the flag, so if the user tries to use PIE it will warn them if the BP did not compile
					BP->bDisplayCompilePIEWarning = true;

					//拿到我们的编译环境后就开始编译我们的类层级，最重要的是编译我们的函数
					FKismetCompilerContext& CompilerContext = *(CompilerData.Compiler);
					CompilerContext.CompileClassLayout(EInternalCompilerFlags::PostponeLocalsGenerationUntilPhaseTwo);
					//CompileClassLayout KismetCompiler.cpp 3903行
					{
						/** Compile a blueprint into a class and a set of functions */
						void FKismetCompilerContext::CompileClassLayout(EInternalCompilerFlags InternalFlags)
						{
							PreCompile();

							// Interfaces only need function signatures, so we only need to perform the first phase of compilation for them
							bIsFullCompile = CompileOptions.DoesRequireBytecodeGeneration() && (Blueprint->BlueprintType != BPTYPE_Interface);

							CallsIntoUbergraph.Empty();
							if (bIsFullCompile)
							{
								Blueprint->IntermediateGeneratedGraphs.Empty();
							}

							// This flag tries to ensure that component instances will use their template name (since that's how old->new instance mapping is done here)
							//@TODO: This approach will break if and when we multithread compiling, should be an inc-dec pair instead
							TGuardValue<bool> GuardTemplateNameFlag(GCompilingBlueprint, true);

							if (Schema == NULL)
							{
								BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_CreateSchema);
								Schema = CreateSchema();
								PostCreateSchema();
							}

							// Make sure the parent class exists and can be used
							check(Blueprint->ParentClass && Blueprint->ParentClass->GetPropertiesSize());

							UClass* TargetUClass = Blueprint->GeneratedClass;

							// >>> Backwards Compatibility:  Make sure this is an actual UBlueprintGeneratedClass / UAnimBlueprintGeneratedClass, as opposed to the old UClass
							EnsureProperGeneratedClass(TargetUClass);
							// <<< End Backwards Compatibility

							TargetClass = Cast<UBlueprintGeneratedClass>(TargetUClass);

							// >>> Backwards compatibility:  If SkeletonGeneratedClass == GeneratedClass, we need to make a new generated class the first time we need it
							if (Blueprint->SkeletonGeneratedClass == Blueprint->GeneratedClass)
							{
								Blueprint->GeneratedClass = NULL;
								TargetClass = NULL;
							}
							// <<< End Backwards Compatibility

							if (!TargetClass)
							{
								FName NewSkelClassName, NewGenClassName;
								Blueprint->GetBlueprintClassNames(NewGenClassName, NewSkelClassName);
								SpawnNewClass(NewGenClassName.ToString());
								check(NewClass);

								TargetClass = NewClass;

								// Fix up the reference in the blueprint to the new class
								Blueprint->GeneratedClass = TargetClass;
							}

							// Early validation
							if (CompileOptions.CompileType == EKismetCompileType::Full)
							{
								TArray<UEdGraph*> AllGraphs;
								Blueprint->GetAllGraphs(AllGraphs);
								for (UEdGraph* Graph : AllGraphs)
								{
									if (Graph)
									{
										TArray<UK2Node*> AllNodes;
										Graph->GetNodesOfClass(AllNodes);
										for (UK2Node* Node : AllNodes)
										{
											if (Node)
											{
												Node->EarlyValidation(MessageLog);
											}
										}
									}
								}
							}

							// Ensure that member variable names are valid and that there are no collisions with a parent class
							// This validation requires CDO object.
							//进行基础验证，验证名字的时效性，确保名字和父类没有冲突
							ValidateVariableNames();

							ValidateComponentClassOverrides();

							OldCDO = NULL;
							OldGenLinkerIdx = INDEX_NONE;
							OldLinker = Blueprint->GetLinker();

							if (OldLinker)
							{
								// Cache linker addresses so we can fixup linker for old CDO
								for (int32 i = 0; i < OldLinker->ExportMap.Num(); i++)
								{
									FObjectExport& ThisExport = OldLinker->ExportMap[i];
									if (ThisExport.ObjectFlags & RF_ClassDefaultObject)
									{
										OldGenLinkerIdx = i;
										break;
									}
								}
							}

							for (int32 TimelineIndex = 0; TimelineIndex < Blueprint->Timelines.Num(); )
							{
								if (NULL == Blueprint->Timelines[TimelineIndex])
								{
									Blueprint->Timelines.RemoveAt(TimelineIndex);
									continue;
								}
								++TimelineIndex;
							}


							CleanAndSanitizeClass(TargetClass, OldCDO);

							NewClass->ClassGeneratedBy = Blueprint;

							// Set class metadata as needed
							UClass* ParentClass = NewClass->GetSuperClass();
							NewClass->ClassFlags |= (ParentClass->ClassFlags & CLASS_Inherit);
							NewClass->ClassCastFlags |= ParentClass->ClassCastFlags;

							if (FBlueprintEditorUtils::IsInterfaceBlueprint(Blueprint))
							{
								TargetClass->ClassFlags |= CLASS_Interface;
							}

							if (Blueprint->bGenerateConstClass)
							{
								NewClass->ClassFlags |= CLASS_Const;
							}

							if (CompileOptions.CompileType == EKismetCompileType::Full)
							{
								UInheritableComponentHandler* InheritableComponentHandler = Blueprint->GetInheritableComponentHandler(false);
								if (InheritableComponentHandler)
								{
									InheritableComponentHandler->ValidateTemplates();
								}
							}

							{
								// the following calls may mark the blueprint as dirty, but we know that these operations just cleaned up the BP 
								// so dependencies can still be considered 'up to date'
								TGuardValue<bool> LockDependenciesUpToDate(Blueprint->bCachedDependenciesUpToDate, Blueprint->bCachedDependenciesUpToDate);

								//验证
								//1.请确保此蓝图与其父函数保持同步，并且确保对父函数的调用是有效的，如果不是，则删除他
								//2.确保我们处理的所有事件都存在，如果没有，则用自定义事件替换
								//3.确保我们实现的所有接口的所有graph都存在，如果没有，则添加
								// Make sure that this blueprint is up-to-date with regards to its parent functions
								FBlueprintEditorUtils::ConformCallsToParentFunctions(Blueprint);

								// Conform implemented events here, to ensure we generate custom events if necessary after reparenting
								FBlueprintEditorUtils::ConformImplementedEvents(Blueprint);

								// Conform implemented interfaces here, to ensure we generate all functions required by the interface as stubs
								FBlueprintEditorUtils::ConformImplementedInterfaces(Blueprint);
							}


							// Run thru the class defined variables first, get them registered
							//在我们的蓝图数组为每一个蓝图类变量创建一个条目
							CreateClassVariablesFromBlueprint();

							// Add any interfaces that the blueprint implements to the class
							// (has to happen before we validate pin links in CreateFunctionList(), so that we can verify self/interface pins)
							//将实现的接口信息添加到类
							AddInterfacesFromBlueprint(NewClass);

							// Construct a context for each function, doing validation and building the function interface
							//为每个函数构造FkismetFunctionContext与之对应，进行验证并构建函数接口
							CreateFunctionList();
							//KismetCompiler.cpp 3840行
							{
								// Performs initial validation that the graph is at least well formed enough to be processed further
								// Merge separate pages of the ubergraph together into one ubergraph
								// Creates a copy of the graph to allow further transformations to occur
								void FKismetCompilerContext::CreateFunctionList()
								{
									//进行初步验证，证明该Graph至少已经形成，可以进一步处理
									//将独立的ubergraph合并成为一个ubergraph
									//创建graph的副本，以允许发生进一步的变换
									{
										BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_GenerateFunctionGraphs);

										// Allow blueprint extensions for the blueprint to generate function graphs
										for (UBlueprintExtension* Extension : Blueprint->Extensions)
										{
											Extension->GenerateFunctionGraphs(this);
										}
									}

									BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_CreateFunctionList);

									// Process the ubergraph if one should be present
									if (FBlueprintEditorUtils::DoesSupportEventGraphs(Blueprint))
									{
										//从ubergraph开始合并图
										CreateAndProcessUbergraph();
										//KismetCompiler.cpp 3278行
										{
											// Merges pages and creates function stubs, etc... from the ubergraph entry points
											void FKismetCompilerContext::CreateAndProcessUbergraph()
											{
												BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_ProcessUbergraph);

												ConsolidatedEventGraph = NewObject<UEdGraph>(Blueprint, GetUbergraphCallName());
												ConsolidatedEventGraph->Schema = UEdGraphSchema_K2::StaticClass();
												ConsolidatedEventGraph->SetFlags(RF_Transient);

												// Merge all of the top-level pages
												MergeUbergraphPagesIn(ConsolidatedEventGraph);

												// Loop over implemented interfaces, and add dummy event entry points for events that aren't explicitly handled by the user
												TArray<UK2Node_Event*> EntryPoints;
												ConsolidatedEventGraph->GetNodesOfClass(EntryPoints);

												for (int32 i = 0; i < Blueprint->ImplementedInterfaces.Num(); i++)
												{
													const FBPInterfaceDescription& InterfaceDesc = Blueprint->ImplementedInterfaces[i];
													for (TFieldIterator<UFunction> FunctionIt(InterfaceDesc.Interface, EFieldIteratorFlags::IncludeSuper); FunctionIt; ++FunctionIt)
													{
														const UFunction* Function = *FunctionIt;
														const FName FunctionName = Function->GetFName();

														const bool bCanImplementAsEvent = UEdGraphSchema_K2::FunctionCanBePlacedAsEvent(Function);
														bool bExistsAsGraph = false;

														// Any function that can be implemented as an event needs to check to see if there is already an interface function graph
														// If there is, we want to warn the user that this is unexpected but proceed to successfully compile the Blueprint
														if (bCanImplementAsEvent)
														{
															for (UEdGraph* InterfaceGraph : InterfaceDesc.Graphs)
															{
																if (InterfaceGraph->GetFName() == Function->GetFName())
																{
																	bExistsAsGraph = true;

																	// Having an event override implemented as a function won't cause issues but is something the user should be aware of.
																	MessageLog.Warning(TEXT("Interface '@@' is already implemented as a function graph but is expected as an event. Remove the function graph and reimplement as an event."), InterfaceGraph);
																}
															}
														}

														// If this is an event, check the merged ubergraph to make sure that it has an event handler, and if not, add one
														if (bCanImplementAsEvent && UEdGraphSchema_K2::CanKismetOverrideFunction(Function) && !bExistsAsGraph)
														{
															bool bFoundEntry = false;
															// Search the cached entry points to see if we have a match
															for (int32 EntryIndex = 0; EntryIndex < EntryPoints.Num(); ++EntryIndex)
															{
																const UK2Node_Event* EventNode = EntryPoints[EntryIndex];
																if (EventNode && (EventNode->EventReference.GetMemberName() == FunctionName))
																{
																	bFoundEntry = true;
																	break;
																}
															}

															if (!bFoundEntry)
															{
																// Create an entry node stub, so that we have a entry point for interfaces to call to
																UK2Node_Event* EventNode = SpawnIntermediateEventNode<UK2Node_Event>(nullptr, nullptr, ConsolidatedEventGraph);
																EventNode->EventReference.SetExternalMember(FunctionName, InterfaceDesc.Interface);
																EventNode->bOverrideFunction = true;
																EventNode->AllocateDefaultPins();
															}
														}
													}
												}

												// We need to stop the old EventGraphs from having the Blueprint as an outer, it impacts renaming.
												if (!Blueprint->HasAnyFlags(RF_NeedLoad | RF_NeedPostLoad))
												{
													for (UEdGraph* OldEventGraph : Blueprint->EventGraphs)
													{
														if (OldEventGraph)
														{
															OldEventGraph->Rename(NULL, GetTransientPackage(), (Blueprint->bIsRegeneratingOnLoad) ? REN_ForceNoResetLoaders : 0);
														}
													}
												}
												Blueprint->EventGraphs.Empty();

												if (ConsolidatedEventGraph->Nodes.Num())
												{
													// Add a dummy entry point to the uber graph, to get the function signature correct
													{
														UK2Node_FunctionEntry* EntryNode = SpawnIntermediateNode<UK2Node_FunctionEntry>(NULL, ConsolidatedEventGraph);
														EntryNode->FunctionReference.SetExternalMember(UEdGraphSchema_K2::FN_ExecuteUbergraphBase, UObject::StaticClass());
														EntryNode->CustomGeneratedFunctionName = ConsolidatedEventGraph->GetFName();
														EntryNode->AllocateDefaultPins();
													}

													// Expand out nodes that need it
													ExpansionStep(ConsolidatedEventGraph, true);

													// If a function in the graph cannot be overridden/placed as event make sure that it is not.
													VerifyValidOverrideEvent(ConsolidatedEventGraph);

													// Do some cursory validation (pin types match, inputs to outputs, pins never point to their parent node, etc...)
													{
														//创建FKismetFunctionContext，记录需要编译的函数以及属性链，但是并不完整，后面还需要对该类型进行注册
														UbergraphContext = new FKismetFunctionContext(MessageLog, Schema, NewClass, Blueprint, CompileOptions.DoesRequireCppCodeGeneration());
														FunctionList.Add(UbergraphContext);
														UbergraphContext->SourceGraph = ConsolidatedEventGraph;
														UbergraphContext->MarkAsEventGraph();
														UbergraphContext->MarkAsInternalOrCppUseOnly();
														UbergraphContext->SetExternalNetNameMap(&ClassScopeNetNameMap);

														// Validate all the nodes in the graph
														for (int32 ChildIndex = 0; ChildIndex < ConsolidatedEventGraph->Nodes.Num(); ++ChildIndex)
														{
															const UEdGraphNode* Node = ConsolidatedEventGraph->Nodes[ChildIndex];
															const int32 SavedErrorCount = MessageLog.NumErrors;
															UK2Node_Event* SrcEventNode = Cast<UK2Node_Event>(ConsolidatedEventGraph->Nodes[ChildIndex]);
															if (bIsFullCompile)
															{
																// We only validate a full compile, we want to always make a function stub so we can display the errors for it later
																ValidateNode(Node);
															}

															// If the node didn't generate any errors then generate function stubs for event entry nodes etc.
															if ((SavedErrorCount == MessageLog.NumErrors) && SrcEventNode)
															{
																CreateFunctionStubForEvent(SrcEventNode, Blueprint);
															}
														}
													}
												}
											}
										}
									}

									if (Blueprint->BlueprintType != BPTYPE_MacroLibrary)
									{
										// Ensure that function graph names are valid and that there are no collisions with a parent class
										//ValidateFunctionGraphNames();

										// Run thru the individual function graphs
										for (int32 i = 0; i < Blueprint->FunctionGraphs.Num(); ++i)
										{
											ProcessOneFunctionGraph(Blueprint->FunctionGraphs[i]);
										}

										for (UEdGraph* FunctionGraph : GeneratedFunctionGraphs)
										{
											ProcessOneFunctionGraph(FunctionGraph);
										}

										for (int32 i = 0; i < Blueprint->DelegateSignatureGraphs.Num(); ++i)
										{
											// change function names to unique

											ProcessOneFunctionGraph(Blueprint->DelegateSignatureGraphs[i]);
										}

										// Run through all the implemented interface member functions
										for (int32 i = 0; i < Blueprint->ImplementedInterfaces.Num(); ++i)
										{
											for (int32 j = 0; j < Blueprint->ImplementedInterfaces[i].Graphs.Num(); ++j)
											{
												UEdGraph* SourceGraph = Blueprint->ImplementedInterfaces[i].Graphs[j];
												ProcessOneFunctionGraph(SourceGraph);
											}
										}
									}
								}
							}

							// Precompile the functions
							// Handle delegates signatures first, because they are needed by other functions
							//拿到我们FunctionList后开始我们的函数预编译阶段
							for (int32 i = 0; i < FunctionList.Num(); ++i)
							{
								if (FunctionList[i].IsDelegateSignature())
								{
									PrecompileFunction(FunctionList[i], InternalFlags);
								}
							}

							for (int32 i = 0; i < FunctionList.Num(); ++i)
							{
								if (!FunctionList[i].IsDelegateSignature())
								{
									PrecompileFunction(FunctionList[i], InternalFlags);
									//KismetCompiler.cpp 1619行
									//函数预编译阶段（函数编译的第一阶段）
									//这个阶段我们开支编译函数图表
									//这个阶段我们需要让graph包含被连接的位置和函数Entry point
									//基于数据依赖性计划每个节点的执行
									//创建一个UFunction对象让他包含参数和局部变量，注意当前还不是脚本代码
									{
										/**
										 * First phase of compiling a function graph
										 *   - Prunes the 'graph' to only included the connected portion that contains the function entry point
										 *   - Schedules execution of each node based on data dependencies
										 *   - Creates a UFunction object containing parameters and local variables (but no script code yet)
										 */
										void FKismetCompilerContext::PrecompileFunction(FKismetFunctionContext& Context, EInternalCompilerFlags InternalFlags)
										{
											BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_PrecompileFunction);

											const bool bImmediatelyGenerateLocals = !(InternalFlags & EInternalCompilerFlags::PostponeLocalsGenerationUntilPhaseTwo);

											// Find the root node, which will drive everything else
											TArray<UK2Node_FunctionEntry*> EntryPoints;
											Context.SourceGraph->GetNodesOfClass(EntryPoints);

											if (EntryPoints.Num())
											{
												Context.EntryPoint = EntryPoints[0];

												// Make sure there was only one function entry node
												for (int32 i = 1; i < EntryPoints.Num(); ++i)
												{
													MessageLog.Error(
														*LOCTEXT("ExpectedOneFunctionEntry_Error", "Expected only one function entry node in graph @@, but found both @@ and @@").ToString(),
														Context.SourceGraph,
														Context.EntryPoint,
														EntryPoints[i]
													);
												}

												{
													TArray<UEdGraphNode*> RootSet;
													const bool bIncludePotentialRootNodes = false;
													// Find any all entry points caused by special nodes
													GatherRootSet(Context.SourceGraph, RootSet, bIncludePotentialRootNodes);

													// Find the connected subgraph starting at the root node and prune out unused nodes
													//将逻辑链中没有被连接的蓝图去掉
													PruneIsolatedNodes(RootSet, Context.SourceGraph->Nodes);
												}

												if (bIsFullCompile)
												{
													// Check if self pins are connected and types are resolved after PruneIsolatedNodes, to avoid errors from isolated nodes.
													//验证self pin是否有连接，并且验证有没有通配符在graph
													ValidateSelfPinsInGraph(Context);
													ValidateNoWildcardPinsInGraph(Context.SourceGraph);

													// Transforms
													TransformNodes(Context);
												}

												// Create the function stub
												FName NewFunctionName = (Context.EntryPoint->CustomGeneratedFunctionName != NAME_None) ? Context.EntryPoint->CustomGeneratedFunctionName : Context.EntryPoint->FunctionReference.GetMemberName();
												if (Context.IsDelegateSignature())
												{
													// prefix with the the blueprint name to avoid conflicts with natively defined delegate signatures
													FString Name = NewFunctionName.ToString();
													Name += HEADER_GENERATED_DELEGATE_SIGNATURE_SUFFIX;
													NewFunctionName = FName(*Name);
												}

												// Determine if this is a new function or if it overrides a parent function
													//@TODO: Does not support multiple overloads for a parent virtual function
												UClass* SuperClass = Context.NewClass->GetSuperClass();
												UFunction* ParentFunction = Context.NewClass->GetSuperClass()->FindFunctionByName(NewFunctionName);

												const FString NewFunctionNameString = NewFunctionName.ToString();
												if (CreatedFunctionNames.Contains(NewFunctionNameString))
												{
													MessageLog.Error(
														*FText::Format(
															LOCTEXT("DuplicateFunctionName_ErrorFmt", "Found more than one function with the same name {0}; second occurance at @@"),
															FText::FromString(NewFunctionNameString)
														).ToString(),
														Context.EntryPoint
													);
													return;
												}
												else if (NULL != FindField<UProperty>(NewClass, NewFunctionName))
												{
													MessageLog.Error(
														*FText::Format(
															LOCTEXT("DuplicateFieldName_ErrorFmt", "Name collision - function and property have the same name - '{0}'. @@"),
															FText::FromString(NewFunctionNameString)
														).ToString(),
														Context.EntryPoint
													);
													return;
												}
												else
												{
													CreatedFunctionNames.Add(NewFunctionNameString);
												}

												//验证函数ubergraph名字是不是代理
												//验证函数ubergraph名字是不是和基类的名字重了
												//生成函数，该函数是ubergraph函数
												Context.Function = NewObject<UFunction>(NewClass, NewFunctionName, RF_Public);

											#if USE_TRANSIENT_SKELETON
												// Propagate down transient settings from the class
												if (NewClass->HasAnyFlags(RF_Transient))
												{
													Context.Function->SetFlags(RF_Transient);
												}
											#endif

												Context.Function->SetSuperStruct(ParentFunction);
												Context.Function->ReturnValueOffset = MAX_uint16;
												Context.Function->FirstPropertyToInit = NULL;

												// Set up the function category
												FKismetUserDeclaredFunctionMetadata& FunctionMetaData = Context.EntryPoint->MetaData;
												if (!FunctionMetaData.Category.IsEmpty())
												{
													Context.Function->SetMetaData(FBlueprintMetadata::MD_FunctionCategory, *FunctionMetaData.Category.ToString());
												}

												// Set up the function keywords
												if (!FunctionMetaData.Keywords.IsEmpty())
												{
													Context.Function->SetMetaData(FBlueprintMetadata::MD_FunctionKeywords, *FunctionMetaData.Keywords.ToString());
												}

												// Set up the function compact node title
												if (!FunctionMetaData.CompactNodeTitle.IsEmpty())
												{
													Context.Function->SetMetaData(FBlueprintMetadata::MD_CompactNodeTitle, *FunctionMetaData.CompactNodeTitle.ToString());
												}

												// Set up the function tooltip
												if (!FunctionMetaData.ToolTip.IsEmpty())
												{
													Context.Function->SetMetaData(FBlueprintMetadata::MD_Tooltip, *FunctionMetaData.ToolTip.ToString());
												}

												// Set as call in editor function
												if (FunctionMetaData.bCallInEditor)
												{
													Context.Function->SetMetaData(FBlueprintMetadata::MD_CallInEditor, TEXT("true"));
												}

												// Set appropriate metadata if the function is deprecated
												if (FunctionMetaData.bIsDeprecated)
												{
													Context.Function->SetMetaData(FBlueprintMetadata::MD_DeprecatedFunction, TEXT("true"));
													if (!FunctionMetaData.DeprecationMessage.IsEmpty())
													{
														Context.Function->SetMetaData(FBlueprintMetadata::MD_DeprecationMessage, *FunctionMetaData.DeprecationMessage);
													}
												}

												// Set the required function flags
												if (Context.CanBeCalledByKismet())
												{
													Context.Function->FunctionFlags |= FUNC_BlueprintCallable;
												}

												if (Context.IsInterfaceStub())
												{
													Context.Function->FunctionFlags |= FUNC_BlueprintEvent;
												}

												// Inherit extra flags from the entry node
												if (Context.EntryPoint)
												{
													Context.Function->FunctionFlags |= (EFunctionFlags)Context.EntryPoint->GetExtraFlags();

													if (UEdGraphPin* WorldContextPin = Context.EntryPoint->GetAutoWorldContextPin())
													{
														Context.Function->SetMetaData(FBlueprintMetadata::MD_WorldContext, *WorldContextPin->PinName.ToString());
													}
												}

												// First try to get the overriden function from the super class
												UFunction* OverridenFunction = Context.Function->GetSuperFunction();
												// If we couldn't find it, see if we can find an interface class in our inheritance to get it from
												if (!OverridenFunction && Context.Blueprint)
												{
													bool bInvalidInterface = false;
													OverridenFunction = FBlueprintEditorUtils::FindFunctionInImplementedInterfaces(Context.Blueprint, Context.Function->GetFName(), &bInvalidInterface);
													if (bInvalidInterface)
													{
														MessageLog.Warning(TEXT("Blueprint tried to implement invalid interface."));
													}
												}

												// Inherit flags and validate against overridden function if it exists
												if (OverridenFunction)
												{
													Context.Function->FunctionFlags |= (OverridenFunction->FunctionFlags & (FUNC_FuncInherit | FUNC_Public | FUNC_Protected | FUNC_Private | FUNC_BlueprintPure));

													if ((Context.Function->FunctionFlags & FUNC_AccessSpecifiers) != (OverridenFunction->FunctionFlags & FUNC_AccessSpecifiers))
													{
														MessageLog.Error(*LOCTEXT("IncompatibleAccessSpecifier_Error", "Access specifier is not compatible the parent function @@").ToString(), Context.EntryPoint);
													}

													const uint32 OverrideFlagsToCheck = (FUNC_FuncOverrideMatch & ~FUNC_AccessSpecifiers);
													if ((Context.Function->FunctionFlags & OverrideFlagsToCheck) != (OverridenFunction->FunctionFlags & OverrideFlagsToCheck))
													{
														MessageLog.Error(*LOCTEXT("IncompatibleOverrideFlags_Error", "Overriden function is not compatible with the parent function @@. Check flags: Exec, Final, Static.").ToString(), Context.EntryPoint);
													}

													// Copy metadata from parent function as well
													UMetaData::CopyMetadata(OverridenFunction, Context.Function);
												}
												else
												{
													// If this is the root of a blueprint-defined function or event, and if it's public, make it overrideable
													if (!Context.IsEventGraph() && !Context.Function->HasAnyFunctionFlags(FUNC_Private))
													{
														Context.Function->FunctionFlags |= FUNC_BlueprintEvent;
													}
												}

												// Link it
												//@TODO: should this be in regular or reverse order?
												//对函数的一些基础注册，比如设置其父类，函数标签，元数据等
												//获取父类的重写函数，验证重写函数是否有效，如果是该ubergraph
												//有父类的ubergraph，那么函数标签和元数据按着父类的来

												//开始连接

												//我们开始注册我们的NewClass
												Context.Function->Next = Context.NewClass->Children;
												Context.NewClass->Children = Context.Function;

												//把函数添加到Funcmap里边为后面做准备
												// Add the function to it's owner class function name -> function map
												Context.NewClass->AddFunctionToFunctionMap(Context.Function, Context.Function->GetFName());
												if (UsePersistentUberGraphFrame() && Context.bIsUbergraph)
												{
													ensure(!NewClass->UberGraphFunction);
													NewClass->UberGraphFunction = Context.Function;//拿到ubergraphFunction
													NewClass->UberGraphFunction->FunctionFlags |= FUNC_UbergraphFunction;
													NewClass->UberGraphFunction->FunctionFlags |= FUNC_Final;
												}

												// Register nets from function entry/exit nodes first, even for skeleton compiles (as they form the signature)
												// We're violating the FNodeHandlingFunctor abstraction here because we want to make sure that the signature
												// matches even if all result nodes were pruned:
												bool bReturnNodeFound = false;
												for (UEdGraphNode* Node : Context.SourceGraph->Nodes)
												{
													if (Node->IsA(UK2Node_FunctionResult::StaticClass()))
													{
														bReturnNodeFound = true;
													}

													if (FNodeHandlingFunctor* Handler = NodeHandlers.FindRef(Node->GetClass()))
													{
														if (Handler->RequiresRegisterNetsBeforeScheduling())
														{
															//注册网络
															Handler->RegisterNets(Context, Node);
														}
													}
												}

												if (!bReturnNodeFound &&
													!Context.IsEventGraph() &&
													!Context.bIsSimpleStubGraphWithNoParams &&
													Context.CanBeCalledByKismet() &&
													Context.Function->GetFName() != UEdGraphSchema_K2::FN_UserConstructionScript)
												{
													// dig into the (actual) source graph and find the original return node:
													UObject* Object = Context.MessageLog.FindSourceObject(Context.SourceGraph);
													if (Object)
													{
														UEdGraph* RealSourceGraph = Cast<UEdGraph>(Object);
														if (RealSourceGraph)
														{
															TArray<UK2Node_FunctionResult*> ResultNodes;
															RealSourceGraph->GetNodesOfClass<UK2Node_FunctionResult>(ResultNodes);
															if (ResultNodes.Num() > 0)
															{
																// Use whatever signature the first result node specifies:
																UK2Node_FunctionResult* FirstResultNode = ResultNodes[0];
																if (FNodeHandlingFunctor* Handler = NodeHandlers.FindRef(UK2Node_FunctionResult::StaticClass()))
																{
																	if (Handler->RequiresRegisterNetsBeforeScheduling())
																	{
																		Handler->RegisterNets(Context, FirstResultNode);
																	}
																}

																// We can't reliably warn here because FBlueprintGraphActionDetails::OnAddNewOutputClicked calls 
																// OnParamsChanged immediately after adding a param to a single node, so only the first result node
																// is guaranteed to be coherent/up to date.. For now we just rely on the editor to make uniform 
																// result nodes.
															}
														}
													}
												}

												//为我们当前的Uber函数开始创建参数
												UField** FunctionPropertyStorageLocation = &(Context.Function->Children);

												// Create input/output parameter variables, this must occur before registering nets so that the properties are in place
												CreateParametersForFunction(Context, ParentFunction ? ParentFunction : OverridenFunction, FunctionPropertyStorageLocation);
												//KismetCompiler.cpp 1019行
												{
													void FKismetCompilerContext::CreateParametersForFunction(FKismetFunctionContext& Context, UFunction* ParameterSignature, UField**& FunctionPropertyStorageLocation)
													{
														//创建输入参数和输出参数
														//通过端子（FBPTerminal）来连接我们的参数链表，以及调试的信息
														const bool bArePropertiesLocal = true;
														CreatePropertiesFromList(Context.Function, FunctionPropertyStorageLocation, Context.Parameters, CPF_Parm | CPF_BlueprintVisible | CPF_BlueprintReadOnly, bArePropertiesLocal, /*bPropertiesAreParameters=*/ true);
														CreatePropertiesFromList(Context.Function, FunctionPropertyStorageLocation, Context.Results, CPF_Parm | CPF_OutParm, bArePropertiesLocal, /*bPropertiesAreParameters=*/ true);

														//MAKE SURE THE PARAMETERS ORDER MATCHES THE OVERRIDEN FUNCTION
														if (ParameterSignature)
														{
															UField** CurrentFieldStorageLocation = &Context.Function->Children;
															for (TFieldIterator<UProperty> SignatureIt(ParameterSignature); SignatureIt && ((SignatureIt->PropertyFlags & CPF_Parm) != 0); ++SignatureIt)
															{
																const FName WantedName = SignatureIt->GetFName();
																if (!*CurrentFieldStorageLocation || (WantedName != (*CurrentFieldStorageLocation)->GetFName()))
																{
																	//Find Field with the proper name
																	UField** FoundFieldStorageLocation = *CurrentFieldStorageLocation ? &((*CurrentFieldStorageLocation)->Next) : nullptr;
																	while (FoundFieldStorageLocation && *FoundFieldStorageLocation && (WantedName != (*FoundFieldStorageLocation)->GetFName()))
																	{
																		FoundFieldStorageLocation = &((*FoundFieldStorageLocation)->Next);
																	}

																	if (FoundFieldStorageLocation && *FoundFieldStorageLocation)
																	{
																		// swap the found field and OverridenIt
																		SwapElementsInSingleLinkedList(*CurrentFieldStorageLocation, *FoundFieldStorageLocation); //FoundFieldStorageLocation points now a random element 
																	}
																	else
																	{
																		MessageLog.Error(
																			*FText::Format(
																				LOCTEXT("WrongParameterOrder_ErrorFmt", "Cannot order parameters {0} in function {1}."),
																				FText::FromName(WantedName),
																				FText::FromString(Context.Function->GetName())
																			).ToString()
																		);
																		break;
																	}
																}

																// Ensure that the 'CPF_UObjectWrapper' flag is propagated through to new parameters, so that wrapper types like 'TSubclassOf' can be preserved if the compiled UFunction is ever nativized.
																if (SignatureIt->HasAllPropertyFlags(CPF_UObjectWrapper))
																{
																	CastChecked<UProperty>(*CurrentFieldStorageLocation)->SetPropertyFlags(CPF_UObjectWrapper);
																}

																CurrentFieldStorageLocation = &((*CurrentFieldStorageLocation)->Next);
															}
															FunctionPropertyStorageLocation = CurrentFieldStorageLocation;

															// There is no guarantee that CurrentFieldStorageLocation points the last parameter's next. We need to ensure that.
															while (*FunctionPropertyStorageLocation)
															{
																FunctionPropertyStorageLocation = &((*FunctionPropertyStorageLocation)->Next);
															}
														}
													}
												}
												if (bImmediatelyGenerateLocals)
												{
													//局部变量注册到网络
													CreateLocalsAndRegisterNets(Context, FunctionPropertyStorageLocation);
												}
												else
												{
													// Fix up the return value - this used to be done by CreateLocalVariablesForFunction.
													// This should probably be done in CreateParametersForFunction
													const FName RetValName = FName(TEXT("ReturnValue"));
													for (TFieldIterator<UProperty> It(Context.Function); It && (It->PropertyFlags & CPF_Parm); ++It)
													{
														UProperty* Property = *It;
														if ((Property->GetFName() == RetValName) && Property->HasAnyPropertyFlags(CPF_OutParm))
														{
															Property->SetPropertyFlags(CPF_ReturnParm);
														}
													}
												}

												// Validate AccessSpecifier
												const uint32 AccessSpecifierFlag = FUNC_AccessSpecifiers & Context.EntryPoint->GetExtraFlags();
												const bool bAcceptedAccessSpecifier =
													(0 == AccessSpecifierFlag) || (FUNC_Public == AccessSpecifierFlag) || (FUNC_Protected == AccessSpecifierFlag) || (FUNC_Private == AccessSpecifierFlag);
												if (!bAcceptedAccessSpecifier)
												{
													MessageLog.Warning(*LOCTEXT("WrongAccessSpecifier_Error", "Wrong access specifier @@").ToString(), Context.EntryPoint);
												}

												Context.LastFunctionPropertyStorageLocation = FunctionPropertyStorageLocation;
												Context.Function->FunctionFlags |= (EFunctionFlags)Context.GetNetFlags();

												// Parameter list needs to be linked before signatures are compared. 
												//对我们的Uber函数开始连接
												Context.Function->StaticLink(true);

												// Make sure the function signature is valid if this is an override
												if (ParentFunction != nullptr)
												{
													// Verify the signature
													if (!ParentFunction->IsSignatureCompatibleWith(Context.Function))
													{
														FString SignatureClassName("");
														if (const UClass* SignatureClass = Context.EntryPoint->FunctionReference.GetMemberParentClass())
														{
															SignatureClassName = SignatureClass->GetName();
														}
														MessageLog.Error(
															*FText::Format(
																LOCTEXT("OverrideFunctionDifferentSignature_ErrorFmt", "Cannot override '{0}::{1}' at @@ which was declared in a parent with a different signature"),
																FText::FromString(SignatureClassName),
																FText::FromString(NewFunctionNameString)
															).ToString(),
															Context.EntryPoint
														);
													}
													const bool bEmptyCase = (0 == AccessSpecifierFlag);
													const bool bDifferentAccessSpecifiers = AccessSpecifierFlag != (ParentFunction->FunctionFlags & FUNC_AccessSpecifiers);
													if (!bEmptyCase && bDifferentAccessSpecifiers)
													{
														MessageLog.Warning(*LOCTEXT("IncompatibleAccessSpecifier_Error", "Access specifier is not compatible the parent function @@").ToString(), Context.EntryPoint);
													}

													EFunctionFlags const ParentNetFlags = (ParentFunction->FunctionFlags & FUNC_NetFuncFlags);
													if (ParentNetFlags != Context.GetNetFlags())
													{
														MessageLog.Error(*LOCTEXT("MismatchedNetFlags_Error", "@@ function's net flags don't match parent function's flags").ToString(), Context.EntryPoint);

														// clear the existing net flags
														Context.Function->FunctionFlags &= ~(FUNC_NetFuncFlags);
														// have to replace with the parent's net flags, or this will   
														// trigger an assert in Link()
														Context.Function->FunctionFlags |= ParentNetFlags;
													}
												}

												////////////////////////////////////////

												if (Context.IsDelegateSignature())
												{
													Context.Function->FunctionFlags |= FUNC_Delegate;

													if (UMulticastDelegateProperty* Property = Cast<UMulticastDelegateProperty>(StaticFindObjectFast(UMulticastDelegateProperty::StaticClass(), NewClass, Context.DelegateSignatureName)))
													{
														Property->SignatureFunction = Context.Function;
													}
													else
													{
														MessageLog.Warning(*LOCTEXT("NoDelegateProperty_Error", "No delegate property found for @@").ToString(), Context.SourceGraph);
													}
												}

											}
											else
											{
												MessageLog.Error(*LOCTEXT("NoRootNodeFound_Error", "Could not find a root node for the graph @@").ToString(), Context.SourceGraph);
											}
										}
									}
								}
							}

							if (UsePersistentUberGraphFrame() && UbergraphContext)
							{
								//UBER GRAPH PERSISTENT FRAME
								FEdGraphPinType Type(TEXT("struct"), NAME_None, FPointerToUberGraphFrame::StaticStruct(), EPinContainerType::None, false, FEdGraphTerminalType());
								UProperty* Property = CreateVariable(UBlueprintGeneratedClass::GetUberGraphFrameName(), Type);
								Property->SetPropertyFlags(CPF_DuplicateTransient | CPF_Transient);
							}

							{ BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_BindAndLinkClass);

							// Relink the class
							//重新连接NewClass
							NewClass->Bind();
							NewClass->StaticLink(true);
							}
						}
					}
					// We immediately relink children so that iterative compilation logic has an easier time:
					TArray<UClass*> ClassesToRelink;
					GetDerivedClasses(BP->GeneratedClass, ClassesToRelink, false);
					for (UClass* ChildClass : ClassesToRelink)
					{
						ChildClass->Bind();
						ChildClass->StaticLink();
						ensure(ChildClass->ClassDefaultObject == nullptr);
					}
				}
				else
				{
					CompilerData.ActiveResultsLog->Error(*LOCTEXT("KismetCompileError_MalformedParentClasss", "Blueprint @@ has missing or NULL parent class.").ToString(), BP);
				}
			}
			else if (CompilerData.Compiler.IsValid() && BP->GeneratedClass)
			{
				CompilerData.Compiler->SetNewClass(CastChecked<UBlueprintGeneratedClass>(BP->GeneratedClass));
			}
		}
		bGeneratedClassLayoutReady = true;

		ProcessExtensions(CurrentlyCompilingBPs);

		// STAGE XIII: Compile functions
		//编译第十三阶段：编译我们的函数
		UBlueprintEditorSettings* Settings = GetMutableDefault<UBlueprintEditorSettings>();

		const bool bSaveBlueprintsAfterCompile = Settings->SaveOnCompile == SoC_Always;
		const bool bSaveBlueprintAfterCompileSucceeded = Settings->SaveOnCompile == SoC_SuccessOnly;

		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			UBlueprint* BP = CompilerData.BP;
			UClass* BPGC = BP->GeneratedClass;

			if (!CompilerData.ShouldCompileClassFunctions())
			{
				if (BPGC &&
					(BPGC->ClassDefaultObject == nullptr ||
						BPGC->ClassDefaultObject->GetClass() != BPGC))
				{
					// relink, generate CDO:
					BPGC->ClassFlags &= ~CLASS_LayoutChanging;
					BPGC->Bind();
					BPGC->StaticLink(true);
					//CDO
					BPGC->ClassDefaultObject = nullptr;
					BPGC->GetDefaultObject(true);
				}
			}
			else
			{
				DECLARE_SCOPE_HIERARCHICAL_COUNTER(CompileClassFunctions)

				// default value propagation occurs below:
				if (BPGC)
				{
					if (BPGC->ClassDefaultObject &&
						BPGC->ClassDefaultObject->GetClass() == BPGC)
					{
						// the CDO has been created early, it is possible that the reflection data was still
						// being mutated by CompileClassLayout. Warn the user and and move the CDO aside:
						ensureAlwaysMsgf(false,
							TEXT("ClassDefaultObject for %s created at the wrong time - it may be corrupt. It is recommended that you save all data and restart the editor session"),
							*BP->GetName()
						);

						BPGC->ClassDefaultObject->Rename(
							nullptr,
							// destination - this is the important part of this call. Moving the object 
							// out of the way so we can reuse its name:
							GetTransientPackage(),
							// Rename options:
							REN_DoNotDirty | REN_DontCreateRedirectors | REN_ForceNoResetLoaders
						);
					}
					BPGC->ClassDefaultObject = nullptr;

					// class layout is ready, we can clear CLASS_LayoutChanging and CompileFunctions can create the CDO:
					//我们现在要编译我们函数来创建一个新的CDO
					BPGC->ClassFlags &= ~CLASS_LayoutChanging;

					FKismetCompilerContext& CompilerContext = *(CompilerData.Compiler);
					CompilerContext.CompileFunctions(
						EInternalCompilerFlags::PostponeLocalsGenerationUntilPhaseTwo
						| EInternalCompilerFlags::PostponeDefaultObjectAssignmentUntilReinstancing
						| EInternalCompilerFlags::SkipRefreshExternalBlueprintDependencyNodes
					);
					//KismetCompiler.cpp 4104行
					{
						//函数编译阶段
						void FKismetCompilerContext::CompileFunctions(EInternalCompilerFlags InternalFlags)
						{
							// This is phase two, so we want to generated locals if PostponeLocalsGenerationUntilPhaseTwo is set:
							const bool bGenerateLocals = !!(InternalFlags & EInternalCompilerFlags::PostponeLocalsGenerationUntilPhaseTwo);
							// Don't propagate values to CDO if we're going to do that in reinstancing:
							bool bPropagateValuesToCDO = !(InternalFlags & EInternalCompilerFlags::PostponeDefaultObjectAssignmentUntilReinstancing);
							// Don't RefreshExternalBlueprintDependencyNodes if the calling code has done so already:
							const bool bSkipRefreshExternalBlueprintDependencyNodes = !!(InternalFlags & EInternalCompilerFlags::SkipRefreshExternalBlueprintDependencyNodes);
							//VM虚拟机后端编译类
							FKismetCompilerVMBackend Backend_VM(Blueprint, Schema, *this);

							// Determine whether or not to skip generated class validation.
							bool bSkipGeneratedClassValidation;
							if (CompileOptions.DoesRequireCppCodeGeneration())
							{
								// CPP codegen requires default value assignment to occur as part of the compilation phase, so we override it here.
								bPropagateValuesToCDO = true;

								// Also skip generated class validation since it may result in errors and we don't really need to keep the generated class.
								bSkipGeneratedClassValidation = true;
							}
							else
							{
								// In all other cases, validation requires CDO value propagation to occur first.
								bSkipGeneratedClassValidation = !bPropagateValuesToCDO;
							}

							if (bGenerateLocals)
							{
								for (int32 i = 0; i < FunctionList.Num(); ++i)
								{
									if (FunctionList[i].IsValid())
									{
										//为我们当前的Uber函数创建局部变量，如果有网络需求就注册到网络上
										FKismetFunctionContext& Context = FunctionList[i];
										CreateLocalsAndRegisterNets(Context, Context.LastFunctionPropertyStorageLocation);
									}
								}
							}

							if (bIsFullCompile && !MessageLog.NumErrors)
							{
								// Generate code for each function (done in a second pass to allow functions to reference each other)
								//为我们的每个函数生成代码
								for (int32 i = 0; i < FunctionList.Num(); ++i)
								{
									if (FunctionList[i].IsValid())
									{
										CompileFunction(FunctionList[i]);
										//KismetCompiler.cpp 2023行
										{
											/**
											 * Second phase of compiling a function graph
											 *   - Generates executable code and performs final validation
											 */
											//函数图编译的第二阶段
											//生成可执行代码并执行最终验证
											void FKismetCompilerContext::CompileFunction(FKismetFunctionContext& Context)
											{
												BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_CompileFunction);

												check(Context.IsValid());

												//为我们每个Node生成对应的语句，这个node是逻辑的根部，root
												// Generate statements for each node in the linear execution order (which should roughly correspond to the final execution order)
												TMap<UEdGraphNode*, int32> SortKeyMap;
												int32 NumNodesAtStart = Context.LinearExecutionList.Num();
												for (int32 i = 0; i < Context.LinearExecutionList.Num(); ++i)
												{
													UEdGraphNode* Node = Context.LinearExecutionList[i];
													SortKeyMap.Add(Node, i);

													const FString NodeComment = Node->NodeComment.IsEmpty() ? Node->GetName() : Node->NodeComment;
													const bool bPureNode = IsNodePure(Node);
													// Debug comments
													if (KismetCompilerDebugOptions::EmitNodeComments && !Context.bGeneratingCpp)
													{
														FBlueprintCompiledStatement& Statement = Context.AppendStatementForNode(Node);
														Statement.Type = KCST_Comment;
														Statement.Comment = NodeComment;
													}

													// Debug opcode insertion point
													if (Context.IsDebuggingOrInstrumentationRequired())
													{
														if (!bPureNode)
														{
															UEdGraphPin* ExecPin = nullptr;
															bool bEmitDebuggingSite = true;

															if (Context.IsEventGraph() && (Node->IsA(UK2Node_FunctionEntry::StaticClass())))
															{
																// The entry point in the ubergraph is a non-visual construct, and will lead to some
																// other 'fake' entry point such as an event or latent action.  Therefore, don't create
																// debug data for the behind-the-scenes entry point, only for the user-visible ones.
																bEmitDebuggingSite = false;
															}

															if (bEmitDebuggingSite)
															{
																FBlueprintCompiledStatement& Statement = Context.AppendStatementForNode(Node);
																Statement.Type = Context.GetBreakpointType();
																Statement.ExecContext = ExecPin;
																Statement.Comment = NodeComment;
															}
														}
													}

													// Let the node handlers try to compile it
													//开始执行编译
													if (FNodeHandlingFunctor* Handler = NodeHandlers.FindRef(Node->GetClass()))
													{
														Handler->Compile(Context, Node);
														//CallFunctionHandler.cpp 662行
														{
															void FKCHandler_CallFunction::Compile(FKismetFunctionContext& Context, UEdGraphNode* Node)
															{
																//开始寻找我们的节点，这个节点一定是具有Exec这种输入或者输出端口的
																//因为纯蓝图我们会把它整合到参数里边，单独的不连接到Exec蓝图的纯蓝图是不可执行的
																//验证自引脚是否连接，因为修剪独立节点可能会导致无效的目标
																check(NULL != Node);

																//@TODO: Can probably move this earlier during graph verification instead of compilation, but after island pruning
																if (!IsCalledFunctionPure(Node))
																{
																	// For imperative nodes, make sure the exec function was actually triggered and not just included due to an output data dependency
																	UEdGraphPin* ExecTriggeringPin = CompilerContext.GetSchema()->FindExecutionPin(*Node, EGPD_Input);
																	if (ExecTriggeringPin == NULL)
																	{
																		CompilerContext.MessageLog.Error(*NSLOCTEXT("KismetCompiler", "NoValidExecutionPinForCallFunc_Error", "@@ must have a valid execution pin").ToString(), Node);
																		return;
																	}
																	else if (ExecTriggeringPin->LinkedTo.Num() == 0)
																	{
																		CompilerContext.MessageLog.Warning(*NSLOCTEXT("KismetCompiler", "NodeNeverExecuted_Warning", "@@ will never be executed").ToString(), Node);
																		return;
																	}
																}

																// Validate the self pin again if it is disconnected, because pruning isolated nodes could have caused an invalid target
																UEdGraphPin* SelfPin = CompilerContext.GetSchema()->FindSelfPin(*Node, EGPD_Input);
																if (SelfPin && (SelfPin->LinkedTo.Num() == 0))
																{
																	FEdGraphPinType SelfType;
																	SelfType.PinCategory = UEdGraphSchema_K2::PC_Object;
																	SelfType.PinSubCategory = UEdGraphSchema_K2::PSC_Self;

																	if (!CompilerContext.GetSchema()->ArePinTypesCompatible(SelfType, SelfPin->PinType, Context.NewClass) && (SelfPin->DefaultObject == NULL))
																	{
																		CompilerContext.MessageLog.Error(*NSLOCTEXT("KismetCompiler", "PinMustHaveConnectionPruned_Error", "Pin @@ must have a connection.  Self pins cannot be connected to nodes that are culled.").ToString(), SelfPin);
																	}
																}

																// Make sure the function node is valid to call
																//确保函数节点可以有效地调用
																CreateFunctionCallStatement(Context, Node, SelfPin);
																//CallFunctionHandler.cpp 64行
																{
																	/**
																	 * Searches for the function referenced by a graph node in the CallingContext class's list of functions,
																	 * validates that the wiring matches up correctly, and creates an execution statement.
																	 */
																	//在CallingContext类的函数列表中搜索由图形节点引用的函数
																	//验证连接是否正确匹配，并创建执行语句
																	void FKCHandler_CallFunction::CreateFunctionCallStatement(FKismetFunctionContext& Context, UEdGraphNode* Node, UEdGraphPin* SelfPin)
																	{
																		int32 NumErrorsAtStart = CompilerContext.MessageLog.NumErrors;

																		// Find the function, starting at the parent class
																		//通过具体的节点找到具体的函数
																		if (UFunction* Function = FindFunction(Context, Node))
																		{
																			CheckIfFunctionIsCallable(Function, Context, Node);
																			// Make sure the pin mapping is sound (all pins wire up to a matching function parameter, and all function parameters match a pin)

																			// Remaining unmatched pins
																			TArray<UEdGraphPin*> RemainingPins;
																			RemainingPins.Append(Node->Pins);

																			const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

																			// Remove expected exec and self pins
																			RemainingPins.RemoveAllSwap([Schema](UEdGraphPin* Pin) { return (Pin->bOrphanedPin || Schema->IsMetaPin(*Pin)); }, false);

																			// Check for magic pins
																			const bool bIsLatent = Function->HasMetaData(FBlueprintMetadata::MD_Latent);
																			if (bIsLatent && (CompilerContext.UbergraphContext != &Context))
																			{
																				CompilerContext.MessageLog.Error(*LOCTEXT("ContainsLatentCall_Error", "@@ contains a latent call, which cannot exist outside of the event graph").ToString(), Node);
																			}

																			UEdGraphPin* LatentInfoPin = nullptr;

																			if (TMap<FName, FString>* MetaData = UMetaData::GetMapForObject(Function))
																			{
																				for (TMap<FName, FString>::TConstIterator It(*MetaData); It; ++It)
																				{
																					const FName& Key = It.Key();

																					if (Key == TEXT("LatentInfo"))
																					{
																						UEdGraphPin* Pin = Node->FindPin(It.Value());
																						if (Pin && (Pin->Direction == EGPD_Input) && (Pin->LinkedTo.Num() == 0))
																						{
																							LatentInfoPin = Pin;

																							UEdGraphPin* PinToTry = FEdGraphUtilities::GetNetFromPin(Pin);
																							if (FBPTerminal** Term = Context.NetMap.Find(PinToTry))
																							{
																								check((*Term)->bIsLiteral);

																								int32 LatentUUID = CompilerContext.MessageLog.CalculateStableIdentifierForLatentActionManager(LatentInfoPin->GetOwningNode());

																								const FString ExecutionFunctionName = UEdGraphSchema_K2::FN_ExecuteUbergraphBase.ToString() + TEXT("_") + Context.Blueprint->GetName();
																								(*Term)->Name = FString::Printf(TEXT("(Linkage=%s,UUID=%s,ExecutionFunction=%s,CallbackTarget=None)"), *FString::FromInt(INDEX_NONE), *FString::FromInt(LatentUUID), *ExecutionFunctionName);

																								// Record the UUID in the debugging information
																								UEdGraphNode* TrueSourceNode = Cast<UEdGraphNode>(Context.MessageLog.FindSourceObject(Node));
																								Context.NewClass->GetDebugData().RegisterUUIDAssociation(TrueSourceNode, LatentUUID);
																							}
																						}
																						else
																						{
																							CompilerContext.MessageLog.Error(*FText::Format(LOCTEXT("FindPinFromLinkage_ErrorFmt", "Function {0} (called from @@) was specified with LatentInfo metadata but does not have a pin named {1}"),
																								FText::FromString(Function->GetName()), FText::FromString(It.Value())).ToString(), Node);
																						}
																					}
																				}
																			}

																			// Parameter info to be stored, and assigned to all function call statements generated below
																			//要存储的参数信息，并分配给下面生成的所有函数调用语句
																			FBPTerminal* LHSTerm = nullptr;
																			TArray<FBPTerminal*> RHSTerms;
																			UEdGraphPin* ThenExecPin = nullptr;
																			UEdGraphNode* LatentTargetNode = nullptr;
																			int32 LatentTargetParamIndex = INDEX_NONE;

																			// Grab the special case structs that use their own literal path
																			UScriptStruct* VectorStruct = TBaseStructure<FVector>::Get();
																			UScriptStruct* RotatorStruct = TBaseStructure<FRotator>::Get();
																			UScriptStruct* TransformStruct = TBaseStructure<FTransform>::Get();

																			// Check each property
																			bool bMatchedAllParams = true;
																			for (TFieldIterator<UProperty> It(Function); It && (It->PropertyFlags & CPF_Parm); ++It)
																			{
																				UProperty* Property = *It;

																				bool bFoundParam = false;
																				for (int32 i = 0; !bFoundParam && (i < RemainingPins.Num()); ++i)
																				{
																					UEdGraphPin* PinMatch = RemainingPins[i];
																					if (Property->GetFName() == PinMatch->PinName)
																					{
																						// Found a corresponding pin, does it match in type and direction?
																						if (FKismetCompilerUtilities::IsTypeCompatibleWithProperty(PinMatch, Property, CompilerContext.MessageLog, CompilerContext.GetSchema(), Context.NewClass))
																						{
																							UEdGraphPin* PinToTry = FEdGraphUtilities::GetNetFromPin(PinMatch);

																							if (FBPTerminal** Term = Context.NetMap.Find(PinToTry))
																							{
																								// For literal structs, we have to verify the default here to make sure that it has valid formatting
																								if ((*Term)->bIsLiteral && (PinMatch != LatentInfoPin))
																								{
																									UStructProperty* StructProperty = Cast<UStructProperty>(Property);
																									if (StructProperty)
																									{
																										UScriptStruct* Struct = StructProperty->Struct;
																										if (Struct != VectorStruct
																											&& Struct != RotatorStruct
																											&& Struct != TransformStruct)
																										{
																											// Ensure all literal struct terms can be imported if its empty
																											if ((*Term)->Name.IsEmpty())
																											{
																												(*Term)->Name = TEXT("()");
																											}

																											int32 StructSize = Struct->GetStructureSize();
																											[this, StructSize, StructProperty, Node, Term, &bMatchedAllParams]()
																											{
																												uint8* StructData = (uint8*)FMemory_Alloca(StructSize);
																												StructProperty->InitializeValue(StructData);

																												// Import the literal text to a dummy struct to verify it's well-formed
																												FImportTextErrorContext ErrorPipe(CompilerContext.MessageLog, Node);
																												StructProperty->ImportText(*((*Term)->Name), StructData, 0, nullptr, &ErrorPipe);
																												if (ErrorPipe.NumErrors > 0)
																												{
																													bMatchedAllParams = false;
																												}
																											}();
																										}

																									}
																								}

																								if (Property->HasAnyPropertyFlags(CPF_ReturnParm))
																								{
																									LHSTerm = *Term;
																								}
																								else
																								{
																									FBPTerminal* RHSTerm = *Term;

																									// if this term is an object that needs to be cast to an interface
																									if (FBPTerminal** InterfaceTerm = InterfaceTermMap.Find(PinMatch))
																									{
																										UClass* InterfaceClass = CastChecked<UClass>(PinMatch->PinType.PinSubCategoryObject.Get());

																										FBPTerminal* ClassTerm = Context.CreateLocalTerminal(ETerminalSpecification::TS_Literal);
																										ClassTerm->Name = InterfaceClass->GetName();
																										ClassTerm->bIsLiteral = true;
																										ClassTerm->Source = Node;
																										ClassTerm->ObjectLiteral = InterfaceClass;
																										ClassTerm->Type.PinCategory = UEdGraphSchema_K2::PC_Class;

																										// insert a cast op before a call to the function (and replace
																										// the param with the result from the cast)
																										FBlueprintCompiledStatement& CastStatement = Context.AppendStatementForNode(Node);
																										CastStatement.Type = InterfaceClass->HasAnyClassFlags(CLASS_Interface) ? KCST_CastObjToInterface : KCST_CastInterfaceToObj;
																										CastStatement.LHS = *InterfaceTerm;
																										CastStatement.RHS.Add(ClassTerm);
																										CastStatement.RHS.Add(*Term);

																										RHSTerm = *InterfaceTerm;
																									}

																									int32 ParameterIndex = RHSTerms.Add(RHSTerm);

																									if (PinMatch == LatentInfoPin)
																									{
																										// Record the (latent) output impulse from this node
																										ThenExecPin = CompilerContext.GetSchema()->FindExecutionPin(*Node, EGPD_Output);

																										if (ThenExecPin && (ThenExecPin->LinkedTo.Num() > 0))
																										{
																											LatentTargetNode = ThenExecPin->LinkedTo[0]->GetOwningNode();
																										}

																										if (LatentTargetNode)
																										{
																											LatentTargetParamIndex = ParameterIndex;
																										}
																									}
																								}

																								// Make sure it isn't trying to modify a const term
																								if (Property->HasAnyPropertyFlags(CPF_OutParm) && !((*Term)->IsTermWritable()))
																								{
																									if (Property->HasAnyPropertyFlags(CPF_ReferenceParm))
																									{
																										if (!Property->HasAnyPropertyFlags(CPF_ConstParm))
																										{
																											CompilerContext.MessageLog.Error(*LOCTEXT("PassReadOnlyReferenceParam_Error", "Cannot pass a read-only variable to a reference parameter @@").ToString(), PinMatch);
																										}
																									}
																									else
																									{
																										CompilerContext.MessageLog.Error(*LOCTEXT("PassReadOnlyOutputParam_Error", "Cannot pass a read-only variable to a output parameter @@").ToString(), PinMatch);
																									}
																								}
																							}
																							else
																							{
																								CompilerContext.MessageLog.Error(*LOCTEXT("ResolveTermPassed_Error", "Failed to resolve term passed into @@").ToString(), PinMatch);
																								bMatchedAllParams = false;
																							}
																						}
																						else
																						{
																							bMatchedAllParams = false;
																						}

																						bFoundParam = true;
																						RemainingPins.RemoveAtSwap(i);
																					}
																				}

																				if (!bFoundParam)
																				{
																					CompilerContext.MessageLog.Error(*FText::Format(LOCTEXT("FindPinParameter_ErrorFmt", "Could not find a pin for the parameter {0} of {1} on @@"), FText::FromString(Property->GetName()), FText::FromString(Function->GetName())).ToString(), Node);
																					bMatchedAllParams = false;
																				}
																			}

																			// At this point, we should have consumed all pins.  If not, there are extras that need to be removed.
																			for (int32 i = 0; i < RemainingPins.Num(); ++i)
																			{
																				CompilerContext.MessageLog.Error(*FText::Format(LOCTEXT("PinMismatchParameter_ErrorFmt", "Pin @@ named {0} doesn't match any parameters of function {1}"), FText::FromName(RemainingPins[i]->PinName), FText::FromString(Function->GetName())).ToString(), RemainingPins[i]);
																			}

																			if (NumErrorsAtStart == CompilerContext.MessageLog.NumErrors)
																			{
																				// Build up a list of contexts that this function will be called on
																				TArray<FBPTerminal*> ContextTerms;
																				if (SelfPin)
																				{
																					const bool bIsConstSelfContext = Context.IsConstFunction();
																					const bool bIsNonConstFunction = !Function->HasAnyFunctionFlags(FUNC_Const | FUNC_Static);
																					const bool bEnforceConstCorrectness = Context.EnforceConstCorrectness();
																					auto CheckAndAddSelfTermLambda = [this, &Node, &ContextTerms, bIsConstSelfContext, bIsNonConstFunction, bEnforceConstCorrectness](FBPTerminal* Target)
																					{
																						bool bIsSelfTerm = true;
																						if (Target != nullptr)
																						{
																							const UEdGraphPin* SourcePin = Target->SourcePin;
																							bIsSelfTerm = (SourcePin == nullptr || CompilerContext.GetSchema()->IsSelfPin(*SourcePin));
																						}

																						// Ensure const correctness within the context of the function call:
																						//	a) Attempting to call a non-const, non-static function within a const function graph (i.e. 'const self' as context)
																						//	b) Attempting to call a non-const, non-static function with a 'const' term linked to the target pin as the function context
																						if (bIsSelfTerm && bIsConstSelfContext && bIsNonConstFunction)
																						{
																							// If we're not enforcing const correctness in this context, emit a warning here rather than an error, and allow compilation of this statement to proceed
																							if (Target != nullptr)
																							{
																								if (bEnforceConstCorrectness)
																								{
																									CompilerContext.MessageLog.Error(*LOCTEXT("NonConstFunctionCallOnReadOnlyTarget_Error", "Function @@ can modify state and cannot be called on @@ because it is a read-only Target in this context").ToString(), Node, Target->Source);
																								}
																								else
																								{
																									CompilerContext.MessageLog.Warning(*LOCTEXT("NonConstFunctionCallOnReadOnlyTarget_Warning", "Function @@ can modify state and should not be called on @@ because it is considered to be a read-only Target in this context").ToString(), Node, Target->Source);
																								}
																							}
																							else
																							{
																								if (bEnforceConstCorrectness)
																								{
																									CompilerContext.MessageLog.Error(*LOCTEXT("NonConstFunctionCallOnReadOnlySelfScope_Error", "Function @@ can modify state and cannot be called on 'self' because it is a read-only Target in this context").ToString(), Node);
																								}
																								else
																								{
																									CompilerContext.MessageLog.Warning(*LOCTEXT("NonConstFunctionCallOnReadOnlySelfScope_Warning", "Function @@ can modify state and should not be called on 'self' because it is considered to be a read-only Target in this context").ToString(), Node);
																								}
																							}
																						}

																						ContextTerms.Add(Target);
																					};

																					if (SelfPin->LinkedTo.Num() > 0)
																					{
																						for (int32 i = 0; i < SelfPin->LinkedTo.Num(); i++)
																						{
																							FBPTerminal** pContextTerm = Context.NetMap.Find(SelfPin->LinkedTo[i]);
																							if (ensureMsgf(pContextTerm != nullptr, TEXT("'%s' is missing a target input - if this is a server build, the input may be a cosmetic only property which was discarded (if this is the case, and this is expecting component variable try resaving.)"), *Node->GetPathName()))
																							{
																								CheckAndAddSelfTermLambda(*pContextTerm);
																							}
																						}
																					}
																					else
																					{
																						FBPTerminal** pContextTerm = Context.NetMap.Find(SelfPin);
																						CheckAndAddSelfTermLambda((pContextTerm != nullptr) ? *pContextTerm : nullptr);
																					}
																				}

																				// Check for a call into the ubergraph, which will require a patchup later on for the exact state entry point
																				UEdGraphNode** pSrcEventNode = NULL;
																				if (!bIsLatent)
																				{
																					pSrcEventNode = CompilerContext.CallsIntoUbergraph.Find(Node);
																				}

																				bool bInlineEventCall = false;
																				bool bEmitInstrumentPushState = false;
																				FName EventName = NAME_None;

																				// Iterate over all the contexts this functions needs to be called on, and emit a call function statement for each
																				//为我们每个Node添加对应的语句，详细可以参考这个函数代码
																				FBlueprintCompiledStatement* LatentStatement = nullptr;
																				for (FBPTerminal* Target : ContextTerms)
																				{
																					FBlueprintCompiledStatement& Statement = Context.AppendStatementForNode(Node);
																					Statement.FunctionToCall = Function;
																					Statement.FunctionContext = Target;
																					Statement.Type = KCST_CallFunction;
																					Statement.bIsInterfaceContext = IsCalledFunctionFromInterface(Node);
																					Statement.bIsParentContext = IsCalledFunctionFinal(Node);

																					Statement.LHS = LHSTerm;
																					Statement.RHS = RHSTerms;
																				//当前的阶段整合了我们的所有蓝图节点对应的语句，这个为我们后面的编译做准备
																					if (!bIsLatent)
																					{
																						// Fixup ubergraph calls
																						if (pSrcEventNode)
																						{
																							UEdGraphPin* ExecOut = CompilerContext.GetSchema()->FindExecutionPin(**pSrcEventNode, EGPD_Output);

																							check(CompilerContext.UbergraphContext);
																							CompilerContext.UbergraphContext->GotoFixupRequestMap.Add(&Statement, ExecOut);
																							Statement.UbergraphCallIndex = 0;
																						}
																					}
																					else
																					{
																						// Fixup latent functions
																						if (LatentTargetNode && (Target == ContextTerms.Last()))
																						{
																							check(LatentTargetParamIndex != INDEX_NONE);
																							Statement.UbergraphCallIndex = LatentTargetParamIndex;
																							Context.GotoFixupRequestMap.Add(&Statement, ThenExecPin);
																							LatentStatement = &Statement;
																						}
																					}

																					AdditionalCompiledStatementHandling(Context, Node, Statement);

																					if (Statement.Type == KCST_CallFunction && Function->HasAnyFunctionFlags(FUNC_Delegate))
																					{
																						CompilerContext.MessageLog.Error(*LOCTEXT("CallingDelegate_Error", "@@ is trying to call a delegate function - delegates cannot be called directly").ToString(), Node);
																						// Sanitize the statement, this would have ideally been detected earlier but we need
																						// to run AdditionalCompiledStatementHandling to satisify the DelegateNodeHandler
																						// implementation:
																						Statement.Type = KCST_CallDelegate;
																					}
																				}

																				// Create the exit from this node if there is one
																				if (bIsLatent)
																				{
																					// End this thread of execution; the latent function will resume it at some point in the future
																					FBlueprintCompiledStatement& PopStatement = Context.AppendStatementForNode(Node);
																					PopStatement.Type = KCST_EndOfThread;
																				}
																				else
																				{
																					// Generate the output impulse from this node
																					if (!IsCalledFunctionPure(Node))
																					{
																						GenerateSimpleThenGoto(Context, *Node);
																					}
																				}
																			}
																		}
																		else
																		{
																			FString WarningMessage = FText::Format(LOCTEXT("FindFunction_ErrorFmt", "Could not find the function '{0}' called from @@"), FText::FromString(GetFunctionNameFromNode(Node).ToString())).ToString();
																			CompilerContext.MessageLog.Warning(*WarningMessage, Node);
																		}
																	}
																}
															}
														}
													}
													else
													{
														MessageLog.Error(
															*FText::Format(
																LOCTEXT("UnexpectedNodeTypeWhenCompilingFunc_ErrorFmt", "Unexpected node type {0} encountered in execution chain at @@"),
																FText::FromString(Node->GetClass()->GetName())
															).ToString(),
															Node
														);
													}
												}

												// The LinearExecutionList should be immutable at this point
												check(Context.LinearExecutionList.Num() == NumNodesAtStart);

												// Now pull out pure chains and inline their generated code into the nodes that need it
												TMap< UEdGraphNode*, TSet<UEdGraphNode*> > PureNodesNeeded;

												for (int32 TestIndex = 0; TestIndex < Context.LinearExecutionList.Num(); )
												{
													UEdGraphNode* Node = Context.LinearExecutionList[TestIndex];

													// List of pure nodes this node depends on.
													bool bHasAntecedentPureNodes = PureNodesNeeded.Contains(Node);

													if (IsNodePure(Node))
													{
														// For profiling purposes, find the statement that marks the function's entry point.
														FBlueprintCompiledStatement* ProfilerStatement = nullptr;
														TArray<FBlueprintCompiledStatement*>* SourceStatementList = Context.StatementsPerNode.Find(Node);
														const bool bDidNodeGenerateCode = SourceStatementList != nullptr && SourceStatementList->Num() > 0;
														if (bDidNodeGenerateCode)
														{
															for (FBlueprintCompiledStatement* Statement : *SourceStatementList)
															{
																if (Statement && Statement->Type == KCST_InstrumentedPureNodeEntry)
																{
																	ProfilerStatement = Statement;
																	break;
																}
															}
														}

														// Push this node to the requirements list of any other nodes using it's outputs, if this node had any real impact
														if (bDidNodeGenerateCode || bHasAntecedentPureNodes)
														{
															for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
															{
																UEdGraphPin* Pin = Node->Pins[PinIndex];
																if (Pin->Direction == EGPD_Output && Pin->LinkedTo.Num() > 0)
																{
																	// Record the pure node output pin, since it's linked
																	if (ProfilerStatement)
																	{
																		ProfilerStatement->PureOutputContextArray.AddUnique(Pin);
																	}

																	for (UEdGraphPin* LinkedTo : Pin->LinkedTo)
																	{
																		UEdGraphNode* NodeUsingOutput = LinkedTo->GetOwningNode();
																		if (NodeUsingOutput != nullptr)
																		{
																			// Add this node, as well as other nodes this node depends on
																			TSet<UEdGraphNode*>& TargetNodesRequired = PureNodesNeeded.FindOrAdd(NodeUsingOutput);
																			TargetNodesRequired.Add(Node);
																			if (bHasAntecedentPureNodes)
																			{
																				TargetNodesRequired.Append(PureNodesNeeded.FindChecked(Node));
																			}
																		}
																	}
																}
															}
														}

														// Remove it from the linear execution list; the dependent nodes will inline the code when necessary
														Context.LinearExecutionList.RemoveAt(TestIndex);
													}
													else
													{
														if (bHasAntecedentPureNodes)
														{
															// This node requires the output of one or more pure nodes, so that pure code needs to execute at this node

															// Sort the nodes by execution order index
															TSet<UEdGraphNode*>& AntecedentPureNodes = PureNodesNeeded.FindChecked(Node);
															TArray<UEdGraphNode*> SortedPureNodes;
															for (TSet<UEdGraphNode*>::TIterator It(AntecedentPureNodes); It; ++It)
															{
																OrderedInsertIntoArray(SortedPureNodes, SortKeyMap, *It);
															}

															// Inline their code
															for (int32 i = 0; i < SortedPureNodes.Num(); ++i)
															{
																UEdGraphNode* NodeToInline = SortedPureNodes[SortedPureNodes.Num() - 1 - i];

																Context.CopyAndPrependStatements(Node, NodeToInline);
															}
														}

														// Proceed to the next node
														++TestIndex;
													}
												}

												if (Context.bIsUbergraph && CompileOptions.DoesRequireCppCodeGeneration())
												{
													Context.UnsortedSeparateExecutionGroups = FKismetCompilerUtilities::FindUnsortedSeparateExecutionGroups(Context.LinearExecutionList);
												}

											}
										}
									}
								}

								// Finalize all functions (done last to allow cross-function patchups)
								//完成所有功能（最后完成跨功能修补）
								for (int32 i = 0; i < FunctionList.Num(); ++i)
								{
									if (FunctionList[i].IsValid())
									{
										PostcompileFunction(FunctionList[i]);
										//KismetCompiler.cpp 2195行
										{
											/**
											 * Final phase of compiling a function graph; called after all functions have had CompileFunction called
											 *   - Patches up cross-references, etc..., and performs final validation
											 */
											//函数编译的第三阶段，也是编译蓝图的最后一个阶段
											//在调用了所有函数之后调用CompileFunction修补交叉引用等，并执行最终验证
											void FKismetCompilerContext::PostcompileFunction(FKismetFunctionContext& Context)
											{
												BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_PostcompileFunction);

												// The function links gotos, sorts statments, and merges adjacent ones. 
												//该函数链接定位、排序和合并相邻的语句
												Context.ResolveStatements();

												//@TODO: Code generation (should probably call backend here, not later)

												// Seal the function, it's done!
												//处理最终的编译后设置、flags、创建通常在反序列化期间设置的缓存值等
												FinishCompilingFunction(Context);
											}
										}
									}
								}

								for (TFieldIterator<UMulticastDelegateProperty> PropertyIt(NewClass); PropertyIt; ++PropertyIt)
								{
									if (const UMulticastDelegateProperty* MCDelegateProp = *PropertyIt)
									{
										if (NULL == MCDelegateProp->SignatureFunction)
										{
											MessageLog.Warning(*FString::Printf(TEXT("No SignatureFunction in MulticastDelegateProperty '%s'"), *MCDelegateProp->GetName()));
										}
									}
								}
							}
							else
							{
								// Still need to set flags on the functions even for a skeleton class
								for (int32 i = 0; i < FunctionList.Num(); ++i)
								{								
									FKismetFunctionContext& Function = FunctionList[i];
									if (Function.IsValid())
									{
										BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_PostcompileFunction);
										FinishCompilingFunction(Function);
									}
								}
							}

							FunctionListCompiledEvent.Broadcast(this);

							// Save off intermediate build products if requested
							if (bIsFullCompile && CompileOptions.bSaveIntermediateProducts && !Blueprint->bIsRegeneratingOnLoad)
							{
								// Generate code for each function (done in a second pass to allow functions to reference each other)
								for (int32 i = 0; i < FunctionList.Num(); ++i)
								{
									FKismetFunctionContext& ContextFunction = FunctionList[i];
									if (FunctionList[i].SourceGraph != NULL)
									{
										// Record this graph as an intermediate product
										ContextFunction.SourceGraph->Schema = UEdGraphSchema_K2::StaticClass();
										Blueprint->IntermediateGeneratedGraphs.Add(ContextFunction.SourceGraph);
										ContextFunction.SourceGraph->SetFlags(RF_Transient);
									}
								}
							}

							// Late validation for Delegates.
							{
								TSet<UEdGraph*> AllGraphs;
								AllGraphs.Add(UbergraphContext ? UbergraphContext->SourceGraph : NULL);
								for (const FKismetFunctionContext& FunctionContext : FunctionList)
								{
									AllGraphs.Add(FunctionContext.SourceGraph);
								}
								for (UEdGraph* Graph : AllGraphs)
								{
									if (Graph)
									{
										TArray<UK2Node_CreateDelegate*> AllNodes;
										Graph->GetNodesOfClass(AllNodes);
										for (UK2Node_CreateDelegate* Node : AllNodes)
										{
											if (Node)
											{
												Node->ValidationAfterFunctionsAreCreated(MessageLog, (0 != bIsFullCompile));
											}
										}
									}
								}
							}

							// It's necessary to tell if UberGraphFunction is ready to create frame.
							if (NewClass->UberGraphFunction)
							{
								NewClass->UberGraphFunction->SetFlags(RF_LoadCompleted);
							}

							{ BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_FinalizationWork);

							// Set any final flags and seal the class, build a CDO, etc...
							//完成我们类的编译，包括我们的最终flags、CDO等
							FinishCompilingClass(NewClass);

							// Build delegate binding maps if we have a graph
							if (ConsolidatedEventGraph)
							{
								// Build any dynamic binding information for this class
								BuildDynamicBindingObjects(NewClass);
							}

							UObject* NewCDO = NewClass->GetDefaultObject();

							// Copy over the CDO properties if we're not already regenerating on load.  In that case, the copy will be done after compile on load is complete
							FBlueprintEditorUtils::PropagateParentBlueprintDefaults(NewClass);

							if (bPropagateValuesToCDO)
							{
								if (!Blueprint->HasAnyFlags(RF_BeingRegenerated))
								{
									// Propagate the old CDO's properties to the new
									if (OldCDO)
									{
										if (OldLinker && OldGenLinkerIdx != INDEX_NONE)
										{
											// If we have a list of objects that are loading, patch our export table. This also fixes up load flags
											FBlueprintEditorUtils::PatchNewCDOIntoLinker(Blueprint->GeneratedClass->GetDefaultObject(), OldLinker, OldGenLinkerIdx, nullptr);
										}

										UEditorEngine::FCopyPropertiesForUnrelatedObjectsParams CopyDetails;
										CopyDetails.bCopyDeprecatedProperties = Blueprint->bIsRegeneratingOnLoad;
										UEditorEngine::CopyPropertiesForUnrelatedObjects(OldCDO, NewCDO, CopyDetails);
										FBlueprintEditorUtils::PatchCDOSubobjectsIntoExport(OldCDO, NewCDO);
									}
									else
									{
										// Don't perform generated class validation since we didn't do any value propagation.
										bSkipGeneratedClassValidation = true;
									}
								}

								PropagateValuesToCDO(NewCDO, OldCDO);

								// Perform any fixup or caching based on the new CDO.
								PostCDOCompiled();
							}

							// Note: The old->new CDO copy is deferred when regenerating, so we skip this step in that case.
							if (!Blueprint->HasAnyFlags(RF_BeingRegenerated))
							{
								// Update the custom property list used in post construction logic to include native class properties for which the Blueprint CDO differs from the native CDO.
								//更新后构造逻辑中使用的自定义属性列表，以包括蓝图CDO与本地CDO不停地本地类属性
								TargetClass->UpdateCustomPropertyListForPostConstruction();
							}
							}

							// Fill out the function bodies, either with function bodies, or simple stubs if this is skeleton generation
							{
								// Should we display debug information about the backend outputs?
								bool bDisplayCpp = false;
								bool bDisplayBytecode = false;

								if (!Blueprint->bIsRegeneratingOnLoad)
								{
									GConfig->GetBool(TEXT("Kismet"), TEXT("CompileDisplaysTextBackend"), /*out*/ bDisplayCpp, GEngineIni);
									GConfig->GetBool(TEXT("Kismet"), TEXT("CompileDisplaysBinaryBackend"), /*out*/ bDisplayBytecode, GEngineIni);
								}

								// Always run the VM backend, it's needed for more than just debug printing
								{
									//既然我们已经编译好了我们的类和我们的函数，name下一步我们就要通过类生成代码
									//这个代码是我们的字节码
									const bool bGenerateStubsOnly = !bIsFullCompile || (0 != MessageLog.NumErrors);
									BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_CodeGenerationTime);
									Backend_VM.GenerateCodeFromClass(NewClass, FunctionList, bGenerateStubsOnly);
									//KismetCompilerVMBackend.cpp 2143行
									{
										//通过我们的蓝图后端VM虚拟机编译我们的字节码到我们的UFucation里面
										void FKismetCompilerVMBackend::ConstructFunction(FKismetFunctionContext& FunctionContext, bool bIsUbergraph, bool bGenerateStubOnly)
										{
											UFunction* Function = FunctionContext.Function;
											UBlueprintGeneratedClass* Class = FunctionContext.NewClass;

											FString FunctionName;
											Function->GetName(FunctionName);

											TArray<uint8>& ScriptArray = Function->Script;

											// Return statement, to push on FlowStack or to use with _GotoReturn
											//一个返回语句，后面方便结束掉当前逻辑
											FBlueprintCompiledStatement ReturnStatement;
											ReturnStatement.Type = KCST_Return;

											//一个字节码处理的类
											FScriptBuilderBase ScriptWriter(ScriptArray, Class, Schema, UbergraphStatementLabelMap, bIsUbergraph, ReturnStatement);

											if (!bGenerateStubOnly)
											{
												ReturnStatement.bIsJumpTarget = true;
												if (FunctionContext.bUseFlowStack)
												{
													ScriptWriter.PushReturnAddress(ReturnStatement);
												}

												// Emit code in the order specified by the linear execution list (the first node is always the entry point for the function)
												for (int32 NodeIndex = 0; NodeIndex < FunctionContext.LinearExecutionList.Num(); ++NodeIndex)
												{
													UEdGraphNode* StatementNode = FunctionContext.LinearExecutionList[NodeIndex];
													TArray<FBlueprintCompiledStatement*>* StatementList = FunctionContext.StatementsPerNode.Find(StatementNode);

													if (StatementList != NULL)
													{
														for (int32 StatementIndex = 0; StatementIndex < StatementList->Num(); ++StatementIndex)
														{
															FBlueprintCompiledStatement* Statement = (*StatementList)[StatementIndex];

															//对每个node对应的语句进行字节码处理
															ScriptWriter.GenerateCodeForStatement(CompilerContext, FunctionContext, *Statement, StatementNode);

															const bool bUberGraphFunctionCall = Statement->FunctionToCall && (Statement->FunctionToCall == Class->UberGraphFunction)
																&& (EKismetCompiledStatementType::KCST_CallFunction == Statement->Type);
															const bool bIsReducible = FKismetCompilerUtilities::IsStatementReducible(Statement->Type) || bUberGraphFunctionCall;
															bAnyNonReducibleFunctionGenerated |= !bIsReducible;
														}
													}
												}
											}

											// Handle the function return value
											//返回语句
											ScriptWriter.GenerateCodeForStatement(CompilerContext, FunctionContext, ReturnStatement, NULL);

											// Fix up jump addresses
											ScriptWriter.PerformFixups();

											// Close out the script
											//关闭字节码处理
											ScriptWriter.CloseScript();

											// Save off the offsets within the ubergraph, needed to patch up the stubs later on
											if (bIsUbergraph)
											{
												ScriptWriter.CopyStatementMapToUbergraphMap();
											}

											// Make sure we didn't overflow the maximum bytecode size
									#if SCRIPT_LIMIT_BYTECODE_TO_64KB
											if (ScriptArray.Num() > 0xFFFF)
											{
												MessageLog.Error(TEXT("Script exceeded bytecode length limit of 64 KB"));
												ScriptArray.Empty();
												ScriptArray.Add(EX_EndOfScript);
											}
									#else
											static_assert(sizeof(CodeSkipSizeType) == 4, "Update this code as size changed.");
									#endif
										}

									}
									if (!bGenerateStubsOnly)
									{
										Blueprint->bHasAnyNonReducibleFunction = Backend_VM.bAnyNonReducibleFunctionGenerated ? UBlueprint::EIsBPNonReducible::Yes : UBlueprint::EIsBPNonReducible::No;
									}
								}

								// Fill ScriptObjectReferences arrays in functions
								if (bIsFullCompile && (0 == MessageLog.NumErrors)) // Backend_VM can generate errors, so bGenerateStubsOnly cannot be reused
								{
									for (FKismetFunctionContext& FunctionContext : FunctionList)
									{
										if (FunctionContext.IsValid())
										{
											UFunction* Function = FunctionContext.Function;
											ensure(0 == Function->ScriptObjectReferences.Num());
											FArchiveScriptReferenceCollector ObjRefCollector(Function->ScriptObjectReferences);

											for (int32 iCode = 0; iCode < Function->Script.Num();)
											{
												Function->SerializeExpr(iCode, ObjRefCollector);
											}
										}
									}
								}

								if (bDisplayBytecode && bIsFullCompile && !IsRunningCommandlet())
								{
									TGuardValue<ELogTimes::Type> DisableLogTimes(GPrintLogTimes, ELogTimes::None);

									FKismetBytecodeDisassembler Disasm(*GLog);

									// Disassemble script code
									for (int32 i = 0; i < FunctionList.Num(); ++i)
									{
										FKismetFunctionContext& Function = FunctionList[i];
										if (Function.IsValid())
										{
											UE_LOG(LogK2Compiler, Log, TEXT("\n\n[function %s]:\n"), *(Function.Function->GetName()));
											Disasm.DisassembleStructure(Function.Function);
										}
									}
								}

								// Generate code thru the backend(s)
								if ((bDisplayCpp && bIsFullCompile && !IsRunningCommandlet()) || CompileOptions.DoesRequireCppCodeGeneration())
								{
									FString CppSourceCode;
									FString HeaderSourceCode;

									{
										TUniquePtr<IBlueprintCompilerCppBackend> Backend_CPP(IBlueprintCompilerCppBackendModuleInterface::Get().Create());
										HeaderSourceCode = Backend_CPP->GenerateCodeFromClass(NewClass, FunctionList, !bIsFullCompile, CompileOptions.NativizationOptions, CppSourceCode);
									}

									if (CompileOptions.OutHeaderSourceCode.IsValid())
									{
										*CompileOptions.OutHeaderSourceCode = HeaderSourceCode;
									}

									if (CompileOptions.OutCppSourceCode.IsValid())
									{
										*CompileOptions.OutCppSourceCode = CppSourceCode;
									}

									if (bDisplayCpp && !IsRunningCommandlet())
									{
										UE_LOG(LogK2Compiler, Log, TEXT("[header]\n\n\n%s"), *HeaderSourceCode);
										UE_LOG(LogK2Compiler, Log, TEXT("[body]\n\n\n%s"), *CppSourceCode);
									}
								}

								static const FBoolConfigValueHelper DisplayLayout(TEXT("Kismet"), TEXT("bDisplaysLayout"), GEngineIni);
								if (!Blueprint->bIsRegeneratingOnLoad && bIsFullCompile && DisplayLayout && NewClass && !IsRunningCommandlet())
								{
									UE_LOG(LogK2Compiler, Log, TEXT("\n\nLAYOUT CLASS %s:"), *GetNameSafe(NewClass));

									for (UProperty* Prop : TFieldRange<UProperty>(NewClass, EFieldIteratorFlags::ExcludeSuper))
									{
										UE_LOG(LogK2Compiler, Log, TEXT("%5d:\t%-64s\t%s"), Prop->GetOffset_ForGC(), *GetNameSafe(Prop), *Prop->GetCPPType());
									}

									for (UFunction* LocFunction : TFieldRange<UFunction>(NewClass, EFieldIteratorFlags::ExcludeSuper))
									{
										UE_LOG(LogK2Compiler, Log, TEXT("\n\nLAYOUT FUNCTION %s:"), *GetNameSafe(LocFunction));
										for (UProperty* Prop : TFieldRange<UProperty>(LocFunction))
										{
											const bool bOutParam = Prop && (0 != (Prop->PropertyFlags & CPF_OutParm));
											const bool bInParam = Prop && !bOutParam && (0 != (Prop->PropertyFlags & CPF_Parm));
											UE_LOG(LogK2Compiler, Log, TEXT("%5d:\t%-64s\t%s %s%s")
												, Prop->GetOffset_ForGC(), *GetNameSafe(Prop), *Prop->GetCPPType()
												, bInParam ? TEXT("Input") : TEXT("")
												, bOutParam ? TEXT("Output") : TEXT(""));
										}
									}
								}
							}

							// For full compiles, find other blueprints that may need refreshing, and mark them dirty, in case they try to run
							if (bIsFullCompile && !Blueprint->bIsRegeneratingOnLoad && !bSkipRefreshExternalBlueprintDependencyNodes)
							{
								TArray<UBlueprint*> DependentBlueprints;
								FBlueprintEditorUtils::GetDependentBlueprints(Blueprint, DependentBlueprints);
								for (UBlueprint* CurrentBP : DependentBlueprints)
								{
									// Get the current dirty state of the package
									UPackage* const Package = CurrentBP->GetOutermost();
									const bool bStartedWithUnsavedChanges = Package != nullptr ? Package->IsDirty() : true;
									const EBlueprintStatus OriginalStatus = CurrentBP->Status;

									FBlueprintEditorUtils::RefreshExternalBlueprintDependencyNodes(CurrentBP, NewClass);

									// Dependent blueprints will be recompile anyway by reinstancer (if necessary).
									CurrentBP->Status = OriginalStatus;

									// Note: We do not send a change notification event to the dependent BP here because
									// we have not yet reinstanced any of the instances of the BP being compiled, which may
									// be referenced by instances of the dependent BP that may be reconstructed as a result.

									// Clear the package dirty state if it did not initially have any unsaved changes to begin with
									if (Package != nullptr && Package->IsDirty() && !bStartedWithUnsavedChanges)
									{
										Package->SetDirtyFlag(false);
									}
								}
							}

							// Clear out pseudo-local members that are only valid within a Compile call
							UbergraphContext = NULL;
							CallsIntoUbergraph.Empty();
							TimelineToMemberVariableMap.Empty();


							check(NewClass->PropertiesSize >= UObject::StaticClass()->PropertiesSize);
							check(NewClass->ClassDefaultObject != NULL);

							PostCompileDiagnostics();

							// Perform validation only if CDO propagation was performed above, otherwise the new CDO will not yet be fully initialized.
							if (bIsFullCompile && !bSkipGeneratedClassValidation && !Blueprint->bIsRegeneratingOnLoad)
							{
								bool Result = ValidateGeneratedClass(NewClass);
								// TODO What do we do if validation fails?
							}

							if (bIsFullCompile)
							{
								BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_ChecksumCDO);

								static const FBoolConfigValueHelper ChangeDefaultValueWithoutReinstancing(TEXT("Kismet"), TEXT("bChangeDefaultValueWithoutReinstancing"), GEngineIni);
								// CRC is usually calculated for all Properties. If the bChangeDefaultValueWithoutReinstancing optimization is enabled, then only specific properties are considered (in fact we should consider only . See UE-9883.
								// Some native properties (bCanEverTick) may be implicitly changed by KismetCompiler during compilation, so they always need to be compared.
								// Some properties with a custom Property Editor Widget may not propagate changes among instances. They may be also compared.

								class FSpecializedArchiveCrc32 : public FArchiveObjectCrc32
								{
								public:
									bool bAllProperties;

									FSpecializedArchiveCrc32(bool bInAllProperties)
										: FArchiveObjectCrc32()
										, bAllProperties(bInAllProperties)
									{}

									static bool PropertyCanBeImplicitlyChanged(const UProperty* InProperty)
									{
										check(InProperty);

										UClass* PropertyOwnerClass = InProperty->GetOwnerClass();
										const bool bOwnerIsNativeClass = PropertyOwnerClass && PropertyOwnerClass->HasAnyClassFlags(CLASS_Native);

										UStruct* PropertyOwnerStruct = InProperty->GetOwnerStruct();
										const bool bOwnerIsNativeStruct = !PropertyOwnerClass && (!PropertyOwnerStruct || !PropertyOwnerStruct->IsA<UUserDefinedStruct>());

										return InProperty->IsA<UStructProperty>()
											|| bOwnerIsNativeClass || bOwnerIsNativeStruct;
									}

									// Begin FArchive Interface
									virtual bool ShouldSkipProperty(const UProperty* InProperty) const override
									{
										return FArchiveObjectCrc32::ShouldSkipProperty(InProperty)
											|| (!bAllProperties && !PropertyCanBeImplicitlyChanged(InProperty));
									}
									// End FArchive Interface
								};

								UObject* NewCDO = NewClass->GetDefaultObject(false);
								FSpecializedArchiveCrc32 CrcArchive(!ChangeDefaultValueWithoutReinstancing);
								Blueprint->CrcLastCompiledCDO = NewCDO ? CrcArchive.Crc32(NewCDO) : 0;
							}

							if (bIsFullCompile)
							{
								BP_SCOPED_COMPILER_EVENT_STAT(EKismetCompilerStats_ChecksumSignature);

								class FSignatureArchiveCrc32 : public FArchiveObjectCrc32
								{
								public:
									static bool IsInnerProperty(const UObject* Object)
									{
										const UProperty* Property = Cast<const UProperty>(Object);
										return Property // check arrays
											&& Cast<const UFunction>(Property->GetOwnerStruct())
											&& !Property->HasAnyPropertyFlags(CPF_Parm);
									}

									virtual FArchive& operator<<(UObject*& Object) override
									{
										FArchive& Ar = *this;

										if (Object && !IsInnerProperty(Object))
										{
											// Names of functions and properties are significant.
											FString UniqueName = GetPathNameSafe(Object);
											Ar << UniqueName;

											if (Object->IsIn(RootObject))
											{
												ObjectsToSerialize.Enqueue(Object);
											}
										}

										return Ar;
									}

									virtual bool CustomSerialize(UObject* Object) override
									{
										FArchive& Ar = *this;

										bool bResult = false;
										if (UStruct* Struct = Cast<UStruct>(Object))
										{
											if (Object == RootObject) // name and location are significant for the signature
											{
												FString UniqueName = GetPathNameSafe(Object);
												Ar << UniqueName;
											}

											UObject* SuperStruct = Struct->GetSuperStruct();
											Ar << SuperStruct;

											UField* ChildrenIter = Struct->Children;
											while (ChildrenIter)
											{
												Ar << ChildrenIter;
												ChildrenIter = ChildrenIter->Next;
											}

											if (UFunction* Function = Cast<UFunction>(Struct))
											{
												Ar << Function->FunctionFlags;
											}

											if (UClass* AsClass = Cast<UClass>(Struct))
											{
												Ar << (uint32&)AsClass->ClassFlags;
												Ar << AsClass->Interfaces;
											}

											Ar << Struct->Next;

											bResult = true;
										}

										return bResult;
									}
								};

								FSignatureArchiveCrc32 SignatureArchiveCrc32;
								UBlueprint* ParentBP = UBlueprint::GetBlueprintFromClass(NewClass->GetSuperClass());
								const uint32 ParentSignatureCrc = ParentBP ? ParentBP->CrcLastCompiledSignature : 0;
								Blueprint->CrcLastCompiledSignature = SignatureArchiveCrc32.Crc32(NewClass, ParentSignatureCrc);
							}

							PostCompile();
						}
					}
				}

				if (CompilerData.ActiveResultsLog->NumErrors == 0)
				{
					// Blueprint is error free.  Go ahead and fix up debug info
					BP->Status = (0 == CompilerData.ActiveResultsLog->NumWarnings) ? BS_UpToDate : BS_UpToDateWithWarnings;

					BP->BlueprintSystemVersion = UBlueprint::GetCurrentBlueprintSystemVersion();

					// Reapply breakpoints to the bytecode of the new class
					for (UBreakpoint* Breakpoint : BP->Breakpoints)
					{
						FKismetDebugUtilities::ReapplyBreakpoint(Breakpoint);
					}
				}
				else
				{
					BP->Status = BS_Error; // do we still have the old version of the class?
				}

				// SOC settings only apply after compile on load:
				if (!BP->bIsRegeneratingOnLoad)
				{
					if (bSaveBlueprintsAfterCompile || (bSaveBlueprintAfterCompileSucceeded && BP->Status == BS_UpToDate))
					{
						CompiledBlueprintsToSave.Add(BP);
					}
				}
			}

			if (BPGC)
			{
				BPGC->ClassFlags &= ~CLASS_ReplicationDataIsSetUp;
				BPGC->SetUpRuntimeReplicationData();
			}

			ensure(BPGC == nullptr || BPGC->ClassDefaultObject->GetClass() == BPGC);
		}
	} // end GTimeCompiling scope

	// STAGE XIV: Now we can finish the first stage of the reinstancing operation, moving old classes to new classes:
	//编译第十四阶段：将旧类移到新类（检测的第一阶段）
	{
		{
			DECLARE_SCOPE_HIERARCHICAL_COUNTER(MoveOldClassesToNewClasses)

			TArray<FReinstancingJob> Reinstancers;
			// Set up reinstancing jobs - we need a reference to the compiler in order to honor 
			// CopyTermDefaultsToDefaultObject
			for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
			{
				if (CompilerData.Reinstancer.IsValid() && CompilerData.Reinstancer->ClassToReinstance)
				{
					Reinstancers.Push(
						FReinstancingJob(CompilerData.Reinstancer, CompilerData.Compiler)
					);
				}
			}

			FScopedDurationTimer ReinstTimer(GTimeReinstancing);
			ReinstanceBatch(Reinstancers, ClassesToReinstance, InLoadContext);

			// We purposefully do not remove the OldCDOs yet, need to keep them in memory past first GC
		}

		// STAGE XV: POST CDO COMPILED
		//编译的第十五阶段
		//CDO的最后编译	
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			DECLARE_SCOPE_HIERARCHICAL_COUNTER(PostCDOCompiled)

			if (!CompilerData.IsSkeletonOnly() && CompilerData.Compiler.IsValid())
			{
				CompilerData.Compiler->PostCDOCompiled();
			}
		}

		// STAGE XVI: CLEAR TEMPORARY FLAGS
		//编译的第十六阶段
		//清楚临时的标记
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			DECLARE_SCOPE_HIERARCHICAL_COUNTER(ClearTemporaryFlags)

			UBlueprint* BP = CompilerData.BP;

			if (!CompilerData.IsSkeletonOnly())
			{
				FBlueprintEditorUtils::UpdateDelegatesInBlueprint(BP);
				if (!BP->bIsRegeneratingOnLoad && BP->GeneratedClass)
				{
					FKismetEditorUtilities::StripExternalComponents(BP);

					if (BP->SimpleConstructionScript)
					{
						BP->SimpleConstructionScript->FixupRootNodeParentReferences();
					}

					if (BP->Status != BS_Error)
					{
						if (CompilerData.Compiler.IsValid())
						{
							// Route through the compiler context in order to perform type-specific Blueprint class validation.
							CompilerData.Compiler->ValidateGeneratedClass(CastChecked<UBlueprintGeneratedClass>(BP->GeneratedClass));

							if (CompilerData.ShouldValidateClassDefaultObject())
							{
								// Our CDO should be properly constructed by this point and should always exist
								UObject* ClassDefaultObject = BP->GeneratedClass->GetDefaultObject(false);
								if (ensureAlways(ClassDefaultObject))
								{
									FKismetCompilerUtilities::ValidateEnumProperties(ClassDefaultObject, *CompilerData.ActiveResultsLog);

									// Make sure any class-specific validation passes on the CDO
									TArray<FText> ValidationErrors;
									EDataValidationResult ValidateCDOResult = ClassDefaultObject->IsDataValid(/*out*/ ValidationErrors);
									if (ValidateCDOResult == EDataValidationResult::Invalid)
									{
										for (const FText& ValidationError : ValidationErrors)
										{
											CompilerData.ActiveResultsLog->Error(*ValidationError.ToString());
										}
									}

									// Adjust Blueprint status to match anything new that was found during validation.
									if (CompilerData.ActiveResultsLog->NumErrors > 0)
									{
										BP->Status = BS_Error;
									}
									else if (BP->Status == BS_UpToDate && CompilerData.ActiveResultsLog->NumWarnings > 0)
									{
										BP->Status = BS_UpToDateWithWarnings;
									}
								}
							}
						}
						else
						{
							UBlueprint::ValidateGeneratedClass(BP->GeneratedClass);
						}
					}
				}
			}

			if (CompilerData.ShouldRegisterCompilerResults())
			{
				// This helper structure registers the results log messages with the UI control that displays them:
				FScopedBlueprintMessageLog MessageLog(BP);
				MessageLog.Log->ClearMessages();
				MessageLog.Log->AddMessages(CompilerData.ActiveResultsLog->Messages, false);
			}

			if (CompilerData.ShouldSetTemporaryBlueprintFlags())
			{
				BP->bBeingCompiled = false;
				BP->CurrentMessageLog = nullptr;
				BP->bIsRegeneratingOnLoad = false;
			}

			if (UPackage* Package = BP->GetOutermost())
			{
				Package->SetDirtyFlag(CompilerData.bPackageWasDirty);
			}
		}

		// Make sure no junk in bytecode, this can happen only for blueprints that were in CurrentlyCompilingBPs because
		// the reinstancer can detect all other references (see UpdateBytecodeReferences):
		for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
		{
			if (CompilerData.ShouldCompileClassFunctions())
			{
				if (BlueprintsCompiled)
				{
					BlueprintsCompiled->Add(CompilerData.BP);
				}

				if (!bSuppressBroadcastCompiled)
				{
					// Some logic (e.g. UObject::ProcessInternal) uses this flag to suppress warnings:
					TGuardValue<bool> ReinstancingGuard(GIsReinstancing, true);
					CompilerData.BP->BroadcastCompiled();
				}

				continue;
			}

			UBlueprint* BP = CompilerData.BP;
			for (TFieldIterator<UFunction> FuncIter(BP->GeneratedClass, EFieldIteratorFlags::ExcludeSuper); FuncIter; ++FuncIter)
			{
				UFunction* CurrentFunction = *FuncIter;
				if (CurrentFunction->Script.Num() > 0)
				{
					FFixupBytecodeReferences ValidateAr(CurrentFunction);
				}
			}
		}

		if (!bSuppressBroadcastCompiled)
		{
			DECLARE_SCOPE_HIERARCHICAL_COUNTER(BroadcastBlueprintCompiled)

			if (GEditor)
			{
				GEditor->BroadcastBlueprintCompiled();
			}
		}
	}

	for (FCompilerData& CompilerData : CurrentlyCompilingBPs)
	{
		if (CompilerData.ResultsLog)
		{
			CompilerData.ResultsLog->EndEvent();
		}
		CompilerData.BP->bQueuedForCompilation = false;
	}

	{
		DECLARE_SCOPE_HIERARCHICAL_COUNTER(UEdGraphPin::Purge)
		UEdGraphPin::Purge();
	}

	UE_LOG(LogBlueprint, Display, TEXT("Time Compiling: %f, Time Reinstancing: %f"), GTimeCompiling, GTimeReinstancing);
	//GTimeCompiling = 0.0;
	//GTimeReinstancing = 0.0;
	ensure(QueuedRequests.Num() == 0);
}


#endif