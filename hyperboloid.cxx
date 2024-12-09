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
#include <limits>
#include <cmath>
#include <sstream>


// Function to get user input with default value
template <typename T>
T getInputWithDefault(const std::string& prompt, T defaultValue) {
    std::cout << prompt << " (default " << defaultValue << "): ";
    std::string input;
    std::getline(std::cin, input);
    if (input.empty()) {
        return defaultValue;
    }
    std::istringstream stream(input);
    T value;
    if (stream >> value) {
        return value;
    }
    return defaultValue;
}

int main() {
    // Get user inputs
    double a = getInputWithDefault("Enter parameter a", 1.0);
    double b = getInputWithDefault("Enter parameter b", 1.0);
    double c = getInputWithDefault("Enter parameter c", 1.0);
    double uMin = getInputWithDefault("Enter minimum value of u (default -2)", -2.0);
    double uMax = getInputWithDefault("Enter maximum value of u (default 2)", 2.0);
    double vMin = getInputWithDefault("Enter minimum value of v (default 0)", 0.0);
    double vMax = getInputWithDefault("Enter maximum value of v (default 2*PI)", 2 * vtkMath::Pi());
    int uSteps = getInputWithDefault("Enter number of steps in u-direction (default 50)", 50);
    int vSteps = getInputWithDefault("Enter number of steps in v-direction (default 50)", 50);

    // Create a vtkPoints object to store the points of the hyperboloid
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Generate points for the hyperboloid
    for (int i = 0; i <= uSteps; ++i) {
        double u = uMin + i * (uMax - uMin) / uSteps;
        for (int j = 0; j <= vSteps; ++j) {
            double v = vMin + j * (vMax - vMin) / vSteps;
            double x = a * cosh(u) * cos(v);
            double y = b * cosh(u) * sin(v);
            double z = c * sinh(u);
            points->InsertNextPoint(x, y, z);
        }
    }

    // Create a cell array to store the lines connecting the points
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

    for (int i = 0; i < uSteps; ++i) {
        for (int j = 0; j < vSteps; ++j) {
            vtkSmartPointer<vtkLine> line1 = vtkSmartPointer<vtkLine>::New();
            vtkSmartPointer<vtkLine> line2 = vtkSmartPointer<vtkLine>::New();

            int current = i * (vSteps + 1) + j;
            int nextV = current + 1;
            int nextU = (i + 1) * (vSteps + 1) + j;

            line1->GetPointIds()->SetId(0, current);
            line1->GetPointIds()->SetId(1, nextV);
            lines->InsertNextCell(line1);

            line2->GetPointIds()->SetId(0, current);
            line2->GetPointIds()->SetId(1, nextU);
            lines->InsertNextCell(line2);
        }
    }

    // Create a polydata to store the geometry
    vtkSmartPointer<vtkPolyData> hyperboloidPolyData = vtkSmartPointer<vtkPolyData>::New();
    hyperboloidPolyData->SetPoints(points);
    hyperboloidPolyData->SetLines(lines);

    // Create a mapper and actor for visualization
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(hyperboloidPolyData);

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
