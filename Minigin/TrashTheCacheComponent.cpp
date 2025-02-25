#include "TrashTheCacheComponent.h"
#include <algorithm>
#include <string>
#include <chrono>
#include <numeric>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_plot.h"
#include <iostream>

TrashTheCacheComponent::TrashTheCacheComponent(GameObject* owner)
	: Component(owner), m_SizeIntArray{ int(pow(2, 26)) }, m_SizeGameObject3DArray{int(pow(2, 23))}
{
    m_IntArray = new int[m_SizeIntArray]{};
    m_StepSizes = new float[m_AmountOfSteps] { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
    m_StepTimingsInt = new float[m_AmountOfSteps] {};
    m_StepTimingsGameObject3D = new float[m_AmountOfSteps] {};
    m_StepTimingsGameObject3DAlt = new float[m_AmountOfSteps] {};;
    m_GameObject3DArray = new GameObject3D[m_SizeGameObject3DArray]{};
    m_GameObject3DAltArray = new GameObject3DAlt[m_SizeGameObject3DArray]{};
}

TrashTheCacheComponent::~TrashTheCacheComponent()
{
    delete[] m_IntArray;
    delete[] m_StepSizes;
    delete[] m_StepTimingsInt;
    delete[] m_GameObject3DArray;
    delete[] m_StepTimingsGameObject3D;
    delete[] m_GameObject3DAltArray;
    delete[] m_StepTimingsGameObject3DAlt;
}

void TrashTheCacheComponent::Start()
{
}

void TrashTheCacheComponent::Update()
{
}

void TrashTheCacheComponent::Render() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    if (ImGui::Begin("Exercise 1"))
    {
        ImGui::PushItemWidth(100);
        ImGui::InputInt("# samples", &m_SampleCount);
        ImGui::PopItemWidth();
    
        if (ImGui::Button("Trash The Cache"))
        {
            Exercise1();
        }
        DrawExercise1Graph();
        ImGui::End();
    }

    if (ImGui::Begin("Exercise 2"))
    {
        ImGui::PushItemWidth(100);
        ImGui::InputInt("# samples", &m_SampleCount);
        ImGui::PopItemWidth();

        if (ImGui::Button("Trash The Cache With GameObject3D"))
        {
            Exercise2GameObject3D();
        }
        DrawExercise2GameObject3DGraph();
        if (ImGui::Button("Trash The Cache With GameObject3DAlt"))
        {
            Exercise2GameObject3DAlt();
        }
        DrawExercise2GameObject3DAltGraph();
        ImGui::Text("Combined: ");
        DrawExercise2Combined();
        ImGui::End();
    }
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TrashTheCacheComponent::Exercise1() const
{
    std::vector<std::chrono::milliseconds> sampleTimings{};
    int currentStepIndex = 0;
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        for (int sampleIndex{ 0 }; sampleIndex < m_SampleCount; ++sampleIndex)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i{ 0 }; i < m_SizeIntArray; i += stepSize)
            {
                m_IntArray[i] *= 2;
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            sampleTimings.emplace_back(elapsedTime);
        }
        auto smallest = std::min_element(sampleTimings.begin(), sampleTimings.end());
        sampleTimings.erase(smallest);
        auto biggest = std::max_element(sampleTimings.begin(), sampleTimings.end());
        sampleTimings.erase(biggest);
        auto average = std::accumulate(sampleTimings.begin(), sampleTimings.end(), std::chrono::milliseconds(0)).count() / sampleTimings.size();
        m_StepTimingsInt[currentStepIndex] = float(average);
        ++currentStepIndex;
        sampleTimings.clear();
    }
}

void TrashTheCacheComponent::DrawExercise1Graph() const
{
    ImGui::PlotConfig plotConfig;
    plotConfig.values.xs = m_StepSizes;
    plotConfig.values.ys = m_StepTimingsInt;
    plotConfig.values.count = m_AmountOfSteps;

    plotConfig.scale.min = 0.f;
    plotConfig.scale.max = static_cast<float>(*std::max_element(m_StepTimingsInt, m_StepTimingsInt + m_AmountOfSteps));

    plotConfig.tooltip.show = true;
    plotConfig.tooltip.format = "Step: %g\nTime: %.0f ms";

    plotConfig.frame_size = ImVec2(150, 100);

    plotConfig.grid_y.show = true;
    plotConfig.grid_y.size = plotConfig.scale.max / 10;

    plotConfig.line_thickness = 2.f;

    ImGui::Plot("Int Array", plotConfig);
}

void TrashTheCacheComponent::Exercise2GameObject3D() const
{
    std::vector<std::chrono::milliseconds> sampleTimings{};
    int currentStepIndex = 0;
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        for (int sampleIndex{ 0 }; sampleIndex < m_SampleCount; ++sampleIndex)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i{ 0 }; i < m_SizeGameObject3DArray; i += stepSize)
            {
                m_GameObject3DArray[i].ID *= 2;
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            sampleTimings.emplace_back(elapsedTime);
        }
        auto smallest = std::min_element(sampleTimings.begin(), sampleTimings.end());
        sampleTimings.erase(smallest);
        auto biggest = std::max_element(sampleTimings.begin(), sampleTimings.end());
        sampleTimings.erase(biggest);
        auto average = std::accumulate(sampleTimings.begin(), sampleTimings.end(), std::chrono::milliseconds(0)).count() / sampleTimings.size();
        m_StepTimingsGameObject3D[currentStepIndex] = float(average);
        ++currentStepIndex;
        sampleTimings.clear();
    }
}

