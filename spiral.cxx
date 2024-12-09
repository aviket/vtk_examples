#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <cmath>
#include <iostream>

double getInputWithDefault(const std::string& prompt, double defaultValue) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    // If the input is empty, return the default value
    if (input.empty()) {
        return defaultValue;
    }

    // Otherwise, try to convert the input to a double
    try {
        return std::stod(input);
    }
    catch (const std::invalid_argument&) {
        // If the conversion fails, return the default value
        std::cerr << "Invalid input. Using default value " << defaultValue << ".\n";
        return defaultValue;
    }
}


int main()
{
    // User inputs for spiral parameters
    double pitch = 1.0;  // Distance between turns along the Z-axis
    double radius = 5.0; // Radius of the spiral
    unsigned int turns = 5; // Number of turns
    unsigned int numPointsPerTurn = 100; // Number of points per turn for smoothness

    // std::cout << "Enter pitch of the spiral (default 1.0): ";
    // std::cin >> pitch;
    // std::cout << "Enter radius of the spiral (default 5.0): ";
    // std::cin >> radius;
    // std::cout << "Enter number of turns (default 5): ";
    // std::cin >> turns;
	
	 pitch = getInputWithDefault("Enter pitch of the spiral (default 1.0): ", pitch);
	 radius = getInputWithDefault("Enter radius of the spiral (default 5.0): ", radius);
	 turns = getInputWithDefault("Enter number of turns (default 5): ", turns);
	
	

    // Total number of points
    unsigned int numPoints = numPointsPerTurn * turns;

    // Create a vtkPoints object to store the spiral points
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Calculate points for the spiral
    for (unsigned int i = 0; i < numPoints; i++)
    {
        double angle = 2.0 * vtkMath::Pi() * i / numPointsPerTurn; // Angle in radians
        double z = pitch * i / numPointsPerTurn;                  // Z increases with pitch
        double x = radius * std::cos(angle);                     // X-coordinate
        double y = radius * std::sin(angle);                     // Y-coordinate

        points->InsertNextPoint(x, y, z);
    }

    // Create a cell array to store the lines and connect points
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    for (unsigned int i = 0; i < numPoints - 1; i++)
    {
        vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, i);
        line->GetPointIds()->SetId(1, i + 1);
        lines->InsertNextCell(line);
    }

    // Create a polydata to store the geometry
    vtkSmartPointer<vtkPolyData> spiralPolyData = vtkSmartPointer<vtkPolyData>::New();
    spiralPolyData->SetPoints(points);
    spiralPolyData->SetLines(lines);

    // Create a mapper and actor for visualization
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(spiralPolyData);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // Create a renderer and render window
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // Create a render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Add the actor to the scene
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.2, 0.4); // Set background color

    // Render and interact
    renderWindow->Render();
    renderWindowInteractor->Start();

    return 0;
}
