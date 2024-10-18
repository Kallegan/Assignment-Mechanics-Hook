#include "MechanicsEditor.h"
#include "UnrealEd.h"
#include "ComponentVisualizer.h"
#include "QuadLevelMatrixComponentVisualizer.h"
#include "Mechanics/Components/QuadLevelMatrixComponent.h"

// Implement the module for the editor
IMPLEMENT_MODULE(FMechanicsEditorModule, MechanicsEditor)

void FMechanicsEditorModule::StartupModule()
{
    // Call the base module startup
    IModuleInterface::StartupModule();

    // Check if we are in the editor
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
    // Call the base module shutdown
    IModuleInterface::ShutdownModule();

    // Unregister the component visualizer if we are in the editor
    if (GUnrealEd)
    {
        GUnrealEd->UnregisterComponentVisualizer(UQuadLevelMatrixComponent::StaticClass()->GetFName());
    }
}