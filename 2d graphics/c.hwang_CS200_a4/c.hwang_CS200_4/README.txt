Manual
A.	Generate and Open Executable File
	01.	Locate solution(.sln) file in the directory and execute
	02.	Build the solution
	03.	Copy duck.bmp, ducklow.bmp, lily.bmp, and lilylow.bmp into Debug/Release folder
	04.	Locate Debug/Release folder in the directory and open
	05.	Locate the A4.exe file in the directory and execute

B.	Draw quadrilateral in Program
	01.	Click left mouse button at any viewport location to define first vertex of a quadrilateral
	02.	Click left mouse button at any viewport location to define second vertex of a quadrilateral, but
	select counter-clockwise
	03.	Click left mouse button at any viewport location to define third vertex of a quadrilateral, but
	select counter-clockwise
	04.	Click left mouse button at any viewport location to define fourth vertex of a quadrilateral, but
	select counter-clockwise
C.	Control
	01.	Press [C] or [c] to clear previously drawn quadrilaterals
	02.	To edit vertex texture coordinates or a previously modeled quadrilateral, do the following:
		a.	First, select ¡°Edit Texture Coordinates¡± submenu in ¡°Mode¡± menu
		b.	Second, select a vertex of the quad to be edited
		c.	Select a new set of texture coordinates for the selected vertex by clicking left mouse in color map drawn in top-left corner of viewport
	03.	To edit vertex color coordinates of a previously modeled quadrilateral do the following:
		a.	First, select ¡°Edit Color Coordinates¡± submenu in ¡°Mode¡± menu
		b.	Second, select a vertex of the quad to be edited
		c.	Select a new set of color coordinates for the selected vertex using the color box
	04.	To test your triangle rasterizer, select ¡°Test¡± submenu in ¡°Mode¡± menu. The test mode creates a 500 X 375 rectangle that is rendered by splitting it into two triangles, and rendering each triangle using top-left tie breaking fill rule
	05.	Use ¡°Texture¡± menu to toggle between the three previously defined texture objects
?
	06.	Use ¡°Color Combiner¡± menu to toggle between these four different color combine modes:
		a.	¡°Replace¡± Pixel color is texel color
		b.	¡°Decal¡± Pixel color is a linearly interpolated value between vertex color and texel color using texel alpha as the interpolant
		c.	¡°Modulate¡± Pixel color is pairwise multiplication of texel and vertex color components
		d.	¡°Gourard Shading¡± Pixel color is interpolated vertex color
	07.	Use ¡°TexFuncU¡± and ¡°TexFuncV¡± menu option to implement functions that transform texture coordinates. These functions are independently implemented along horizontal and vertical directions. The following texture functions are implemented
		a.	¡°Repeat¡±
		b.	¡°Mirror¡±
		c.	¡°Clamp (to edge)¡±
D.	Execute within the Visual Studio
	01.	Build the solution
	02.	Locate Debug menu on the title bar
	03.	Select Start Debugging OR Start Without Debugging
