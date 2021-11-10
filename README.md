
Edges are printed in clockwise direction.
In certain cases when the incident edge's face changes,the old face's incident edge becomes equal to that of the newly created split edge.
Multiple splits are handled.
Anti-clockwise splits are handled.

Assumption:
The lines in input.txt and split.txt should be equal to the number of inputs.(No blank lines should be present)
For some reason the last few lines are not visible when I open a normal text editor (gedit on ubuntu) so please open the output.txt file in vscode.
If it is still not visible I have provided my sample output below
In split.txt
for split,input should be in the following way:
Split 2 3

for id,input should be in the following way:
Id: 1 2 5

To compile and run the code:
make
./dcel input.txt split.txt output.txt
To clear the files
make clean

Sample output is:


Edges:
0
1
2
3
Halfedges: 
startVertexIndex=0 endVertexIndex=1 nextEdge=1 previousEdge=3 faceIndex=0 edgeIndex=0
startVertexIndex=1 endVertexIndex=2 nextEdge=2 previousEdge=0 faceIndex=0 edgeIndex=1
startVertexIndex=2 endVertexIndex=3 nextEdge=3 previousEdge=1 faceIndex=0 edgeIndex=2
startVertexIndex=3 endVertexIndex=0 nextEdge=0 previousEdge=2 faceIndex=0 edgeIndex=3
Faces:
FaceIndex:0 Edges 0->1->2->3

//After split

Edges:
0
1
2
3
4
5
6
Halfedges: 
startVertexIndex=0 endVertexIndex=1 nextEdge=1 previousEdge=3 faceIndex=0 edgeIndex=0
startVertexIndex=1 endVertexIndex=2 nextEdge=2 previousEdge=0 faceIndex=0 edgeIndex=1
startVertexIndex=2 endVertexIndex=4 nextEdge=4 previousEdge=1 faceIndex=0 edgeIndex=2
startVertexIndex=4 endVertexIndex=5 nextEdge=3 previousEdge=2 faceIndex=0 edgeIndex=4
startVertexIndex=5 endVertexIndex=0 nextEdge=0 previousEdge=4 faceIndex=0 edgeIndex=3
startVertexIndex=5 endVertexIndex=4 nextEdge=5 previousEdge=6 faceIndex=1 edgeIndex=4
startVertexIndex=4 endVertexIndex=3 nextEdge=6 previousEdge=4 faceIndex=1 edgeIndex=5
startVertexIndex=3 endVertexIndex=5 nextEdge=4 previousEdge=5 faceIndex=1 edgeIndex=6
Faces:
FaceIndex:0 Edges 0->1->2->4->3
FaceIndex:1 Edges 4->5->6

//After split

Edges:
0
1
2
3
4
5
6
7
8
9
Halfedges: 
startVertexIndex=0 endVertexIndex=1 nextEdge=1 previousEdge=3 faceIndex=0 edgeIndex=0
startVertexIndex=1 endVertexIndex=2 nextEdge=2 previousEdge=0 faceIndex=0 edgeIndex=1
startVertexIndex=2 endVertexIndex=4 nextEdge=8 previousEdge=1 faceIndex=0 edgeIndex=2
startVertexIndex=4 endVertexIndex=6 nextEdge=4 previousEdge=2 faceIndex=0 edgeIndex=8
startVertexIndex=6 endVertexIndex=5 nextEdge=3 previousEdge=8 faceIndex=0 edgeIndex=4
startVertexIndex=5 endVertexIndex=0 nextEdge=0 previousEdge=4 faceIndex=0 edgeIndex=3
startVertexIndex=6 endVertexIndex=7 nextEdge=6 previousEdge=4 faceIndex=1 edgeIndex=7
startVertexIndex=7 endVertexIndex=5 nextEdge=4 previousEdge=7 faceIndex=1 edgeIndex=6
startVertexIndex=5 endVertexIndex=6 nextEdge=7 previousEdge=6 faceIndex=1 edgeIndex=4
startVertexIndex=7 endVertexIndex=6 nextEdge=8 previousEdge=9 faceIndex=2 edgeIndex=7
startVertexIndex=6 endVertexIndex=4 nextEdge=5 previousEdge=7 faceIndex=2 edgeIndex=8
startVertexIndex=4 endVertexIndex=3 nextEdge=9 previousEdge=8 faceIndex=2 edgeIndex=5
startVertexIndex=3 endVertexIndex=7 nextEdge=7 previousEdge=5 faceIndex=2 edgeIndex=9
Faces:
FaceIndex:0 Edges 0->1->2->8->4->3
FaceIndex:1 Edges 7->6->4
FaceIndex:2 Edges 7->8->5->9
Id: 1 0
Id: 2 2
Id: 3 0
