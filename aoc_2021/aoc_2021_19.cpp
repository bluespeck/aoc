#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "outf.h"

struct Vector
{
    int32_t x;
    int32_t y;
    int32_t z;
    friend bool operator==(Vector a, Vector b)
    { 
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
    friend Vector operator+(Vector v1, Vector v2)
    {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    }
    friend Vector operator-(Vector v1, Vector v2)
    {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    }
    friend bool operator <(Vector v1, Vector v2) 
    {
        return v1.x < v2.x || v1.x == v2.x && v1.y < v2.y || v1.x == v2.x && v1.y == v2.y && v1.z < v2.z;
    }
};

using Point = Vector;

using Cube = std::vector<Point>;
using Cube_Variations = std::vector<std::pair<char, Cube>>;
using Input = std::vector<Cube_Variations>;

void rotate_90(Point& point, char axis)
{
    const int32_t sin90 = 1;
    const int32_t cos90 = 0;
    if (axis == 'x')
    {
        point = { point.x, cos90 * point.y - sin90 * point.z, sin90 * point.y + cos90 * point.z };
    }
    else if (axis == 'y')
    {
        point = { cos90 * point.x + sin90 * point.z, point.y, -sin90 * point.x + cos90 * point.z };
    }
    else if (axis == 'z')
    {
        point = { cos90 * point.x - sin90 * point.y, sin90 * point.x + cos90 * point.y, point.z };
    }
    else
    {
        assert(false);
    }
}

void rotate_cube_90(Cube& cube, char axis)
{
    for (auto& e : cube)
        rotate_90(e, axis);
}

void rotate_180(Point& point, char axis)
{
    rotate_90(point, axis);
    rotate_90(point, axis);
 
}

void rotate_cube_180(Cube& cube, char axis)
{
    for (auto& e : cube)
    {
        rotate_180(e, axis);
    }
}

Cube_Variations build_scanner_variations(const Cube& input_cube)
{
    Cube_Variations variations;
    Cube cube = input_cube;
    variations.emplace_back('0', cube);

    std::string rotation_sequence = "zzzyxxxyzzzyxxxzyyyXyyy";

    for(auto ch : rotation_sequence)
    {
        if (ch != 'X')
        {
            rotate_cube_90(cube, ch);
        }
        else
        {
            rotate_cube_180(cube, 'x');
        }
        variations.emplace_back(ch, cube);
    }
    return variations;
}

Input read_input()
{
    Input input;
    Cube cube;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size())
        {
            if (line[0] == '-' && line[1] == '-')
            {
                if (cube.size() > 0)
                {
                    input.push_back(build_scanner_variations(cube));
                    cube.clear();
                }
            }
            else
            {
                auto coord_strs = split_string(line, ",");
                assert(coord_strs.size() == 3);
                cube.push_back({ std::stol(coord_strs[0]), std::stol(coord_strs[1]), std::stol(coord_strs[2]) });
            }
        }
    }
    if (cube.size() > 0)
    {
        input.push_back(build_scanner_variations(cube));
        cube.clear();
    }
    return input;
}

void print_point(Vector point)
{
    outf("{},{},{}", point.x, point.y, point.z);
}

using Relative_Positions = std::vector<std::vector<Vector>>;

std::pair<Vector, std::vector<Point>> find_offset_and_overlapping_points(const Cube& c1, const Cube& c2)
{
    std::pair<Point, std::vector<Point>> result;
    for (auto& p1 : c1)
    {
        for (auto& p2 : c2)
        {
            auto offset = p2 - p1;
            
            int32_t count = 0;
            std::vector<Point> overlapping;
            for (auto& e : c1)
            {
                auto p = e + offset;

                if (std::find(c2.begin(), c2.end(), p) != c2.end())
                {
                    overlapping.push_back(e);
                    count++;
                }
            }
            if (count >= 12)
            {
                result.first = offset;
                result.second = overlapping;
                break;
            }
        }
    }
    return result;
}

struct Overlap_Info
{
    Vector translation;
    std::vector<Point> overlapping_points;
    std::string operations;
};

Overlap_Info overlaps(Cube_Variations& ref_cube, const Cube_Variations& other_cube)
{
    Overlap_Info result;
    for (auto& cube : other_cube)
    {
        result.operations += cube.first;
        auto overlap_info = find_offset_and_overlapping_points(ref_cube[0].second, cube.second);
        if (overlap_info.second.size() > 0)
        {
            //outf("Found : \n");
            //for (auto e : overlap_info.second)
            //{
            //    print_point(e); outf("\n");
            //}
            //outf("\n");
            result.translation = overlap_info.first;
            result.overlapping_points = overlap_info.second;
            return result;
        }
    }
    return result;
}

struct Scanner_Index_Pair
{
    size_t reference_index;
    size_t to_be_verified_index;
};

using Overlap_Infos = std::vector<std::pair<Scanner_Index_Pair, Overlap_Info>>;

