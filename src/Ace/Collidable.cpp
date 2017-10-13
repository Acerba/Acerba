#include <Ace/Collidable.h>
#include <Ace/Math.h>
#include <Ace/Log.h>

#include <limits> // numeric_limits::max()

#include <Ace/Debugger.h>

namespace ace
{
    
    bool IsInTriangle(
        const Vector2& p,
        const Vector2& a,
        const Vector2& b,
        const Vector2& c
    )
    {
        const Vector2 v0 = c - a;
        const Vector2 v1 = b - a;
        const Vector2 v2 = p - a;
        
        const float d00 = Vector2::Dot(v0, v0);
        const float d01 = Vector2::Dot(v0, v1);
        const float d02 = Vector2::Dot(v0, v2);
        const float d11 = Vector2::Dot(v1, v1);
        const float d12 = Vector2::Dot(v1, v2);
        
        const float id = 1.f / (d00 * d11 - d01 * d01); // TODO: add div-by-zero check?
        const float u = (d11 * d02 - d01 * d12) * id;
        const float v = (d00 * d12 - d01 * d02) * id;
        return (u >= 0.f) && (v >= 0.f) && ((u + v) < 1.f);
    }

    std::vector<Vector2> GetNormalsImpl(const std::vector<Vector2>& vertices)
    {
        const UInt8 size = static_cast<UInt8>(vertices.size());
        std::vector<Vector2> normals(size);
        for (UInt8 i = 0u; i < size; ++i)
        {
            // TODO: direction checks, normalization?
            //const Vector2 edge((vertices[i] - vertices[(i + 1u) < size ? (i + 1u) : 0u]).Normalize());
            const Vector2 edge(vertices[i] - vertices[(i + 1u) < size ? (i + 1u) : 0u]);
            normals[i].x = -edge.y;
            normals[i].y = edge.x;
        }
        return normals;
    }

    std::vector<Vector2> GetNormals(const Collidable& c)
    {
        return GetNormalsImpl(c.GetVertices());
    }

    std::vector<Vector2> GetNormals(const std::vector<Vector2>& vertices)
    {
        return GetNormalsImpl(vertices);
    }
    
    bool IsOverlapping(const Vector2& a, const Vector2& b)
    {
        return
            math::IsBetween(a.x, b.x, b.y) ||
            math::IsBetween(a.y, b.x, b.y) ||
            math::IsBetween(b.x, a.x, a.y) ||
            math::IsBetween(b.y, a.x, a.y);
            //(b.x < a.x && a.x < b.y) ||
            //(b.x < a.y && a.y < b.y) ||
    }

    Vector2 ProjectAxis(const Vector2& axis, const std::vector<Vector2>& vertices)
    {
        float min = Vector2::Dot(axis, vertices[0]);
        float max = min;
        for (UInt8 i = 1u; i < vertices.size(); ++i)
        {
            const float cur = Vector2::Dot(axis, vertices[i]);
            if (cur < min) min = cur;
            else if (max < cur) max = cur;
        }
        return { min, max };
    }

