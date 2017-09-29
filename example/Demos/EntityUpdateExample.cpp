// Include all necessary Acerba files
#include <Ace/Ace.h>

#include <iostream>
#include <string>

class MyComponent
{
    // Member data
    std::string m_message;

public:

    // Constructor
    MyComponent(const std::string& str) : m_message(str) { }

    // Retrieve member data
    const std::string& getMessage() const
    {
        return m_message;
    }

    // Member function
    static void myMemberUpdateFunc(MyComponent& comp)
    {
        std::cout << comp.m_message << " says 'Hello' from member function.\n";
    }

};

// Free function
void myUpdateFunc(MyComponent& comp)
{
    std::cout << comp.getMessage() << " says 'Hello' from free function.\n";
}

void myEntityUpdateFunc(MyComponent& comp, ace::EntityHandle* handle)
{
    std::cout << "Entity Handle: " << comp.getMessage() << " says 'Hello' from free function.\n";
}

int main(int, char**)
{
    // Initialize Acerba
    ace::Init();

    // Create an entity
    ace::Entity myEntity;

    // Create a custom component
    MyComponent myComp("Comp1");

    // Add a component to the entity
    myEntity.AddComponent(myComp);

    // Set a custom update function
    myEntity->manager->SetUpdateCallback(myUpdateFunc);
    myEntity->manager->SetUpdateCallback(myEntityUpdateFunc); // Sets EntityHandle Callback.

    // Update the entity
    ace::Update();

    // Set another custom update function (overrides the previous)
    myEntity->manager->SetUpdateCallback(MyComponent::myMemberUpdateFunc);

    // Update the entity again
    ace::Update();

    // Shutdown Acerba
    ace::Quit();

    return 0;
}