Overlap_Infos build_overlap_info(const Input& input)
{
    Overlap_Infos overlap_infos;

    for (size_t i = 0; i < input.size(); i++)
    {
        auto reference = input[i];
        for (size_t j = 0; j < input.size(); j++)
        {
            if (j != i)
            {
                auto cube = input[j];
                auto result = overlaps(reference, cube);
                if (result.overlapping_points.size() >= 12)
                {
                    //outf("Scanner {} overlaps with scanner {}\n", i, j);
                    //for (auto e : result.overlapping_points)
                    //{
                    //    print_point(e); outf("\n");
                    //}
                    overlap_infos.push_back({ { i, j }, result });
                }
            }
        }
    }
    return overlap_infos;
}

void solve_1_2(const Input& input)
{
    outf("\nPart {}: ", 1);
    
    auto overlap_infos = build_overlap_info(input);
    //{
    //    outf("All connections:\n");
    //    for (auto& e : overlap_infos)
    //    {
    //        outf("{}, {}\n", e.first.reference_index, e.first.to_be_verified_index);
    //    }
    //}

    std::vector<Point> absolute_points;
    std::unordered_set<size_t> verified_scanners;
    struct Transformation_Info
    {
        size_t previous_index;
        std::string rotation_transforms;
        Vector translation;
    };
    std::unordered_map<size_t, Transformation_Info> scanner_index_to_transformations_from_prev_scanner;
    
    absolute_points.insert(absolute_points.begin(), input[0].front().second.begin(), input[0].front().second.end());
    verified_scanners.insert(0);
    scanner_index_to_transformations_from_prev_scanner[0] = { 0, "", Vector{0,0,0} };
    
    std::vector<Point> scanner_positions;
    scanner_positions.push_back(Point{0,0,0});

    while (verified_scanners.size() != input.size())
    {
        for (auto& e : overlap_infos)
        {
            if (verified_scanners.contains(e.first.reference_index) && !verified_scanners.contains(e.first.to_be_verified_index))
            {
                auto& to_be_verified_transform_info = scanner_index_to_transformations_from_prev_scanner[e.first.to_be_verified_index];
                to_be_verified_transform_info.previous_index = e.first.reference_index;
                to_be_verified_transform_info.rotation_transforms = e.second.operations;
                to_be_verified_transform_info.translation = e.second.translation;

                // reverse translate rotated current scaner points to the previous scanner's position,
                // then rotate until previous scanner frame is reached
                // repeat until we reach the frame of scanner 0
                size_t index = e.first.to_be_verified_index;
                auto to_be_verified_points = input[e.first.to_be_verified_index][e.second.operations.size() - 1].second;
                Point scanner_pos = {};
                while (index != 0)
                {   
                    auto& transform_info = scanner_index_to_transformations_from_prev_scanner[index];
                    std::string operations = scanner_index_to_transformations_from_prev_scanner[transform_info.previous_index].rotation_transforms;
                    for (auto& point : to_be_verified_points)
                    {
                        point = point - transform_info.translation;
                        for (char e: operations)
                        {
                            if (e == '0')
                                continue;
                            if (e != 'X')
                            {
                                rotate_90(point, e);
                            }
                            else
                            {
                                rotate_180(point, 'x');
                            }
                        }

                        // can verify here if the overlapping points from the reference frame land correctly in the absolute points after translating them
                    }
                    scanner_pos = scanner_pos - transform_info.translation;
                    for (char e : operations)
                    {
                        if (e == '0')
                            continue;
                        if (e != 'X')
                        {
                            rotate_90(scanner_pos, e);
                        }
                        else
                        {
                            rotate_180(scanner_pos, 'x');
                        }
                    }
                    
                    //outf("Points of scanner {} in the frame of scanner {}\n", e.first.to_be_verified_index, transform_info.previous_index);
                    //std::sort(to_be_verified_points.begin(), to_be_verified_points.end());
                    //for (auto& point : to_be_verified_points)
                    //{
                    //    print_point(point); outf("\n");
                    //}

                    index = transform_info.previous_index;
                }
                for (auto& point : to_be_verified_points)
                {
                    if (std::find(absolute_points.begin(), absolute_points.end(), point) == absolute_points.end())
                    {
                        absolute_points.push_back(point);
                    }
                }
                scanner_positions.push_back(scanner_pos);
                verified_scanners.insert(e.first.to_be_verified_index);
            }
        }
    }
    //outf("\nAll points:\n");
    //std::sort(absolute_points.begin(), absolute_points.end());
    //for (auto& point : absolute_points)
    //{
    //    print_point(point); outf("\n");
    //}
    outf("{}", absolute_points.size());
    
    outf("\nPart {}: ", 2);
    int32_t max_manhattan_dist = 0;
    for (auto& p1 : scanner_positions)
    {
        //print_point(p1); outf("\n");
        for (auto& p2 : scanner_positions)
        {
            if (p1 != p2)
            {
                int32_t distance = std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y) + std::abs(p1.z - p2.z);
                if (distance > max_manhattan_dist)
                {
                    max_manhattan_dist = distance;
                }
            }
        }

    }
    
    outf("{}\n", max_manhattan_dist);
}

int main()
{
    auto input = read_input();
    solve_1_2(input);

    return 0;
}
