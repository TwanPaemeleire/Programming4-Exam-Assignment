#include "ThrashTheCacheComponent.h"
#include <algorithm>
#include <chrono>
#include <numeric>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_plot.h"

ThrashTheCacheComponent::ThrashTheCacheComponent(GameObject* owner)
	: Component(owner), m_SizeIntArray{ int(pow(2, 26)) }, m_SizeGameObject3DArray{int(pow(2, 23))}, m_IntGraphColor{ImColor(255, 0, 0)}
{
    m_IntArray = new int[m_SizeIntArray]{};
    m_GameObject3DArray = new GameObject3D[m_SizeGameObject3DArray]{};
    m_GameObject3DAltArray = new GameObject3DAlt[m_SizeGameObject3DArray]{};
    m_GameObjectColors[0] = ImColor(0, 255, 0);
    m_GameObjectColors[1] = ImColor(0, 0, 255);
}

ThrashTheCacheComponent::~ThrashTheCacheComponent()
{
    delete[] m_IntArray;
    delete[] m_GameObject3DArray;
    delete[] m_GameObject3DAltArray;
}

void ThrashTheCacheComponent::RenderUI()
{
    // ----- EXERCISE 1 -----
    if (ImGui::Begin("Exercise 1"))
    {
        ImGui::PushItemWidth(100);
        ImGui::InputInt("# samples", &m_SampleCountInt);
        ImGui::PopItemWidth();

        if (m_IntButtonClicked)
        {
            CalculateIntTimings();
        }
        if (!m_IntButtonClicked && ImGui::Button("Trash The Cache"))
        {
            m_IntButtonClicked = true;
            m_ShowIntGraph = false;
            ImGui::Text("Wait For It...");
        }
        if (m_ShowIntGraph)
        {
            DrawExercise1Graph();
        }

        ImGui::End();
    }

    // ----- EXERCISE 2 -----
    if (ImGui::Begin("Exercise 2"))
    {
        ImGui::PushItemWidth(100);
        ImGui::InputInt("# samples", &m_SampleCountGameObject3D);
        ImGui::PopItemWidth();

        // ----- GameObject3D -----
        if (m_GameObject3DButtonClicked)
        {
            CalculateGameObject3DTimings();
        }

        if (!m_GameObject3DButtonClicked && ImGui::Button("Trash The Cache With GameObject3D"))
        {
            m_GameObject3DButtonClicked = true;
            m_ShowGameObject3DGraph = false;
            ImGui::Text("Wait For It...");
        }
        if (m_ShowGameObject3DGraph)
        {
            DrawGameObject3DGraph();
        }

        // ----- GameObject3DAlt -----
        if (m_GameObject3DAltButtonClicked)
        {
            CalculateGameObject3DAltTimings();
        }

        if (!m_GameObject3DAltButtonClicked && ImGui::Button("Trash The Cache With GameObject3DAlt"))
        {
            m_GameObject3DAltButtonClicked = true;
            m_ShowGameObject3DAltGraph = false;
            ImGui::Text("Wait For It...");
        }
        if (m_ShowGameObject3DAltGraph)
        {
            DrawGameObject3DAltGraph();
        }

        // ----- Combined -----
        if (m_ShowGameObject3DGraph && m_ShowGameObject3DAltGraph)
        {
            ImGui::Text("Combined: ");
            DrawExercise2CombinedGraph();
        }

        ImGui::End();
    }
}

void ThrashTheCacheComponent::CalculateIntTimings()
{
    if (m_SampleCountInt == 0) return; // Avoid Zero-Division
    std::vector<std::chrono::milliseconds> sampleTimings{};
    int currentStepIndex = 0;

    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        for (int sampleIndex{ 0 }; sampleIndex < m_SampleCountInt; ++sampleIndex)
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
        if (m_SampleCountInt > 2) // Avoid Zero-Division
        {
            auto smallest = std::min_element(sampleTimings.begin(), sampleTimings.end());
            sampleTimings.erase(smallest);
            auto biggest = std::max_element(sampleTimings.begin(), sampleTimings.end());
            sampleTimings.erase(biggest);
        }
        auto average = std::accumulate(sampleTimings.begin(), sampleTimings.end(), std::chrono::milliseconds(0)).count() / sampleTimings.size();
        m_StepTimingsInt[currentStepIndex] = float(average);
        
        ++currentStepIndex;
        sampleTimings.clear();
    }
    m_IntButtonClicked = false;
    m_ShowIntGraph = true;
}