    bool IsCollidingCircle(const Circle& c, const std::vector<Vector2>& otherVertices)
    {
        const float radiusSquared = c.GetRadius() * c.GetRadius();
        const Vector2 center(c.GetLocalPosition());
        Vector2 vertex = otherVertices.back();
        float nearestDistance = std::numeric_limits<float>::max();
        bool nearestIsInside = false;
        Int32 nearestVertexIndex = -1;
        bool lastIsInside = false;
        for (UInt8 i = 0u; i < otherVertices.size(); ++i)
        {
            const Vector2& nextVertex(otherVertices[i]);
            Vector2 axis(center - vertex);
            const float distance = axis.LengthSquared() - radiusSquared;
            if (distance <= 0.f) return true;
            bool isInside = false;
            const Vector2 edge(nextVertex - vertex);
            const float edgeLengthSquared = edge.LengthSquared();
            if (!math::IsNearEpsilon(edgeLengthSquared))
            {
                const float dot = Vector2::Dot(edge, axis);
                if (dot >= 0.f && dot <= edgeLengthSquared)
                {
                    axis = (vertex + (edge * (dot / edgeLengthSquared))) - center;
                    if (axis.LengthSquared() <= radiusSquared) return true;
                    if (edge.x > 0.f && axis.y > 0.f) return false;
                    if (edge.x < 0.f && axis.y < 0.f) return false;
                    if (edge.y > 0.f && axis.x < 0.f) return false;
                    if (axis.x > 0.f) return false;
                    isInside = true;
                }
            }
            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestIsInside = isInside || lastIsInside;
                nearestVertexIndex = i;
            }
            vertex = nextVertex;
            lastIsInside = isInside;
        }
        return (nearestVertexIndex == 0u) ? (nearestIsInside || lastIsInside) : nearestIsInside;
    }


    bool Collidable::IsColliding(const Collidable& a, const Collidable& b)
    {
        const std::vector<Vector2> verticesA(a.GetVertices());
        const std::vector<Vector2> verticesB(b.GetVertices());
        const std::vector<Vector2> normalsA(GetNormals(verticesA));
        const std::vector<Vector2> normalsB(GetNormals(verticesB));
        
        if (normalsA.empty() && normalsB.empty()) // Both are circles
        {
            const float radius = static_cast<const Circle&>(a).GetRadius() + static_cast<const Circle&>(b).GetRadius();
            return (a.GetGlobalPosition() - b.GetGlobalPosition()).LengthSquared() <= (radius * radius);
        }
        else if (normalsA.empty()) // A is a circle
        {
            return IsCollidingCircle(static_cast<const Circle&>(a), verticesB);
        }
        else if (normalsB.empty()) // B is a circle
        {
            return IsCollidingCircle(static_cast<const Circle&>(b), verticesA);
        }
        else
        {
            for (const auto& n : normalsA) if (!IsOverlapping(ProjectAxis(n, verticesA), ProjectAxis(n, verticesB))) return false; // SA found
            for (const auto& n : normalsB) if (!IsOverlapping(ProjectAxis(n, verticesA), ProjectAxis(n, verticesB))) return false; // SA found
        }
        return true; // No non-overlaps found, must be colliding
    }
    
    Collidable::Collidable(const Vector2& position, const Matrix2& rotation) :
        m_rotation(rotation), m_position(position)
    {
        
    }
    
    Collidable::~Collidable()
    {
        
    }

    Circle::Circle(const float radius, const Vector2& position, const Matrix2& rotation) :
        Collidable(position, rotation), m_radius(math::Abs(radius))
    {
        if (math::IsNearEpsilon(radius))
            Logger::Log(Logger::Priority::Warning, "Collidable: Circle: Radius near epsilon: %f", radius);
    }

    bool Circle::IsColliding(const Vector2& point) const
    {
        return (point - GetGlobalPosition()).LengthSquared() <= (m_radius * m_radius);
    }

    std::vector<Vector2> Circle::GetVertices() const
    {
        return { };
    }

    Rectangle::Rectangle(const Vector2& extents, const Vector2& position, const Matrix2& rotation) :
        Collidable(position, rotation), m_extents(extents)
    {
        
    }
    
    bool Rectangle::IsColliding(const Vector2& point) const
    {
        const Vector2 pos = m_rotation * (m_position + m_extents);
        const Vector2 neg = m_rotation * (m_position + m_extents.Invert());
        return IsInTriangle(
            point,
            pos,
            m_rotation * (m_position + Vector2(-m_extents.x, m_extents.y)),
            neg
        ) || IsInTriangle(
            point,
            neg,
            m_rotation * (m_position + Vector2(m_extents.x, -m_extents.y)),
            pos
        );
    }
    
    std::vector<Vector2> Rectangle::GetVertices() const
    {
        return {
            m_rotation * (m_position + Vector2{-m_extents.x, -m_extents.y}),
            m_rotation * (m_position + Vector2{ m_extents.x, -m_extents.y}),
            m_rotation * (m_position + m_extents),
            m_rotation * (m_position + Vector2{-m_extents.x,  m_extents.y})
        };
    }
    
    
    Triangle::Triangle(const Vector2 (&extents)[3u], const Vector2& position, const Matrix2& rotation) :
        Collidable(position, rotation), m_extents{ extents[0], extents[1], extents[2] }
    {
        
    }
    
    bool Triangle::IsColliding(const Vector2& point) const
    {
        return IsInTriangle(
            point,
            m_rotation * (m_position + m_extents[0]),
            m_rotation * (m_position + m_extents[1]),
            m_rotation * (m_position + m_extents[2])
        );
    }
    
    std::vector<Vector2> Triangle::GetVertices() const
    {
        return {
            m_position + (m_rotation * m_extents[0]),
            m_position + (m_rotation * m_extents[1]),
            m_position + (m_rotation * m_extents[2])
        };
    }
    
}