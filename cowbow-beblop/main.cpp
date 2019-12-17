#include <iostream>
#include <vector>
#include <array>
#include <cmath>

int count = 0;
bool debug_mode = false;

#define dbg \
    if (!debug_mode) {} \
    else std::cout

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

    Vec3 operator*(float scalar) const
    {
        return { this->x * scalar, this->y * scalar, this->z * scalar };
    }

    Vec3 operator/(float scalar) const
    {
        return { this->x / scalar, this->y / scalar, this->z / scalar };
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

    float mag2() const
    {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    float mag() const
    {
        return sqrt(this->mag2());
    }

    Vec3 norm() const
    {
        return *this / this->mag();
    }
};

std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    return out;
}

struct Polygon
{
    std::vector<Vec3> vertices;
};

std::ostream& operator<<(std::ostream& out, const Polygon& poly)
{
    out << "Polygon ( ";
    for (auto& v : poly.vertices)
        out << v << ", ";
    out << ')';
    return out;
}

struct Triangle
{
    std::array<Vec3, 3> vertices;
};

std::ostream& operator<<(std::ostream& out, const Triangle& tri)
{
    out << "Triangle ( ";
    for (auto& v : tri.vertices)
        out << v << ", ";
    out << ')';
    return out;
}

struct Line
{
    Vec3 start;
    Vec3 end;
    Vec3 dir;
};

std::ostream& operator<<(std::ostream& out, const Line& l)
{
    out << "Line { start" << l.start << " end" << l.end << " dir" << l.dir << " }";
    return out;
}

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
    dbg << "Polygon " << index << ": \n";
    for (auto& vertex : poly.vertices)
    {
        dbg << "Vec3: " << vertex.x << ' ' << vertex.y << ' ' << vertex.z << '\n';
    }
    dbg << '\n';
}

std::vector<Triangle> get_tris_for_poly(const Polygon& poly)
{
    std::vector<Triangle> tris;

    auto first_vertex = poly.vertices.begin();
    for (auto vertex = first_vertex + 1; vertex != poly.vertices.end() - 1; ++vertex)
    {
        tris.push_back({
            *first_vertex,
            *vertex,
            *(vertex + 1),
        });
    }

    return tris;
}

std::vector<Line> get_lines_for_poly(const Polygon& poly)
{
    std::vector<Line> lines;

    auto first_vertex = poly.vertices.begin();
    auto last_vertex = poly.vertices.end() - 1;

    for (auto vertex = first_vertex; vertex != last_vertex; ++vertex)
    {
        lines.push_back({ 
            *vertex,
            *(vertex + 1),
            *(vertex + 1) - *vertex,
        });
    }

    lines.push_back({ *first_vertex, *last_vertex, *last_vertex - *first_vertex });
    return lines;
}

bool line_intersects_tri(const Line& line, const Triangle& tri)
{
    // Checking whether line intersects plane
    Vec3 isct_point;
    Vec3 normal = (tri.vertices[1] - tri.vertices[0]).cross((tri.vertices[2] - tri.vertices[0]));
    {
        // calculate plane
        float d = normal.dot(tri.vertices[0]);

        if (normal.dot(line.dir) == 0) {
            return false; // Line parallel to plane
        }

        // Compute the t value for the directed line ray intersecting the plane
        float t = (d - normal.dot(line.start)) / normal.dot(line.dir);

        dbg << t << '\n';

        if (t < 0 || t > 1) {
            return false; // line does not
        }

        isct_point = line.start + line.dir * t;
    }

    // Checking whether line intersects triangle
    {
        dbg << "here" << "\n";
        dbg << isct_point << '\n';
        float tri_area_2 = normal.mag();
        float a = ((tri.vertices[1] - isct_point).cross((tri.vertices[2] - isct_point))).mag() / tri_area_2;
        float b = ((tri.vertices[2] - isct_point).cross((tri.vertices[0] - isct_point))).mag() / tri_area_2;
        float c = 1 - a - b;

        dbg << a << b << c << '\n';

        auto is_within_range = [&](float n) { return n >= 0 && n <= 1; };
        return is_within_range(a) && is_within_range(b) && is_within_range(c);
    }
}

int main(int argc, char *argv[])
{
    Polygon first_poly = get_poly_from_cin();
    Polygon second_poly = get_poly_from_cin();

    auto first_poly_tris = get_tris_for_poly(first_poly);
    auto second_poly_lines = get_lines_for_poly(second_poly);

    dbg << first_poly << '\n';
    dbg << second_poly << '\n';

    int isct_count = 0;

    for (auto& line : second_poly_lines)
    {
        dbg << line << '\n';
        for (auto& tri : first_poly_tris)
        {
            dbg << tri << '\n';
            if (line_intersects_tri(line, tri))
            {
                dbg << "Is within" << '\n';
                ++isct_count;
            }
        }
    }

    if (isct_count % 2 == 0)
        std::cout << "NO\n";
    else
        std::cout << "YES\n";

    return 0;
}
