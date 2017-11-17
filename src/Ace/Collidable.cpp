#include <Ace/Collidable.h>
#include <Ace/CollidableImpl.h>
#include <Ace/BVH.h>
#include <Ace/Log.h>
#include <Ace/Math.h>
#include <Ace/UUID.h>

#include <limits> // std::numeric_limits::max()

namespace ace
{
    
    using Matrix2 = Collidable::Matrix2;
    using Vector2 = Collidable::Vector2;
    
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
        
        const float d00 = math::Dot(v0, v0);
        const float d01 = math::Dot(v0, v1);
        const float d02 = math::Dot(v0, v2);
        const float d11 = math::Dot(v1, v1);
        const float d12 = math::Dot(v1, v2);
        
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
    }
    
    Vector2 ProjectAxis(const Vector2& axis, const std::vector<Vector2>& vertices)
    {
        float min = math::Dot(axis, vertices[0]);
        float max = min;
        for (UInt8 i = 1u; i < vertices.size(); ++i)
        {
            const float cur = math::Dot(axis, vertices[i]);
            if (cur < min) min = cur;
            else if (max < cur) max = cur;
        }
        return Vector2(min, max);
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
            const float distance = axis.lengthSquared() - radiusSquared;
            if (distance <= 0.f) return true;
            bool isInside = false;
            const Vector2 edge(nextVertex - vertex);
            const float edgeLengthSquared = edge.lengthSquared();
            if (!math::IsNearEpsilon(edgeLengthSquared))
            {
                const float dot = math::Dot(edge, axis);
                if (dot >= 0.f && dot <= edgeLengthSquared)
                {
                    axis = (vertex + (edge * (dot / edgeLengthSquared))) - center;
                    if (axis.lengthSquared() <= radiusSquared) return true;
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
    
    
    
    
    Collidable::Collidable(const Vector2& position, const Matrix2& rotation) :
    m_impl(BVH::AddCollidable(position, rotation)), m_id(UUID<void>::GetID()), m_mask(Mask::A)
    {
        // Don't call SetOwner here. Derived class not yet created.
    }
    
    Collidable& Collidable::operator=(const Collidable& other)
    {
        m_mask = other.m_mask;
        m_impl = other.m_impl;
        return *this;
    }
    
    Collidable::~Collidable()
    {
        // Disabled for copy-assigment
        // TODO: make m_impl shared_ptr?
        // m_impl.Destroy();
    }
    
    // Vector2 Collidable::GetGlobalPosition() const { return m_rotation * m_position; }
    
    const AABB& Collidable::GetAABB() const
    {
        return m_impl.GetAABB();
    }
    
    UInt32 Collidable::GetCollisionCount() const
    {
        return m_impl.GetCollisionCount();
    }
    
    const std::vector<CollidableImpl*>& Collidable::GetCollisions() const
    {
        return m_impl.GetCollisions();
    }
    
    const Vector2& Collidable::GetLocalPosition() const
    {
        return m_impl.GetLocalPosition();
    }
    Vector2& Collidable::GetLocalPosition()
    {
        return m_impl.GetLocalPosition();
    }
    
    const Matrix2& Collidable::GetRotation() const
    {
        return m_impl.GetRotation();
    }
    Matrix2& Collidable::GetRotation()
    {
        return m_impl.GetRotation();
    }
    
    bool Collidable::HasChanged() const
    {
        return m_impl.HasChanged();
    }
    
    bool Collidable::HasCollision(const Collidable& other) const
    {
        return m_impl.HasCollision(other.GetID());
    }
    
    bool Collidable::IsColliding(const Collidable& a, const Collidable& b)
    {
        if (!a.HasMask(b))
        {
            return false;
        }
        
        const std::vector<Vector2> verticesA(a.GetVertices());
        const std::vector<Vector2> verticesB(b.GetVertices());
        const std::vector<Vector2> normalsA(GetNormals(verticesA));
        const std::vector<Vector2> normalsB(GetNormals(verticesB));
        
        if (normalsA.empty() && normalsB.empty()) // Both are circles
        {
            const float radius = static_cast<const Circle&>(a).GetRadius() + static_cast<const Circle&>(b).GetRadius();
            return (a.GetLocalPosition() - b.GetLocalPosition()).lengthSquared() <= (radius * radius);
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
    
    void Collidable::Reserve(const UInt32 size)
    {
        BVH::Reserve(size);
    }
    
    void Collidable::ResetCollisions()
    {
        m_impl.ResetCollisions();
    }
    
    void Collidable::SetHasChanged(const bool value)
    {
        m_impl.SetHasChanged(value);
    }
    
    void Collidable::SetMask(const UInt8 mask)
    {
        m_mask = mask;
    }
    
    void Collidable::UpdateCollisions()
    {
        UpdateAABB();
        BVH::UpdateCollisions(*this);
    }
    
    
    
    
    
    Circle::Circle(const float radius, const Vector2& position, const Matrix2& rotation) :
    Collidable(position, rotation), m_radius(math::Abs(radius))
    {
        if (math::IsNearEpsilon(radius))
        Logger::Log(Logger::Priority::Warning, "Collidable: Circle: Radius near epsilon: %f", radius);
        
        m_impl.SetOwner(this);
    }
    
    std::vector<Vector2> Circle::GetVertices() const
    {
        return { };
    }
    
    bool Circle::IsColliding(const Vector2& point) const
    {
        return (point - GetLocalPosition()).lengthSquared() <= (m_radius * m_radius);
    }
    
    void Circle::Rotate(float)
    {
        return;
    }
    
    void Circle::UpdateAABB(const bool)
    {
        AABB& aabb = m_impl.GetAABB();
        const Vector2& pos = GetLocalPosition();
        aabb.min.x = pos.x - m_radius;
        aabb.min.y = pos.y - m_radius;
        aabb.max.x = pos.x + m_radius;
        aabb.max.y = pos.y + m_radius;
    }
    
    
    
    
    
    Rectangle::Rectangle(const Vector2& extents, const Vector2& position, const Matrix2& rotation) :
    Collidable(position, rotation), m_extents(extents)
    {
        m_impl.SetOwner(this);
    }
    
    std::vector<Vector2> Rectangle::GetVertices() const
    {
        const Matrix2& rot = GetRotation();
        const Vector2& pos = GetLocalPosition();
        return {
            math::ToVektor(rot * (pos + Vector2{-m_extents.x, -m_extents.y})),
            math::ToVektor(rot * (pos + Vector2{ m_extents.x, -m_extents.y})),
            math::ToVektor(rot * (pos + m_extents)),
            math::ToVektor(rot * (pos + Vector2{-m_extents.x,  m_extents.y}))
        };
    }
    
    bool Rectangle::IsColliding(const Vector2& point) const
    {
        const Matrix2& rot = GetRotation();
        const Vector2& pos = GetLocalPosition();
        const Vector2 expos = math::ToVektor(rot * (pos + m_extents));
        const Vector2 exneg = math::ToVektor(rot * (pos + math::Invert(m_extents)));
        return IsInTriangle(
            point,
            expos,
            math::ToVektor(rot * (pos + Vector2(-m_extents.x, m_extents.y))),
            exneg
        ) || IsInTriangle(
            point,
            exneg,
            math::ToVektor(rot * (pos + Vector2(m_extents.x, -m_extents.y))),
            expos
        );
    }
    
    void Rectangle::Rotate(float deg)
    {
        m_extents = math::ToVektor(math::RotateZ2(deg) * m_extents);
        GetRotation() = math::s_identity2;
        
        UpdateAABB(false);
    }
    
    void Rectangle::UpdateAABB(const bool accountRotation)
    {
        AABB& aabb = m_impl.GetAABB();
        aabb.Reset();
        if (accountRotation)
        {
            for (const auto& vertex : GetVertices())
            {
                aabb.Merge(vertex);
            }
        }
        else
        {
            aabb.Merge(m_impl.GetLocalPosition() + math::Invert(m_extents));
            aabb.Merge(m_impl.GetLocalPosition() + m_extents);
        }
    }
    
    
    
    
    
    Triangle::Triangle(const Vector2 (&extents)[3u], const Vector2& position, const Matrix2& rotation) :
    Collidable(position, rotation), m_extents{ extents[0], extents[1], extents[2] }
    {
        m_impl.SetOwner(this);
    }
    
    std::vector<Vector2> Triangle::GetVertices() const
    {
        const Matrix2& rot = GetRotation();
        const Vector2& pos = GetLocalPosition();
        return {
            pos + math::ToVektor(rot * m_extents[0]),
            pos + math::ToVektor(rot * m_extents[1]),
            pos + math::ToVektor(rot * m_extents[2])
        };
    }
    
    bool Triangle::IsColliding(const Vector2& point) const
    {
        const Matrix2& rot = GetRotation();
        const Vector2& pos = GetLocalPosition();
        return IsInTriangle(
            point,
            math::ToVektor(rot * (pos + m_extents[0])),
            math::ToVektor(rot * (pos + m_extents[1])),
            math::ToVektor(rot * (pos + m_extents[2]))
        );
    }
    
    void Triangle::Rotate(float deg)
    {
        const Matrix2 rot(math::RotateZ2(deg));
        for (auto& itr : m_extents)
        itr = math::ToVektor(rot * itr);
        
        GetRotation() = math::s_identity2;
        
        UpdateAABB(false);
    }
    
    void Triangle::UpdateAABB(const bool accountRotation)
    {
        AABB& aabb = m_impl.GetAABB();
        aabb.Reset();
        if (accountRotation)
        {
            for (const auto& vertex : GetVertices())
            {
                aabb.Merge(vertex);
            }
        }
        else
        {
            const Vector2& pos = GetLocalPosition();
            aabb.Merge(pos + m_extents[0]);
            aabb.Merge(pos + m_extents[1]);
            aabb.Merge(pos + m_extents[2]);
        }
    }
    
}