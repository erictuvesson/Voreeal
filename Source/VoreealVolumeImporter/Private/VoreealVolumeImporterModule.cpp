#include "VoreealVolumeImporterPrivatePCH.h"

class FVoreealVolumeImporterModule : public FDefaultModuleImpl
{
public:
    virtual void StartupModule() override
    {
    }

    virtual void ShutdownModule() override
    {
    }
};

IMPLEMENT_MODULE(FVoreealVolumeImporterModule, VoreealImporter);

DEFINE_LOG_CATEGORY(LogVoreealVolumeImporter);
