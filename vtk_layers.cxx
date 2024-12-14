#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkProperty.h>

void ToggleLayerVisibility(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData)
{
    vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
    vtkRenderer** renderers = static_cast<vtkRenderer**>(clientData);

    const char* key = interactor->GetKeySym();
    static bool sphereLayerVisible = true;
    static bool coneLayerVisible = true;
    static bool cubeLayerVisible = true;

    if (std::string(key) == "s") // Toggle sphere layer
    {
        renderers[0]->SetDraw(!sphereLayerVisible);
        sphereLayerVisible = !sphereLayerVisible;
    }
    else if (std::string(key) == "c") // Toggle cone layer
    {
        renderers[1]->SetDraw(!coneLayerVisible);
        coneLayerVisible = !coneLayerVisible;
    }
    else if (std::string(key) == "u") // Toggle cube layer
    {
        renderers[2]->SetDraw(!cubeLayerVisible);
        cubeLayerVisible = !cubeLayerVisible;
    }

    // Update active layer properties
    for (int i = 0; i < 3; ++i)
    {
        if (renderers[i]->GetDraw())
        {
            renderers[i]->SetBackground(0.1, 0.2, 0.4); // Visible layers get a blue background
        }
        else
        {
            renderers[i]->SetBackground(0.0, 0.0, 0.0); // Hidden layers are effectively invisible
            renderers[i]->SetBackgroundAlpha(0.0);
        }
    }

    // Force a complete render refresh
    interactor->GetRenderWindow()->Render();
}

int main(int, char* [])
{
    // Create a sphere
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(5.0);
    sphereSource->SetThetaResolution(32);
    sphereSource->SetPhiResolution(32);

    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    // Create a cone
    vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetHeight(10.0);
    coneSource->SetRadius(5.0);
    coneSource->SetResolution(32);

    vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection(coneSource->GetOutputPort());

    vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);

    // Create a cube
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->SetXLength(6.0);
    cubeSource->SetYLength(6.0);
    cubeSource->SetZLength(6.0);

    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputConnection(cubeSource->GetOutputPort());

    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);

    // Create a dummy actor. Added last for avoiding visibility toggle problem in last actor.
    vtkSmartPointer<vtkPolyData> dummyPolyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyDataMapper> dummyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    dummyMapper->SetInputData(dummyPolyData);

    vtkSmartPointer<vtkActor> dummyActor = vtkSmartPointer<vtkActor>::New();
    dummyActor->SetMapper(dummyMapper);
    dummyActor->GetProperty()->SetOpacity(0.0); // Fully transparent


    // Create renderers
    vtkSmartPointer<vtkRenderer> sphereRenderer = vtkSmartPointer<vtkRenderer>::New();
    sphereRenderer->AddActor(sphereActor);
    sphereRenderer->SetBackground(0.1, 0.2, 0.4);
    sphereRenderer->SetLayer(3); // Highest layer

    vtkSmartPointer<vtkRenderer> coneRenderer = vtkSmartPointer<vtkRenderer>::New();
    coneRenderer->AddActor(coneActor);
    coneRenderer->SetBackground(0.1, 0.2, 0.4);
    coneRenderer->SetLayer(2);

    vtkSmartPointer<vtkRenderer> cubeRenderer = vtkSmartPointer<vtkRenderer>::New();
    cubeRenderer->AddActor(cubeActor);
    cubeRenderer->SetBackground(0.1, 0.2, 0.4);
    cubeRenderer->SetLayer(1); // Lowest layer

    vtkSmartPointer<vtkRenderer> dummyRenderer = vtkSmartPointer<vtkRenderer>::New();
    dummyRenderer->AddActor(dummyActor);
    dummyRenderer->SetBackground(0.1, 0.2, 0.4);
    dummyRenderer->SetLayer(0); // Lowest layer

    // Create a render window
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetNumberOfLayers(4);
    renderWindow->AddRenderer(cubeRenderer);
    renderWindow->AddRenderer(coneRenderer);
    renderWindow->AddRenderer(sphereRenderer);
    renderWindow->AddRenderer(dummyRenderer);
    renderWindow->SetSize(800, 600);
    renderWindow->SetWindowName("Sphere, Cone, and Cube");

    // Create a render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Set up layer toggle callback
    vtkSmartPointer<vtkCallbackCommand> toggleCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    toggleCallback->SetCallback(ToggleLayerVisibility);

    vtkRenderer* renderers[3] = { sphereRenderer, coneRenderer, cubeRenderer };
    toggleCallback->SetClientData(renderers);

    renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, toggleCallback);

    // Start the interaction
    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
