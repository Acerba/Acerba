
#include <Ace/Ace.h>

struct Mesh
{
	struct MeshImpl;
	
	std::shared_ptr<MeshImpl> impl; // CPP
	Buffer buffers[2];
};

void MeshAddBuffer(Mesh& mesh, ...)
{
	//mesh.impl->AddBuffer(...);
	
	mesh.buffers[0] = ...;
	//glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[0]->ID);
	
}

// C++ 11

// Doxygen comment Example.
/// Clear Enum
enum class Clear
{
	Color	= 0x1, // 0x2, 0x4, 0x8, 0x10, 0x20, 0x40,
	Depth	= 1 << 1,
	Stencil	= 4,
};

/*
	C 001	
	D 010
	S 100
	
	C | D
	011
	
	C | S
	101
	
	if(flags & Clear::Color == Clear::Color)
	{
		
	}
	
*/

ACE_ENUM(Clear)

ace::Mesh g_mesh; // Null

struct Renderer
{
	Clear clearFlags;

	static Renderer* currentRenderer;
	
	private:
	
	int m_genericValue;
	
	void DoSomething()
	{
		
	}
};

int main(int, char**)
{
	ace::Window win( ... );
	ace::Renderer ren( win );
	
	ace::Renderer::Init(win);

	g_mesh = ace::Mesh();
	g_mesh = ren.CreateMesh();
	g_mesh = ace::Renderer::CreateMesh();
	
	g_mesh.AddBuffer( ... );
	ace::Renderer::MeshAddBuffer(g_mesh, ...);
	
	ace::Renderer::SetVertexBuffer(...);
	ace::Renderer::SetIndexBuffer(...);
	
	
	
	return 0;
}