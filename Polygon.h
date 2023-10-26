#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

struct Point {
    int x, y;

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const
    {
        return !(*this == other);
    }
};

struct Polygon {
    std::vector<Point> points;
};

class AreaPredicate {
public:
    explicit AreaPredicate(const std::string& type) : type_(type) {}

    bool operator()(const Polygon& polygon) const {
        if (type_ == "EVEN") {
            return polygon.points.size() % 2 == 0;
        }
        else if (type_ == "ODD") {
            return polygon.points.size() % 2 != 0;
        }
        return false;
    }

private:
    std::string type_;
};

class CalculateArea {
public:
    double operator()(const Polygon& polygon) const {
        if (polygon.points.size() < 3) {
            return 0.0;
        }

        double sum = 0.0;
        for (size_t i = 0; i < polygon.points.size() - 1; ++i) {
            sum += (polygon.points[i].x * polygon.points[i + 1].y) -
                (polygon.points[i + 1].x * polygon.points[i].y);
        }
        sum += (polygon.points[polygon.points.size() - 1].x * polygon.points[0].y) -
            (polygon.points[0].x * polygon.points[polygon.points.size() - 1].y);

        return 0.5 * std::abs(sum);
    }
};

std::vector<Polygon> readPolygonsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file.");
    }

    std::vector<Polygon> polygons;
    std::string line;
    std::getline(file, line);
    int numPolygons = std::stoi(line);

    polygons.reserve(numPolygons);
    for (int i = 0; i < numPolygons; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);
        int numPoints;
        if (iss >> numPoints) {
            Polygon polygon;
            for (int j = 0; j < numPoints; ++j) {
                char openParenthesis, semicolon, closeParenthesis;
                Point point;
                if (iss >> openParenthesis >> point.x >> semicolon >> point.y >> closeParenthesis) {
                    polygon.points.push_back(point);
                }
                else {
                    throw std::runtime_error("Invalid polygon description in file.");
                }
            }
            polygons.push_back(polygon);
        }
        else {
            throw std::runtime_error("Invalid polygon count in file.");
        }
    }
    return polygons;
}

void processCommands(const std::vector<Polygon>& polygons) {
    std::string command;
    while (std::cin >> command) {
        if (command == "AREA") {
            std::string type;
            if (std::cin >> type) {
                if (type == "MEAN") {
                    if (!polygons.empty())
                    {
                        double totalArea = 0.0;
                        CalculateArea calculateArea;

                        for (const auto& polygon : polygons)
                        {
                            totalArea += calculateArea(polygon);
                        }

                        double meanArea = totalArea / polygons.size();
                        std::cout << std::fixed << std::setprecision(1) << meanArea << std::endl;
                    }
                    else
                    {
                        std::cout << "At least one figure is required to calculate the average" << std::endl;
                    }
                }
                else if (type == "EVEN" || type == "ODD") {
                    AreaPredicate predicate(type);
                    CalculateArea calculateArea;
                    double totalArea = 0.0;
                    for (const auto& polygon : polygons) {
                        if (predicate(polygon)) {
                            totalArea += calculateArea(polygon);
                        }
                    }
                    std::cout << std::fixed << std::setprecision(1) << totalArea << std::endl;
                }
                else {
                    int numVertices = std::stoi(type);
                    CalculateArea calculateArea;
                    double totalArea = 0.0;
                    for (const auto& polygon : polygons) {
                        if (polygon.points.size() == numVertices) {
                            totalArea += calculateArea(polygon);
                        }
                    }
                    std::cout << std::fixed << std::setprecision(1) << totalArea << std::endl;
                }
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
        else if (command == "MAX" || command == "MIN") {
            std::string property;
            if (std::cin >> property) {
                if (property == "AREA" || property == "VERTEXES") {
                    std::function<bool(double, double)> compareFunc;
                    if (command == "MAX") {
                        compareFunc = std::greater<double>();
                    }
                    else {
                        compareFunc = std::less<double>();
                    }

                    double maxValue;
                    bool isFirst = true;
                    for (const auto& polygon : polygons) {
                        double value;
                        if (property == "AREA") {
                            CalculateArea calculateArea;
                            value = calculateArea(polygon);
                        }
                        else {
                            value = polygon.points.size();
                        }

                        if (isFirst || compareFunc(value, maxValue)) {
                            maxValue = value;
                            isFirst = false;
                        }
                    }

                    if (isFirst) {
                        std::cout << "<INVALID COMMAND>" << std::endl;
                    }
                    else {
                        if (property == "AREA") {
                            std::cout << std::fixed << std::setprecision(1) << maxValue << std::endl;
                        }
                        else {
                            std::cout << maxValue << std::endl;
                        }
                    }
                }
                else {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
        else if (command == "COUNT") {
            std::string countType;
            if (std::cin >> countType) {
                if (countType == "EVEN" || countType == "ODD") {
                    AreaPredicate predicate(countType);
                    int count = std::count_if(polygons.begin(), polygons.end(), predicate);
                    std::cout << count << std::endl;
                }
                else {
                    int numVertices = std::stoi(countType);
                    int count = std::count_if(polygons.begin(), polygons.end(),
                        [&](const Polygon& polygon) {
                            return polygon.points.size() == numVertices;
                        });
                    std::cout << count << std::endl;
                }
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
        else if (command == "LESSAREA") {
            Polygon polygon;
            char openParenthesis, semicolon, closeParenthesis;
            while (std::cin >> openParenthesis >> polygon.points.back().x >> semicolon >> polygon.points.back().y >> closeParenthesis) {
                if (openParenthesis == '(' && semicolon == ';' && closeParenthesis == ')') {
                    if (polygon.points.back().x == 0 && polygon.points.back().y == 0) {
                        break;
                    }
                    polygon.points.emplace_back();
                }
                else {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    break;
                }
            }

            if (polygon.points.size() > 1) {
                CalculateArea calculateArea;
                int count = std::count_if(polygons.begin(), polygons.end(),
                    [&](const Polygon& p) {
                        return calculateArea(p) < calculateArea(polygon);
                    });
                std::cout << count << std::endl;
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
        else if (command == "SAME") {
            Polygon polygon;
            char openParenthesis, semicolon, closeParenthesis;
            while (std::cin >> openParenthesis >> polygon.points.back().x >> semicolon >> polygon.points.back().y >> closeParenthesis) {
                if (openParenthesis == '(' && semicolon == ';' && closeParenthesis == ')') {
                    if (polygon.points.back().x == 0 && polygon.points.back().y == 0) {
                        break;
                    }
                    polygon.points.emplace_back();
                }
                else {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    break;
                }
            }

            if (polygon.points.size() > 1) {
                int count = std::count_if(polygons.begin(), polygons.end(),
                    [&](const Polygon& p) {
                        return std::equal(p.points.begin(), p.points.end(),
                        polygon.points.begin(), polygon.points.end());
                    });
                std::cout << count << std::endl;
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
        else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }
}
