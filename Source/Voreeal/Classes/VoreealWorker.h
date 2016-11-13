#pragma once

#include <mutex>

class FVoreealWorker : public FRunnable
{
	typedef TFunction<FVoreealMesh()>	TFunc;		   //< The input function
	typedef TPair<uint32, TFunc>		TTask;		   //< Identifier with input function
	typedef TPair<uint32, FVoreealMesh>	TTaskResult;   //< Identifier with function result

public:
	FVoreealWorker();
	virtual ~FVoreealWorker();

	// Begin FRunnable Interface
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable Interface

	// Ensure the thread is complete.
	void EnsureCompletion();

	// Returns a new unused identifier.
	uint32 RequestNewIdentifier();

	// Gets if there is a completed task with the specified Identifier.
	TSharedPtr<FVoreealMesh, ESPMode::Fast> GetTask(uint32 Identifier);

	// Add a task to the queue.
	void AddTask(uint32 Identifier, TFunc Func);

	// Add a task to the top of the queue.
	void AddPriorityTask(uint32 Identifier, TFunc Func);

private:
	FRunnableThread* Thread;
	FThreadSafeCounter StopTaskCounter;

	TArray<TTaskResult> FinishedTasks;
	std::mutex FinishedTasksMutex;

	TArray<TTask> Tasks;
	std::mutex TasksMutex;

	uint32 NextIdentifier;
	std::mutex NextIdentifierMutex;
};
