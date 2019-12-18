#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>

int count = 0;
bool debug_mode = true;

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

    bool operator==(const Vec3& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
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
    bool has_intersected = false;
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
    bool has_intersected = false;
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

// Checks if line intersects triangle (other than the points in intersected_points), adding intersection point to the third argument if so
void intersect_line_tri(
        std::vector<Line>& lines,
        std::vector<Triangle>& tris,
        std::vector<Vec3>& intersected_points,
        int& isct_count
        )
{
    for (auto line = lines.begin(); line != lines.end(); ++line)
    {
        for (auto tri = tris.begin(); tri != tris.end(); ++tri)
        {
            // Checking whether line intersects plane
            Vec3 isct_point;
            Vec3 normal = (tri->vertices[1] - tri->vertices[0]).cross((tri->vertices[2] - tri->vertices[0]));
            {
                // calculate plane
                float d = normal.dot(tri->vertices[0]);

                if (normal.dot(line->dir) == 0)
                {
                    // Then line is parallel to plane
                   
                    if ((line->start - tri->vertices[0]).dot(normal) == 0 
                            && (line->end - tri->vertices[0]).dot(normal) == 0)
                    {
                        // Then line is on the plane

                        auto next_line = line + 1 == lines.end() ? lines.begin() : line + 1;
                        auto prev_line = line == lines.begin() ? lines.end() - 1 : line - 1;

                        if (next_line->dir.dot(prev_line->dir) > 0)
                        {
                            // Then adjacent lines are pointing in the same direction, thus need to add one to count
                            --isct_count;
                        } // otherwise do nothing.
                    }

                    continue; // Line parallel to plane
                }

                // Compute the t value for the directed line ray intersecting the plane
                float t = (d - normal.dot(line->start)) / normal.dot(line->dir);

                dbg << t << '\n';

                if (t < 0 || t > 1)
                    continue; // line does not

                isct_point = line->start + line->dir * t;

                if (std::find(intersected_points.begin(), intersected_points.end(), isct_point) 
                        != intersected_points.end())
                    continue; // we have already accounted this point;
            }

            // Checking whether line intersects triangle
            {
                dbg << "here" << "\n";
                dbg << isct_point << '\n';

                Vec3 v0 = tri->vertices[1] - tri->vertices[0];
                Vec3 v1 = tri->vertices[2] - tri->vertices[0];
                Vec3 v2 = isct_point - tri->vertices[0];

                float d00 = v0.dot(v0);
                float d01 = v0.dot(v1);
                float d11 = v1.dot(v1);
                float d20 = v2.dot(v0);
                float d21 = v2.dot(v1);

                float denom = d00 * d11 - d01 * d01;
                float a = (d11 * d20 - d01 * d21) / denom;
                float b = (d00 * d21 - d01 * d20) / denom;
                float c = 1 - a - b;

                /* float tri_area_2 = normal.mag(); */
                /* float a = ((tri.vertices[1] - isct_point).cross((tri.vertices[2] - isct_point))).mag() / tri_area_2; */
                /* float b = ((tri.vertices[2] - isct_point).cross((tri.vertices[0] - isct_point))).mag() / tri_area_2; */
                /* float c = 1 - a - b; */

                dbg << a << b << c << '\n';

                auto is_within_range = [&](float n) { return n >= 0 && n <= 1; };

                if (is_within_range(a) && is_within_range(b) && is_within_range(c)) {
                    intersected_points.push_back(isct_point);
                    ++isct_count;
                    dbg << "Is within" << '\n';
                }
            }

        }
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

    std::vector<Vec3> intersected_points;

    intersect_line_tri(second_poly_lines, first_poly_tris, intersected_points, isct_count);

    dbg << "\nIntersection count: " << isct_count << '\n';

    if (isct_count % 2 == 0)
        std::cout << "NO\n";
    else
        std::cout << "YES\n";

    return 0;
}
