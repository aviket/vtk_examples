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
    // User input for start and end degrees
    double startDeg = -360;
    double endDeg = 360;
    //std::cout << "Enter start degree (-360 by default): ";
    //std::cin >> startDeg;
    //std::cout << "Enter end degree (360 by default): ";
    //std::cin >> endDeg;

    startDeg = getInputWithDefault("Enter start degree (-360 by default): ", startDeg);
    endDeg = getInputWithDefault("Enter end degree (360 by default): ", endDeg);

    // Number of points for smooth sine wave
    const unsigned int numPoints = 1000;

    // Create a vtkPoints object to store the sine wave points
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    // Calculate points
    double startRad = vtkMath::RadiansFromDegrees(startDeg);
    double endRad = vtkMath::RadiansFromDegrees(endDeg);
    double step = (endRad - startRad) / (numPoints - 1);

    for (unsigned int i = 0; i < numPoints; i++)
    {
        double x = startRad + i * step;
        points->InsertNextPoint(x, std::sin(x), 0.0);
    }

    // Create a cell array to store the lines in and add the lines
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

    for (unsigned int i = 0; i < numPoints - 1; i++)
    {
        vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0, i);
        line->GetPointIds()->SetId(1, i + 1);
        lines->InsertNextCell(line);
    }

    // Create a polydata to store the geometry
    vtkSmartPointer<vtkPolyData> sineWavePolyData = vtkSmartPointer<vtkPolyData>::New();
    sineWavePolyData->SetPoints(points);
    sineWavePolyData->SetLines(lines);

    // Create a mapper and actor for visualization
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(sineWavePolyData);

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
