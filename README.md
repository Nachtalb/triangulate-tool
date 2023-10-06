# Triangulate Tool CLI

> Easily convert any image into low-poly art

![before afger](images/gura.jpg)

Originaly created by:
[jaybosamiya](https://github.com/jaybosamiya/triangulate-tool)

## How it works

The tool creates a defined amount of random points and runs a
[Delaunay Triangulation](https://en.wikipedia.org/wiki/Delaunay_triangulation)
on it. It then averages out the colours in the source image for each triangle in
order to generate the low-poly image.

## Usage

```bash
./triangulate-tool input_image output_image [total_points] [total_points_per_side]
```

- `total_points` (default: `100`): `int` of total random points in the image
- `total_points_per_side` (default: `3`): `int` total points on each edge of the
  image (does not count towards `total_points`)

## Build process

```sh
./build.sh
```

or manually

```sh
gcc triangulate-tool.cpp -o triangulate-tool $(pkg-config --cflags-only-I opencv4) -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
```

## Requirements

- OpenCV 4.x (`sudo pacman -Sy opencv`)
- A C++ compiler like `gcc` (`sudo pacman -Sy gcc`)

## License

This project is licensed under the
[GNU Lesser General Public License v3.0](https://www.gnu.org/licenses/lgpl-3.0.en.html).
