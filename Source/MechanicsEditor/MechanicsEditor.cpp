#include "MechanicsEditor.h"
#include "UnrealEd.h"
#include "ComponentVisualizer.h"
#include "QuadLevelMatrixComponentVisualizer.h"
#include "Mechanics/Components/QuadLevelMatrixComponent.h"

IMPLEMENT_MODULE(FMechanicsEditorModule, MechanicsEditor)

void FMechanicsEditorModule::StartupModule()
{
 
    IModuleInterface::StartupModule();

   
    if (GUnrealEd)
    {
        // Create and register the component visualizer
        const TSharedPtr<FQuadLevelMatrixComponentVisualizer> LevelMatrixComponentVisualizer = MakeShareable(new FQuadLevelMatrixComponentVisualizer());
        GUnrealEd->RegisterComponentVisualizer(UQuadLevelMatrixComponent::StaticClass()->GetFName(), LevelMatrixComponentVisualizer);
        LevelMatrixComponentVisualizer->OnRegister();
    }
}

void FMechanicsEditorModule::ShutdownModule()
{
    IModuleInterface::ShutdownModule();

    if (GUnrealEd)
    {
        GUnrealEd->UnregisterComponentVisualizer(UQuadLevelMatrixComponent::StaticClass()->GetFName());
    }
}