void ThrashTheCacheComponent::DrawExercise1Graph() const
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
    plotConfig.values.color = m_IntGraphColor;

    ImGui::Plot("Int Array", plotConfig);
}

void ThrashTheCacheComponent::CalculateGameObject3DTimings()
{
    if (m_SampleCountGameObject3D == 0) return; // Avoid Zero-Division
    std::vector<std::chrono::milliseconds> sampleTimings{};
    int currentStepIndex = 0;
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        for (int sampleIndex{ 0 }; sampleIndex < m_SampleCountGameObject3D; ++sampleIndex)
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
        if (m_SampleCountGameObject3D > 2) // Avoid Zero-Division
        {
            auto smallest = std::min_element(sampleTimings.begin(), sampleTimings.end());
            sampleTimings.erase(smallest);
            auto biggest = std::max_element(sampleTimings.begin(), sampleTimings.end());
            sampleTimings.erase(biggest);
        }
        auto average = std::accumulate(sampleTimings.begin(), sampleTimings.end(), std::chrono::milliseconds(0)).count() / sampleTimings.size();
        m_StepTimingsGameObject3D[currentStepIndex] = float(average);
        ++currentStepIndex;
        sampleTimings.clear();
    }

    m_GameObject3DButtonClicked = false;
    m_ShowGameObject3DGraph = true;
}

void ThrashTheCacheComponent::DrawGameObject3DGraph() const
{
    if (!m_ShowGameObject3DGraph) return;
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
    plotConfig.values.color = m_GameObjectColors[0];

    ImGui::Plot("GameObject3D", plotConfig);
}

void ThrashTheCacheComponent::CalculateGameObject3DAltTimings()
{
    if (m_SampleCountGameObject3D == 0) return; // Avoid Zero-Division
    std::vector<std::chrono::milliseconds> sampleTimings{};
    int currentStepIndex = 0;
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        for (int sampleIndex{ 0 }; sampleIndex < m_SampleCountGameObject3D; ++sampleIndex)
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
        if (m_SampleCountGameObject3D > 2) // Avoid Zero-Division
        {
            auto smallest = std::min_element(sampleTimings.begin(), sampleTimings.end());
            sampleTimings.erase(smallest);
            auto biggest = std::max_element(sampleTimings.begin(), sampleTimings.end());
            sampleTimings.erase(biggest);
        }
        auto average = std::accumulate(sampleTimings.begin(), sampleTimings.end(), std::chrono::milliseconds(0)).count() / sampleTimings.size();
        m_StepTimingsGameObject3DAlt[currentStepIndex] = float(average);
        ++currentStepIndex;
        sampleTimings.clear();
    }
    m_GameObject3DAltButtonClicked = false;
    m_ShowGameObject3DAltGraph = true;
}

void ThrashTheCacheComponent::DrawGameObject3DAltGraph() const
{
    if (!m_ShowGameObject3DAltGraph) return;
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
    plotConfig.values.color = m_GameObjectColors[1];

    ImGui::Plot("GameObject3DAlt", plotConfig);
}

void ThrashTheCacheComponent::DrawExercise2CombinedGraph() const
{
    if (!(m_ShowGameObject3DGraph && m_ShowGameObject3DAltGraph)) return;
    ImGui::PlotConfig plotConfig;
    plotConfig.values.xs = m_StepSizes;
    const float* data_y[] = { m_StepTimingsGameObject3D , m_StepTimingsGameObject3DAlt };
    plotConfig.values.ys_list = data_y;

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
    plotConfig.values.colors = m_GameObjectColors;

    ImGui::Plot("CombinedPlot", plotConfig);
}
