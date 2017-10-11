#include <Ace/Collidable2D.h>
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
    
    
    
    
    
    // TEMP
    using CVT = Collidable2D::CVT;
    
    std::vector<CVT> GetNormals(const Collidable2D& c)
    {
        const std::vector<CVT> vertices(c.GetVertices());
        const UInt8 size = static_cast<UInt8>(vertices.size());
        std::vector<CVT> normals(size);
        for (UInt8 i = 0u; i < size; ++i)
        {
            // TODO: direction checks, normalization?
            //const CVT edge((vertices[i] - vertices[(i + 1u) < size ? (i + 1u) : 0u]).Normalize());
            const CVT edge(vertices[i] - vertices[(i + 1u) < size ? (i + 1u) : 0u]);
            normals[i].x = -edge.y;
            normals[i].y = edge.x;
        }
        return normals;
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
    
    Vector2 ProjectAxis(const CVT& axis, const std::vector<CVT>& vertices)
    {
        float min = CVT::Dot(axis, vertices[0]);
        float max = min;
        for (UInt8 i = 1u; i < vertices.size(); ++i)
        {
            const float cur = CVT::Dot(axis, vertices[i]);
            if (cur < min) min = cur;
            else if (max < cur) max = cur;
        }
        return { min, max };
    }

    bool IsCollidingCircle(const Circle& c, const Collidable2D& other, const std::vector<CVT>& otherVertices)
    {
        const float radiusSquared = c.GetRadius() * c.GetRadius();
        const CVT& center = c.GetPosition();
        CVT vertex = otherVertices.back();
        float nearestDistance = std::numeric_limits<float>::max();
        bool nearestIsInside = false;
        Int32 nearestVertexIndex = -1;
        bool lastIsInside = false;
        for (UInt8 i = 0u; i < otherVertices.size(); ++i)
        {
            const CVT& nextVertex(otherVertices[i]);
            CVT axis(center - vertex);
            const float distance = axis.LengthSquared() - radiusSquared;
            if (distance <= 0.f) return true;
            bool isInside = false;
            const CVT edge(nextVertex - vertex);
            const float edgeLengthSquared = edge.LengthSquared();
            if (!math::IsNearEpsilon(edgeLengthSquared))
            {
                const float dot = CVT::Dot(edge, axis);
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


    bool Collidable2D::IsColliding(const Collidable2D& a, const Collidable2D& b)
    {
        const std::vector<CVT> verticesA(a.GetVertices());
        const std::vector<CVT> verticesB(b.GetVertices());
        const std::vector<CVT> normalsA(GetNormals(a));
        const std::vector<CVT> normalsB(GetNormals(b));
        
        if (normalsA.empty() && normalsB.empty()) // Both are circles
        {
            const float radius = static_cast<const Circle&>(a).GetRadius() + static_cast<const Circle&>(b).GetRadius();
            return (a.GetPosition() - b.GetPosition()).LengthSquared() <= (radius * radius);
        }
        else if (normalsA.empty()) // A is a circle
        {
            return IsCollidingCircle(static_cast<const Circle&>(a), b, verticesB);
        }
        else if (normalsB.empty()) // B is a circle
        {
            return IsCollidingCircle(static_cast<const Circle&>(b), a, verticesA);
        }
        else
        {
            for (const auto& n : normalsA) if (!IsOverlapping(ProjectAxis(n, verticesA), ProjectAxis(n, verticesB))) return false; // SA found
            for (const auto& n : normalsB) if (!IsOverlapping(ProjectAxis(n, verticesA), ProjectAxis(n, verticesB))) return false; // SA found
        }
        return true; // No non-overlaps found, must be colliding
    }
    
    Collidable2D::Collidable2D(const Vector2& position) :
    m_position(position)
    // m_rotation()
    {
        
    }
    
    Collidable2D::~Collidable2D()
    {
        
    }
    
    #if 0
    
    // Same
    
    // C-C
    template<> bool Collidable2D::IsColliding<Circle, Circle>(const Circle& a, const Circle& b)
    {
        //return (a.GetPosition() - b.GetPosition()).Length() <= (a.GetRadius() + b.GetRadius());
        // TODO: is this valid way to bypass sqrt
        const float r = a.GetRadius() + b.GetRadius();
        return (a.GetPosition() - b.GetPosition()).LengthSquared() <= (r * r);
    }
    // R-R
    template<> bool Collidable2D::IsColliding<Rectangle, Rectangle>(const Rectangle& a, const Rectangle& b)
    {
        return SAT(a, b);
    }
    // T-T
    template<> bool Collidable2D::IsColliding<Triangle, Triangle>(const Triangle& a, const Triangle& b)
    {
        return false;
    }
    
    // Different
    
    // C-R
    template<> bool Collidable2D::IsColliding<Circle, Rectangle>(const Circle& a, const Rectangle& b)
    {
        return false;
    }
    template<> bool Collidable2D::IsColliding<Rectangle, Circle>(const Rectangle& a, const Circle& b)
    {
        return IsColliding(b, a);
    }
    
    // C-T
    template<> bool Collidable2D::IsColliding<Circle, Triangle>(const Circle& a, const Triangle& b)
    {
        return false;
    }
    template<> bool Collidable2D::IsColliding<Triangle, Circle>(const Triangle& a, const Circle& b)
    {
        return IsColliding(b, a);
    }
    
    // R-T
    template<> bool Collidable2D::IsColliding<Rectangle, Triangle>(const Rectangle& a, const Triangle& b)
    {
        return false;
    }
    template<> bool Collidable2D::IsColliding<Triangle, Rectangle>(const Triangle& a, const Rectangle& b)
    {
        return IsColliding(b, a);
    }
    
    #endif
    
    Circle::Circle(const Vector2& position, const float radius) :
    Collidable2D(position), m_radius(math::Abs(radius))
    {
        if (math::IsNearEpsilon(radius))
        Logger::Log(Logger::Priority::Warning, "Collidable2D: Circle: Radius near epsilon: %f", radius);
    }
    
    bool Circle::IsColliding(const Vector2& point) const
    {
        const Vector2 d = point - m_position;
        return (d.x*d.x + d.y*d.y) <= (m_radius * m_radius);
    }
    
    std::vector<CVT> Circle::GetVertices() const
    {
        return { };
    }
    
    
    Rectangle::Rectangle(const Vector2& position, const Vector2& extents) :
    Collidable2D(position), m_extents(extents)
    {
        
    }
    
    bool Rectangle::IsColliding(const Vector2& point) const
    {
        const Vector2 pos = m_position + m_extents;
        const Vector2 neg = m_position + m_extents.Invert();
        return IsInTriangle(
            point,
            pos,
            m_position + Vector2(-m_extents.x, m_extents.y),
            neg
        ) || IsInTriangle(
            point,
            neg,
            m_position + Vector2(m_extents.x, -m_extents.y),
            pos
        );
    }
    
    std::vector<CVT> Rectangle::GetVertices() const
    {
        const CVT& ex = GetExtents();
        const CVT& pos = GetPosition();
        return {
            pos + CVT{-ex.x, -ex.y},
            pos + CVT{ ex.x, -ex.y},
            pos + ex,
            pos + CVT{-ex.x,  ex.y}
        };
    }
    
    
    Triangle::Triangle(const Vector2& position, const Vector2 (&extents)[3u]) :
    Collidable2D(position), m_extents{ extents[0], extents[1], extents[2] }
    {
        
    }
    
    bool Triangle::IsColliding(const Vector2& point) const
    {
        return IsInTriangle(
            point,
            m_position + m_extents[0],
            m_position + m_extents[1],
            m_position + m_extents[2]
        );
    }
    
    std::vector<CVT> Triangle::GetVertices() const
    {
        const auto& ex = GetExtents();
        const CVT& pos = GetPosition();
        return {
            pos + ex[0],
            pos + ex[1],
            pos + ex[2]
        };
    }
    
}