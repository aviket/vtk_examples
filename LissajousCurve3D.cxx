#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLine.h>
#include <vtkMath.h>

#include <cmath>
#include <iostream>

// Define M_PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
    // User input for parameters of the Lissajous curve
    double A = 1.0, B = 1.0, C = 1.0;
    double a = 3.0, b = 2.0, c = 5.0; // Frequencies
    double deltaX = 0.0, deltaY = M_PI / 2.0, deltaZ = M_PI / 2.0; // Phase shifts

    // Get user input with default handling
    A = getInputWithDefault("Enter amplitude A (default 1.0): ", A);
    B = getInputWithDefault("Enter amplitude B (default 1.0): ", B);
    C = getInputWithDefault("Enter amplitude C (default 1.0): ", C);
    a = getInputWithDefault("Enter frequency a (default 3.0): ", a);
    b = getInputWithDefault("Enter frequency b (default 2.0): ", b);
    c = getInputWithDefault("Enter frequency c (default 5.0): ", c);

    // Number of points for smooth curve
    const unsigned int numPoints = 1000;

    // Create a vtkPoints object to store the points of the curve
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Calculate the points for the 3D Lissajous curve
    for (unsigned int i = 0; i < numPoints; i++)
    {
        double t = (2 * M_PI * i) / (numPoints - 1); // Parameter t from 0 to 2π
        double x = A * sin(a * t + deltaX);
        double y = B * sin(b * t + deltaY);
        double z = C * sin(c * t + deltaZ);

        points->InsertNextPoint(x, y, z);
    }

    // Create a cell array to store the lines (connect the points)
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

    for (unsigned int i = 0; i < numPoints - 1; i++)
    {
        vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, i);
        line->GetPointIds()->SetId(1, i + 1);
        lines->InsertNextCell(line);
    }

    // Create polydata to store the geometry
    vtkSmartPointer<vtkPolyData> lissajousPolyData = vtkSmartPointer<vtkPolyData>::New();
    lissajousPolyData->SetPoints(points);
    lissajousPolyData->SetLines(lines);

    // Create a mapper and actor for visualization
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(lissajousPolyData);

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
