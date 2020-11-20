# Elplant

Custom implementation of Lindermayer's l-systems.

- Input: grammars
- Output: .obj .mtl (blender)
- Output: .yaml (raytracer)

## Build

```shell script
mkdir build
mkdir output
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
./elplant
```
