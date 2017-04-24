
#include <Ace/Module.h>
#include <Ace/Time.h>
#include <Ace/UserInterface.h>
#include <Ace/Window.h>

//For ImGui functions
#include <imgui.h>

#include <string>


int main(int, char**)
{
    static const ace::Color clearColor(127u, 127u, 127u, 0u);

    //Initialize Acerba
    ace::Init();

    //Create window
    ace::Window window("Main", 480, 320);

    //Initialize UI
    ace::UI::Init(window);

    //Create timer object
    const ace::Time timer;
    
    //Main loop
    while (window)
    {
        //Update Acerba core systems
        ace::Update();

        //Clear window
        window.Clear(clearColor);

        //UI begins
        //All UI groups must be between this and EndUI functions
        ace::UserInterface::BeginUI();

        //UI group begins
        //If an element (bar, button, text) is created outside of a UI group, a default ImGui window will be created for it
        ace::UI::BeginGroup(ace::Vector2(200.f, 200.f), ace::Vector2(90.f, 20.f));
        //UI text element
        ace::UI::Text("Hello %s", "world 2");
        //UI button
        if (ace::UI::Button(ace::Vector2(100.f, 20.f), "Close"))
        {
            //Closes the window when clicked
            window.Close();
        }
        //UI group ends
        ace::UI::EndGroup();

        //UI group without comments for easier reading
        ace::UI::BeginGroup(ace::Vector2(200.f, 200.f), ace::Vector2(50.f, 60.f));
        ace::UI::Text("Hello %s", "world 1");
        ace::UI::Bar(timer.DeltaTime(), ace::Vector2(250.f, 15.f), std::string("Deltatime: " + std::to_string(timer.DeltaTime())).c_str());
        ace::UI::EndGroup();

        //UI group begins (ImGui style)
        //The default ImGui window can be moved and resized
        ImGui::Begin("ImGui window name");
        //UI text element (ImGui style)
        ImGui::Text("Hello %s", "ImGui");
        //UI progress bar (ImGui style)
        ImGui::ProgressBar(timer.DeltaTime(), ImVec2(250.f, 15.f), std::string("Deltatime: " + std::to_string(timer.DeltaTime())).c_str());
        //UI group ends (imGui style)
        ImGui::End();

        //UI ends
        ace::UserInterface::EndUI();

        //Render window
        window.Present();
    }

    //Terminate acerba
    ace::Quit();

    return 0;
}