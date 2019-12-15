#include <iostream>
#include <vector>

struct Vec3
{
    float x, y, z;

    Vec3 operator+(const Vec3& other) const
    {
        return { this->x + other.x, this->y + other.y, this->z + other.z };
    }

    Vec3 operator-(const Vec3& other) const
    {
        return { this->x - other.x, this->y - other.y, this->z - other.z };
    }

    Vec3 cross(const Vec3& other) const
    {
        return {
            this->y * other.z - this->z * other.y,
            this->z * other.x - this->x * other.z,
            this->x * other.y - this->y * other.x,
        };
    }

    float dot(const Vec3& other) const
    {
        return this->x * other.x + this->y * other.y + this->z * other.z ;
    }
};

struct Polygon
{
    std::vector<Vec3> vertices;
};

// TODO: switch to triangles
struct Plane
{
    Vec3 pos;
    Vec3 normal;
};

struct Line
{
    Vec3 pos;
    Vec3 dir;
};

Polygon get_poly_from_cin()
{
    Polygon poly;
    int edge_count;

    std::cin >> edge_count;

    for (int i = 0; i < edge_count; ++i)
    {
        float x, y, z;
        std::cin >> x >> y >> z;
        poly.vertices.push_back(Vec3 { x, y, z });
    }

    return poly;
}

void print_poly(const Polygon& poly, int index = 1)
{
    std::cout << "Polygon " << index << ": \n";
    for (auto& vertex : poly.vertices)
    {
        std::cout << "Vec3: " << vertex.x << ' ' << vertex.y << ' ' << vertex.z << '\n';
    }
    std::cout << '\n';
}

std::vector<Plane> get_planes_for_poly(const Polygon& poly)
{
    std::vector<Plane> planes;

    auto first_vertex = poly.vertices.begin();
    for (auto vertex = std::next(first_vertex); vertex != poly.vertices.end() - 1; vertex = std::next(vertex))
    {
        planes.push_back({ 
            *first_vertex,
            (*vertex - *first_vertex).cross(*(vertex+1) - *first_vertex),
        });
    }

    return planes;
}

std::vector<Line> get_lines_for_poly(const Polygon& poly)
{
    std::vector<Line> lines;

    auto first_vertex = poly.vertices.begin();
    auto last_vertex = poly.vertices.begin();

    for (auto vertex = first_vertex; vertex != last_vertex; vertex = std::next(vertex))
    {
        lines.push_back({ 
            *first_vertex,
            *(vertex + 1) - *vertex,
        });
    }

    lines.push_back({ *first_vertex, *first_vertex - *last_vertex });
    return lines;
}

bool line_intersects_plane(const Line& line, const Plane& plane)
{
    return 
}

int main(int argc, char *argv[])
{
    Polygon first_poly = get_poly_from_cin();
    Polygon second_poly = get_poly_from_cin();

    print_poly(first_poly, 1);
    print_poly(second_poly, 2);

    auto first_poly_planes = get_planes_for_poly(first_poly);
    auto second_poly_lines = get_lines_for_poly(second_poly);

    return 0;
}
