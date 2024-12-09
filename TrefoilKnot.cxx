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
#include <vtkMath.h>
#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>
#include <sstream>

// Function to get input with a default value
template <typename T>
T getInputWithDefault(const std::string& prompt, T defaultValue) {
    std::cout << prompt << " (default: " << defaultValue << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (input.empty()) {
        return defaultValue;
    }
    std::istringstream stream(input);
    T value;
    stream >> value;
    return stream.fail() ? defaultValue : value;
}

int main() {
    // User input for the range of t and number of points
    double tStart = getInputWithDefault("Enter start value for t", 0.0);
    double tEnd = getInputWithDefault("Enter end value for t", 2.0 * vtkMath::Pi());
    int numPoints = getInputWithDefault("Enter number of points", 500);

    // Create a vtkPoints object to store the curve points
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Calculate points
    double step = (tEnd - tStart) / (numPoints - 1);
    for (int i = 0; i < numPoints; ++i) {
        double t = tStart + i * step;
        double x = std::sin(t) + 2 * std::sin(2 * t);
        double y = std::cos(t) - 2 * std::cos(2 * t);
        double z = -std::sin(3 * t);
        points->InsertNextPoint(x, y, z);
    }

    // Create a cell array to store the lines
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < numPoints - 1; ++i) {
        vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, i);
        line->GetPointIds()->SetId(1, i + 1);
        lines->InsertNextCell(line);
    }

    // Create a polydata to store the geometry
    vtkSmartPointer<vtkPolyData> curvePolyData = vtkSmartPointer<vtkPolyData>::New();
    curvePolyData->SetPoints(points);
    curvePolyData->SetLines(lines);

    // Create a mapper and actor for visualization
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(curvePolyData);

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