void TrashTheCacheComponent::DrawExercise2GameObject3DGraph() const
{
    ImGui::PlotConfig plotConfig;
    plotConfig.values.xs = m_StepSizes;
    plotConfig.values.ys = m_StepTimingsGameObject3D;
    plotConfig.values.count = m_AmountOfSteps;

    plotConfig.scale.min = 0.f;
    plotConfig.scale.max = static_cast<float>(*std::max_element(m_StepTimingsGameObject3D, m_StepTimingsGameObject3D + m_AmountOfSteps));

    plotConfig.tooltip.show = true;
    plotConfig.tooltip.format = "Step: %g\nTime: %.0f ms";

    plotConfig.frame_size = ImVec2(150, 100);

    plotConfig.grid_y.show = true;
    plotConfig.grid_y.size = plotConfig.scale.max / 10;

    plotConfig.line_thickness = 2.f;

    ImGui::Plot("GameObject3D", plotConfig);
}

void TrashTheCacheComponent::Exercise2GameObject3DAlt() const
{
    std::vector<std::chrono::milliseconds> sampleTimings{};
    int currentStepIndex = 0;
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        for (int sampleIndex{ 0 }; sampleIndex < m_SampleCount; ++sampleIndex)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i{ 0 }; i < m_SizeGameObject3DArray; i += stepSize)
            {
                m_GameObject3DAltArray[i].ID *= 2;
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            sampleTimings.emplace_back(elapsedTime);
        }
        auto smallest = std::min_element(sampleTimings.begin(), sampleTimings.end());
        sampleTimings.erase(smallest);
        auto biggest = std::max_element(sampleTimings.begin(), sampleTimings.end());
        sampleTimings.erase(biggest);
        auto average = std::accumulate(sampleTimings.begin(), sampleTimings.end(), std::chrono::milliseconds(0)).count() / sampleTimings.size();
        m_StepTimingsGameObject3DAlt[currentStepIndex] = float(average);
        ++currentStepIndex;
        sampleTimings.clear();
    }
}

void TrashTheCacheComponent::DrawExercise2GameObject3DAltGraph() const
{
    ImGui::PlotConfig plotConfig;
    plotConfig.values.xs = m_StepSizes;
    plotConfig.values.ys = m_StepTimingsGameObject3DAlt;
    plotConfig.values.count = m_AmountOfSteps;

    plotConfig.scale.min = 0.f;
    plotConfig.scale.max = static_cast<float>(*std::max_element(m_StepTimingsGameObject3DAlt, m_StepTimingsGameObject3DAlt + m_AmountOfSteps));

    plotConfig.tooltip.show = true;
    plotConfig.tooltip.format = "Step: %g\nTime: %.0f ms";

    plotConfig.frame_size = ImVec2(150, 100);

    plotConfig.grid_y.show = true;
    plotConfig.grid_y.size = plotConfig.scale.max / 10;

    plotConfig.line_thickness = 2.f;

    ImGui::Plot("GameObject3DAlt", plotConfig);
}

void TrashTheCacheComponent::DrawExercise2Combined() const
{
    ImGui::PlotConfig plotConfig;
    plotConfig.values.xs = m_StepSizes;
    float gameObject3DTimings[11];
    float gameObject3DAltTimings[11];
    for (int index{0}; index < m_AmountOfSteps; ++index)
    {
        gameObject3DTimings[index] = m_StepTimingsGameObject3D[index];
        gameObject3DAltTimings[index] = m_StepTimingsGameObject3DAlt[index];
    }
    const float* data_y[] = {gameObject3DTimings , gameObject3DAltTimings};
    plotConfig.values.ys_list = data_y;

    plotConfig.values.ys_count = 2;
    plotConfig.values.ys_count = 2;
    plotConfig.values.count = m_AmountOfSteps;

    plotConfig.scale.min = 0.f;
    float maxGameObject3D = static_cast<float>(*std::max_element(m_StepTimingsGameObject3D, m_StepTimingsGameObject3D + m_AmountOfSteps));
    float maxGameObject3DAlt = static_cast<float>(*std::max_element(m_StepTimingsGameObject3DAlt, m_StepTimingsGameObject3DAlt + m_AmountOfSteps));
    plotConfig.scale.max = (maxGameObject3D > maxGameObject3DAlt) ?maxGameObject3D : maxGameObject3DAlt;

    plotConfig.tooltip.show = true;
    plotConfig.tooltip.format = "Step: %g\nTime: %.0f ms";

    plotConfig.frame_size = ImVec2(150, 100);

    plotConfig.grid_y.show = true;
    plotConfig.grid_y.size = plotConfig.scale.max / 10;

    plotConfig.line_thickness = 2.f;

    ImGui::Plot("CombinedPlot", plotConfig);
}
