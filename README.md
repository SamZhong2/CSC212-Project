# B-Tree
### CSC212: Data Structures & Algorithms Term Project <br> Lawrence Cabbabe, Aaron Levesque, Ben Uthoff, Sam Zhong

## Code Compilation
To compile B-Tree in the command line, use g++. Although other versions may work, C++11 is used to ensure consistency.
```
g++ -std=c++11 main.cpp btree.cpp -o btree
```
## Program Execution
The B-Tree program can be executed with command-line arguments, or interfaced with using C++ `cin` when no arguments are provided.
## Visualization via DOT File
The program writes a file called `btree.dot`, which can be drawn using the VSCode Extension [Graphviz Interactive Prev](https://marketplace.visualstudio.com/items?itemName=tintinweb.graphviz-interactive-preview) or by copying and pasting the dot file to a site like [Graphviz Online](https://dreampuf.github.io/GraphvizOnline/) ([Github](https://github.com/dreampuf/GraphvizOnline)). If a node in the tree is a leaf, it will have a green-colored box. Below is an example of the visualization.

<p align="center">
<img src="https://github.com/SamZhong2/CSC212-Project/assets/81537940/6e3a4716-6be3-4a5e-a1cf-665a78ed6593"/>
</p>
