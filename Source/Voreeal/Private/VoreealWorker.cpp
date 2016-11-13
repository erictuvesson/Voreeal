#include "VoreealPrivatePCH.h"
#include "VoreealWorker.h"

FVoreealWorker::FVoreealWorker()
	: NextIdentifier(0)
{
	// windows default = 8mb for thread, could specify more
	Thread = FRunnableThread::Create(this, TEXT("FVoreealWorker"), 0, TPri_BelowNormal);
}

FVoreealWorker::~FVoreealWorker()
{
	delete Thread;
	Thread = nullptr;
}

bool FVoreealWorker::Init()
{
	return true;
}

uint32 FVoreealWorker::Run()
{
	// Initial wait before starting
	FPlatformProcess::Sleep(0.03f);

	// While not told to stop this thread and not yet finished the task
	while (StopTaskCounter.GetValue() == 0)
	{
		TasksMutex.lock();
		int32 Count = Tasks.Num();
		TasksMutex.unlock();

		if (Count > 0)
		{
			TasksMutex.lock();
			TTask Func = Tasks[0];
			Tasks.RemoveAt(0);
			TasksMutex.unlock();

			FVoreealMesh Result = Func.Value();
			FinishedTasksMutex.lock();
			FinishedTasks.Add(TTaskResult(TPairInitializer<uint32, FVoreealMesh>(Func.Key, Result)));
			FinishedTasksMutex.unlock();
		}

		FPlatformProcess::Sleep(0.05f);
	}

	return 0;
}

void FVoreealWorker::Stop()
{
	StopTaskCounter.Increment();
}

void FVoreealWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

uint32 FVoreealWorker::RequestNewIdentifier()
{
	NextIdentifierMutex.lock();
	uint32 NewIdentifier = ++NextIdentifier;
	NextIdentifierMutex.unlock();
	return NewIdentifier;
}

TSharedPtr<FVoreealMesh, ESPMode::Fast> FVoreealWorker::GetTask(uint32 Identifier)
{
	int32 FoundIndex = -1;
	TSharedPtr<FVoreealMesh, ESPMode::Fast> FoundResult;

	FinishedTasksMutex.lock();
	{
		for (int32 i = 0; i < FinishedTasks.Num(); i++)
		{
			if (FinishedTasks[i].Key == Identifier)
			{
				FoundIndex = i;
				FoundResult = MakeShareable(new FVoreealMesh(FinishedTasks[i].Value));
				break;
			}
		}

		if (FoundIndex != -1)
		{
			FinishedTasks.RemoveAt(FoundIndex);
		}
	}
	FinishedTasksMutex.unlock();

	return TSharedPtr<FVoreealMesh, ESPMode::Fast>(FoundResult);
}

void FVoreealWorker::AddTask(uint32 Identifier, TFunc Func)
{
	TasksMutex.lock();
	Tasks.Add(TTask(TPairInitializer<uint32, TFunc>(Identifier, Func)));
	TasksMutex.unlock();
}

void FVoreealWorker::AddPriorityTask(uint32 Identifier, TFunc Func)
{
	// TODO: Add after all priority tasks
	TasksMutex.lock();
	Tasks.Insert(TTask(TPairInitializer<uint32, TFunc>(Identifier, Func)), 0);
	TasksMutex.unlock();
}


//template <typename FVoreealMesh>
//FVoreealNodeWorker* FVoreealWorker::Initialize()
//{
//	static FVoreealNodeWorker* Runnable = nullptr;
//
//	if (!Runnable && FPlatformProcess::SupportsMultithreading())
//	{
//		Runnable = new FVoreealNodeWorker();
//	}
//
//	return Runnable;
//}
//
//template <typename FVoreealMesh>
//void FVoreealWorker::Shutdown()
//{
//	if (Runnable)
//	{
//		Runnable->EnsureCompletion();
//		delete Runnable;
//		Runnable = nullptr;
//	}
//}
