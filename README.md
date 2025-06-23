# PCSProject2025

The hereby project was made by Anna Dalmasso, Manuel Macis e Marco Odasso. The objective of the code is to generate a geodetic polyehdron or a generalised Goldberg polyhedron, after tessellating the respective starting platonic polyhedron and then projecting it onto the unit sphere. Moreover, the program finds the minimum path between two vertices of the generated polyhedron.

The name of the executable is './PlatonicSolids': it generates the desired polyhedron based on the parameters given as input.

Input parameters
-

The program accepts a input tuple made of positive integers, following the sintax:

    ./PlatonicSolids p q b c

Where:

p and q identify the type of starting platonic polyhedron (tetrahedron, cube, octahedron, dodecahedron or icosahedron), using Schläfli's symbols {p, q};

b and c define the parameters for the tessellation, following Magnus Wenniger notation {p, q+}_(b,c), determining the number of subdivisions of the faces of the starting polyhedron and the complexity of the mesh on them.

Minimum path finding
-

In addition to the parameters explained above, the program also accepts the two following identifiers:

    id_vertex1 id_vertex2

These parameters, if given as input, make the program compute the minimum path between the two vertices that have those ids on the above-mentioned generated polyhedron. The minimum path is calculated using a proper pathfinding algorithm applied to the graph that has the vertices of the polyhedron as nodes and the edges of the polyhedron as arcs.

Program output
-

The code produces as outputs 3 files with .inp extension, inside the directory 'PolygonalData'. They contain the description of the main data sttructures of the above-mentioned generated polyhedron with a format compatible with converting and visualising softwares.

Polyhedron visualisation
-

#### Kitware Glance

In order to visualise the above-mentioned generated polyhedron through Kitware Glance, you should follow these steps:

1. Upload the .inp files onto the site https://meshconverter.it/it, and convert them form .ucd files to .vtu files, downloading the latter ones.

2. Once you've obtained the .vtu files, it is possible to upload them onto the online visualiser software https://kitware.github.io/glance/app/ in order to interactively explore the 3D geometry of the above-mentioned generated polyhedron.

#### ParaView

In order to visualise the above-mentioned generated polyhedron through ParaView downloaded locally, you should follow these steps:

1. Open ParaView;

2. Inside ParaView, open the .inp files e accept the default reader suggested by ParaView itself;

3. Click on "Apply" inside the "Properties" menu, which should be in the bottom left corner of the screen. You should now be able to interactively explore the 3D geometry of the above-mentioned generated polyhedron.

4. Moreover, in order to visualise the "ShortPath" property, associated to the minimum path found by the program, it is necessary to follow these steps:

    - Click on "Cell0Ds.inp" in the "Pipeline Browser" menu, which should be in the left side of the screen; then click above in the drop-down menu where is written "Material Id" and click on  the "ShortPath" beside a cube;

    - Click on "Cell1Ds.inp" in the "Pipeline Browser" menu, which should be in the left side of the screen; then click above in the drop-down menu where is written "Material Id" and click on  the "ShortPath" beside a cube;

    - Inside the "Properties" menu, in the "Coloring" section, click on the button where is written "Edit"; a "Color Map Editor" menu should appear on the right side of the screen. Inside it, click on the box beside "Interpret Values As Categories";

    - Modify the size of the points and of the edges respectively belonging to "Cell0Ds.inp" and "Cell1Ds.inp" through the "Properties" menu.


More details
-

In order to gain more information on the contents of the project and on how they work, please consult the 'Documentation' directory, which contains the UML documentation (generated through PlantUML language and online editor) for the more difficult functions and methods.

