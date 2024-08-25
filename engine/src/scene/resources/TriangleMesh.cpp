#include "TriangleMesh.h"

namespace pio
{
    TriangleMesh::TriangleMesh(const TriangleMesh& rhs)
    {
        Vertices = rhs.Vertices;
        Indices = rhs.Indices;
        Triangles = rhs.Triangles;
    }

    TriangleMesh::TriangleMesh(TriangleMesh&& rhs) noexcept
    {
        Vertices = std::move(rhs.Vertices);
        Indices = std::move(rhs.Indices);
        Triangles = std::move(rhs.Triangles);
    }

    TriangleMesh& TriangleMesh::operator=(const TriangleMesh& rhs)
    {
        if (this != &rhs)
        {
            this->TriangleMesh::TriangleMesh(rhs);
        }
        return *this;
    }

    TriangleMesh& TriangleMesh::operator=(TriangleMesh&& rhs) noexcept
    {
        if (this != &rhs)
        {
            this->TriangleMesh::TriangleMesh(std::forward<TriangleMesh>(rhs));
        }
        return *this;
    }
}