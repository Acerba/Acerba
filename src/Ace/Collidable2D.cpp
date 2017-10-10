#include <Ace/Collidable2D.h>
#include <Ace/Math.h>
#include <Ace/Log.h>

#include <array>
#include <vector>

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

            const CVT vertex((vertices[i] - vertices[(i + 1u) < size ? (i + 1u) : 0u]).Normalize());

            normals[i].x = -vertex.y;
            normals[i].y = vertex.x;
            //normals[i].x = -vertices[i].y;
            //normals[i].y = vertices[i].x;
            //LogDebug(vertices[i], "GetNormals Vertex");
            //LogDebug(normals[i], "GetNormals Normal");
        }
        return normals;
    }

    // Project a onto b
    CVT ProjectVector(const CVT& a, const CVT& b)
    {
        //const CVT bn(b.Normalize());
        //return bn * CVT::Dot(a, bn);
        return b * (CVT::Dot(a, b) / b.LengthSquared());
    }

    /*
    void ProjectVectors(CVT& min, CVT& max, const CVT& target, const Collidable2D& c)
    {
        const std::vector<CVT> points(c.GetVertices());
        const CVT centerProj(ProjectVector(c.GetPosition(), target));
        const UInt8 size = points.size();
        if (size == 1u) // Circle
        {
            min = max = centerProj;
            const float radius = dynamic_cast<const Circle*>(&c)->GetRadius();
            CVT curProj(ProjectVector(points[0] + radius, target));
            if (curProj < min) min = curProj;
            if (max < curProj) max = curProj;
            curProj = ProjectVector(points[0] - radius, target);
            if (curProj < min) min = curProj;
            if (max < curProj) max = curProj;
        }
        else // Other shapes
        {
            min = max = ProjectVector(points[0], target);
            for (UInt8 i = 1u; i < size; ++i)
            {
                const CVT curProj(ProjectVector(points[i], target));
                if (curProj < min) min = curProj;
                if (max < curProj) max = curProj;
            }
        }
    }
    */

    /*
    bool ProjectVectors(const CVT& normal, const Collidable2D& a, const Collidable2D& b)
    {
        float maxA = 0.f;
        const CVT centerProjA(ProjectVector(a.GetPosition(), normal));
        const std::vector<CVT> pointsA(a.GetVertices());
        if (pointsA.empty()) // A is a Circle
        {
            maxA = ProjectVector(
                (b.GetPosition() - centerProjA).Normalize() * dynamic_cast<const Circle*>(&a)->GetRadius(),
                normal
            ) - centerProjA).LengthSquared();
        }
        else // Other shapes A
        {
            for (const auto& itr : pointsA)
            {
                const float curLength = ProjectVector(itr, normal).LengthSquared();
                if (curLength > maxA) maxA = curLength;
            }
        }

        float maxB = 0.f;
        const CVT centerProjB(ProjectVector(a.GetPosition(), normal));
        const std::vector<CVT> pointsB(b.GetVertices());
        if (pointsB.empty()) // B is a Circle
        {
            maxB = ProjectVector(
                (a.GetPosition() - centerProjB).Normalize() * dynamic_cast<const Circle*>(&b)->GetRadius(),
                normal
            ) - centerProjB).LengthSquared();
        }
        else // Other shapes
        {
            for (const auto& itr : pointsB)
            {
                const float curLength = ProjectVector(itr, normal).LengthSquared();
                if (curLength > maxB) maxB = curLength;
            }
        }

        // False if separated
        return (centerProjA - centerProjB).LengthSquared() < (maxA + maxB);
    }
    */

    float ProjectVectors(const CVT& normal, const Collidable2D& a, const Collidable2D& b)
    {
        const std::vector<CVT> points(a.GetVertices());
        if (points.empty()) // Circle
        {
            const CVT centerProj(ProjectVector(a.GetPosition(), normal));
            return (ProjectVector(
                (b.GetPosition() - centerProj).Normalize() * dynamic_cast<const Circle*>(&a)->GetRadius(),
                normal
            ) - centerProj).LengthSquared();
        }
        else // Other shapes
        {
            float max = 0.f;
            for (const auto& itr : points)
            {
                const float curLength = ProjectVector(itr, normal).LengthSquared();
                LogDebug(curLength, "curLength");
                if (curLength > max) max = curLength;
            }
            return max;
        }
    }

    bool SATImpl(const Collidable2D& a, const Collidable2D& b)
    {
        LogDebug(a.GetPosition(), "A center");
        LogDebug(b.GetPosition(), "B center");
        for (const auto& n : GetNormals(a))
        {
            LogDebug(n, "Normal");
            auto maxA = ProjectVectors(n, a, b);
            auto maxB = ProjectVectors(n, b, a);
            auto centers = ProjectVector(a.GetPosition() - b.GetPosition(), n).LengthSquared();
            //auto centers = (ProjectVector(a.GetPosition(), n) - ProjectVector(b.GetPosition(), n)).LengthSquared();
            LogDebug(maxA, "maxA");
            LogDebug(maxB, "maxB");
            LogDebug(centers, "centers");
            // True if separated
            if ((maxA + maxB) < centers) continue;
            else return true;
            //if (maxProjB < minProjA || maxProjA < minProjB) continue; // Separated
            //else return true; // Not separated

            // TODO: old
            //axis.Normalize();
            //const Type overlap = b1.ProjectToAxis(axis) + b2.ProjectToAxis(axis) - math::Abs(Dot(toCenter, axis));
            //if (overlap < static_cast<Type>(0))
            //return false;

        }
        return false;
    }

    bool SAT(const Collidable2D& a, const Collidable2D& b)
    {
        return SATImpl(a, b) || SATImpl(b, a);
    }

    /*
    Vektor<3u, Type> GetAxis(const AXIS axis) const
    {
        MV_ASSERT(axis != AXIS::Invalid, "Invalid axis in PrimitiveBox GetAxis");
        const UInt32 ax = static_cast<UInt32>(axis);
        return Vektor<3u, Type>(
            this->transform[(ax * 4u)],
            this->transform[(ax * 4u)+1u],
            this->transform[(ax * 4u)+2u]
        );
    }
    Type GetHalfSize (AXIS axis)
    {
        return math::Abs((max[static_cast<UInt8>(axis)]-min[static_cast<UInt8>(axis)]) * static_cast<Type>(0.5f));
    }
    Type ProjectToAxis(const Vektor<3u, Type>& axis) const
    {
        return
            this->GetHalfSize(AXIS::X) * math::Abs(Dot(axis, GetAxis(AXIS::X))) +
            this->GetHalfSize(AXIS::Y) * math::Abs(Dot(axis, GetAxis(AXIS::Y))) +
            this->GetHalfSize(AXIS::Z) * math::Abs(Dot(axis, GetAxis(AXIS::Z)));
    }
    */


    Collidable2D::Collidable2D(const Vector2& position) :
        m_position(position)
        // m_rotation()
    {

    }

    Collidable2D::~Collidable2D()
    {

    }


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