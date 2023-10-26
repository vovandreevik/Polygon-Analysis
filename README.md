# Polygon-Analysis

This C++ program reads and processes polygon data from a file and provides various commands for analyzing the polygons. It supports commands for calculating areas, finding polygons with maximum or minimum properties, counting polygons, and more.

## Features

- **Polygon Data**: The program reads polygon data from a file. Each polygon is defined by its vertices.

- **Area Calculation**: Calculate the area of polygons and find their mean area.

- **Polygon Property Analysis**: Analyze polygons based on their area and the number of vertices. Find polygons with the maximum or minimum properties.

- **Counting Polygons**: Count polygons with specific properties.

## Commands

The program accepts various commands in the following format:

- `AREA [TYPE]`: Calculate the area of polygons based on the given type. Supported types are:
  - `MEAN`: Calculate the mean area of all polygons.
  - `EVEN`: Calculate the total area of polygons with an even number of vertices.
  - `ODD`: Calculate the total area of polygons with an odd number of vertices.
  - `[NUM]`: Calculate the total area of polygons with a specific number of vertices.

- `MAX AREA | VERTEXES` and `MIN AREA | VERTEXES`: Find the maximum or minimum area or number of vertices among polygons.

- `COUNT [TYPE]`: Count the number of polygons based on the given type. Supported types are the same as those for the `AREA` command.

- `LESSAREA (VERTEX;VERTEX)...`: Count polygons with an area less than a specified polygon.

- `SAME (VERTEX;VERTEX)...`: Count polygons that are the same as a specified polygon.

- `EXIT`: Exit the program.

## Input File Format
The input file should contain polygon data in the following format:

```
[NUMBER OF POLYGONS]
[NUMBER OF VERTICES] ([X;Y]) ([X;Y]) ...
[NUMBER OF VERTICES] ([X;Y]) ([X;Y]) ...
```

### Example:


```
4 (0;0) (0;1) (1;1) (1;0)
5 (0;0) (0;1) (1;2) (2;1) (2;0)
3 (0;0) (-2;0) (0;-2)
3 (1;1) (1;3) (3;3)
```

## Examples
Here are some example commands and their expected outputs:

- AREA MEAN: Calculate the mean area of all polygons.
- AREA EVEN: Calculate the total area of polygons with an even number of vertices.
- MAX AREA: Find the maximum area among polygons.
