
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

class Vertex //to store the information about the vertices
{
public:
    double x;
    double y;
    class HalfEdge *IncidentEdge;
    int index;
};
struct Vertex_input //to store the input present in the file
{
    double x, y;
};

class HalfEdge // to store the information about the edges which are divided into half edges
{
public:
    int index;
    class Vertex *Origin, *Destination;
    class HalfEdge *NextEdge, *Twin, *PreviousEdge;
    class Face *IncidentFace;
};

class Face // to store the information about the faces
{
public:
    int index;
    class HalfEdge *IncidentEdge;
};

class DCEL //to store information about the entire doubly connected edge list
{
public:
    vector<Vertex *> Vertices;
    HalfEdge *Edges;
    vector<Face *> Faces;
    int NumberOfEdges;
    int NumberOfVertices;
    int NumberOfFaces;
};
/*Function to make the beginning square with face f*/
void makeEdge(DCEL *d, Vertex *v1, Vertex *v2, Face *f, int index)
{
    HalfEdge *e1 = new HalfEdge;
    e1->Twin = NULL;
    e1->index = index;
    d->NumberOfEdges++;
    e1->Origin = v1;
    e1->Destination = v2;
    e1->IncidentFace = f;
    if (e1->index == 0) // maknig the first edge
    {
        e1->NextEdge = e1->PreviousEdge = e1;
        d->Edges = e1;
        f->IncidentEdge = e1;
    }
    else // implement a doubly linked list for other edges
    {
        HalfEdge *last = new HalfEdge;
        last = d->Edges->PreviousEdge;
        e1->NextEdge = d->Edges;
        d->Edges->PreviousEdge = e1;
        e1->PreviousEdge = last;
        last->NextEdge = e1;
    }
}
/*Function which takes the index as the input and provides the edge pointer with that index */
HalfEdge *FindEdgeInfo(DCEL *d, int index)
{
    for (int i = 0; i < d->Faces.size(); i++)
    {
        HalfEdge *edge = d->Faces[i]->IncidentEdge;
        if (edge != NULL)
        {
            do
            {
                if (edge->index == index)
                {
                    return edge;
                }

                else
                {
                    edge = edge->NextEdge;
                }
            } while (edge != d->Faces[i]->IncidentEdge);
        }
    }

    return NULL;
}
/*Function which takes the index as the input and provides the edge poijter and also stores its origin and destination vertex in a double pointer*/
HalfEdge *FindEdgeInfo(DCEL *d, int index, Vertex **v1, Vertex **v2)
{

    for (int i = 0; i < d->Faces.size(); i++)
    {
        HalfEdge *edge = d->Faces[i]->IncidentEdge;
        if (edge != NULL)
        {
            do
            {
                if (edge->index == index)
                {
                    *v1 = edge->Origin;
                    *v2 = edge->Destination;
                    return edge;
                }

                else
                {
                    edge = edge->NextEdge;
                }
            } while (edge != d->Faces[i]->IncidentEdge);
        }
    }

    return NULL;
}
/*Function to create the split edge.This edge is made from v1 to v2 with its face as f*/
HalfEdge *SplitEdge(DCEL *d, Vertex *v1, Vertex *v2, Face *f, HalfEdge *DestinationEdge, HalfEdge *OriginEdge, int index, class HalfEdge **OriginNext, class HalfEdge **DestinationPrev)
{
    *OriginNext = OriginEdge->NextEdge;
    *DestinationPrev = DestinationEdge->PreviousEdge;
    HalfEdge *e1 = new HalfEdge;
    e1->Twin = new HalfEdge;
    e1->Twin->IncidentFace = NULL;
    e1->Twin->index = e1->index = index;
    d->NumberOfEdges++;
    e1->Twin->Destination = e1->Origin = v1;
    e1->Twin->Origin = e1->Destination = v2;
    e1->IncidentFace = f;
    e1->NextEdge = DestinationEdge;
    e1->PreviousEdge = OriginEdge;
    e1->Twin->NextEdge = e1->Twin->PreviousEdge = NULL;
    return e1;
}
/*Function to calculate the slope of two given points**/
double getSlope(Vertex *v1, Vertex *v2)
{
    double x = v1->x - v2->x;
    double y = v1->y - v2->y;
    double slope = y / x;
    return slope;
}
/*Function to check if two points lie on the same side of the line*/
bool LineGivesSameSign(Vertex *v0, Vertex *v1, Vertex *v2, Vertex *v3)
{
    double slope = getSlope(v0, v1);
    double sign1 = (v2->y - v1->y - (slope * (v2->x - v1->x)));
    double sign2 = (v3->y - v1->y - (slope * (v3->x - v1->x)));
    if (sign1 * sign2 >= 0)
        return true;
    else
        return false;
}
/*Function to calculate the centroid of a given face*/
Vertex *getCentroid(class Face *f)
{
    HalfEdge *edge = f->IncidentEdge;
    int number_of_edges = 0;
    Vertex *centroid = new Vertex;
    do
    {
        centroid->x += edge->Origin->x;
        centroid->y += edge->Origin->y;
        number_of_edges++;
        edge = edge->NextEdge;

    } while (edge != f->IncidentEdge);
    centroid->x = (centroid->x) / number_of_edges;
    centroid->y = (centroid->y) / number_of_edges;
    return centroid;
}
/*Function to find the face index if a point is given*/
int getFace(DCEL *d, Vertex *v)
{
    int flag = -1;
    for (int j = 0; j < d->NumberOfFaces; j++)
    {
        HalfEdge *edge = d->Faces[j]->IncidentEdge;
        Vertex *c = getCentroid(d->Faces[j]);
        do
        {
            if (LineGivesSameSign(edge->Origin, edge->Destination, c, v))
            {
                edge = edge->NextEdge;
                flag = j;
            }
            else
            {
                flag = -1;
                break;
            }
        } while (edge != d->Faces[j]->IncidentEdge);
        if (flag >= 0)
            return flag;
    }
    return -1;
}

/*Function to change the Destination vertex of the source edge of the split(shrinking the edge)*/
HalfEdge *ChangeSourceEdge(DCEL *d, HalfEdge *e1, HalfEdge *SplitEdge, Vertex *v, HalfEdge *NewEdge1)
{
    e1->Destination = v;
    e1->NextEdge = SplitEdge;
    if (e1->Twin) //If its twin exists,will have to shrink that and also make a twin for the newly created edge
    {
        e1->Twin->Origin = v;
        NewEdge1->Twin = new HalfEdge;
        NewEdge1->Twin->index = NewEdge1->index;
        NewEdge1->Twin->Origin = NewEdge1->Destination;
        NewEdge1->Twin->Destination = NewEdge1->Origin;
        NewEdge1->Twin->IncidentFace = e1->Twin->IncidentFace;
        HalfEdge *temp = new HalfEdge;
        temp = e1->Twin->PreviousEdge;
        e1->Twin->PreviousEdge = NewEdge1->Twin;
        NewEdge1->Twin->PreviousEdge = temp;
        NewEdge1->Twin->NextEdge = e1->Twin;
        temp->NextEdge = NewEdge1->Twin;
    }
    return e1;
}
/*Function to change the origin vertex of the destination edge of the split(shrinking the edge)*/
HalfEdge *ChangeDestinationEdge(DCEL *d, HalfEdge *e1, HalfEdge *SplitEdge, Vertex *v, HalfEdge *NewEdge2)
{
    e1->Origin = v;
    e1->PreviousEdge = SplitEdge;
    if (e1->Twin) //If its twin exists,will have to shrink that and also make a twin for the newly created edge
    {
        NewEdge2->Twin = new HalfEdge;
        NewEdge2->Twin->index = NewEdge2->index;
        NewEdge2->Twin->Origin = NewEdge2->Destination;
        NewEdge2->Twin->Destination = NewEdge2->Origin;
        e1->Twin->Destination = v;
        NewEdge2->Twin->IncidentFace = e1->Twin->IncidentFace;
        HalfEdge *temp = new HalfEdge;
        temp = e1->Twin->NextEdge;
        e1->Twin->NextEdge = NewEdge2->Twin;
        NewEdge2->Twin->PreviousEdge = e1->Twin;
        NewEdge2->Twin->NextEdge = temp;
    }
    return e1;
}
/*Function which returns the midpoint of an edge*/
Vertex *getMidPoint(HalfEdge *e1, int index)
{
    Vertex *v1 = new Vertex;
    v1->x = (e1->Origin->x + e1->Destination->x) / 2;
    v1->y = (e1->Origin->y + e1->Destination->y) / 2;
    v1->index = index;
    return v1;
}
/*After creating the split edge,this function creates new edges and makes them into a doubly linked list along with the twin of the split edge whose incident facew will be a new face*/
void CreateNewEdges(DCEL *d, Face *f, HalfEdge *e, Vertex *v1, Vertex *v2, Vertex *v3, Vertex *v4, HalfEdge **NewEdge1, HalfEdge **NewEdge2, HalfEdge *SourceNext, HalfEdge *DestinationPrev, int index)
{
    //each split will create two new edges
    HalfEdge *e1 = new HalfEdge;

    d->NumberOfEdges++;
    HalfEdge *e2 = new HalfEdge;
    d->NumberOfEdges++;
    e1->index = index;
    e1->Twin = NULL;
    e2->Twin = NULL;
    e2->index = e1->index + 1;
    e1->Origin = v1;
    e1->Destination = v2;
    e2->Origin = v3;
    e2->Destination = v4;
    e->IncidentFace = f;
    e1->IncidentFace = f;
    e2->IncidentFace = f;

    if (v2 == v3) //traingle case
    {
        e->NextEdge = e1;
        e->PreviousEdge = e2;
        e1->NextEdge = e2;
        e1->PreviousEdge = e;
        e2->NextEdge = e;
        e2->PreviousEdge = e1;
    }
    else //genreal polygon case
    {
        e->NextEdge = e1;
        e->PreviousEdge = e2;
        e1->NextEdge = SourceNext;
        SourceNext->PreviousEdge = e1;
        e1->PreviousEdge = e;
        e2->NextEdge = e;
        SourceNext->IncidentFace = DestinationPrev->IncidentFace = f;
        HalfEdge *temp = new HalfEdge;
        temp = SourceNext->NextEdge;
        while (temp != DestinationPrev)
        {
            temp->IncidentFace = f;
            temp = temp->NextEdge;
        }
        e2->PreviousEdge = temp;
        temp->NextEdge = e2;
    }
    //storing the new edges formed
    *NewEdge1 = e1;
    *NewEdge2 = e2;
}
/*Function to print the indices of all the edges present in the entire DCEL*/
void printEdges(class DCEL *d, ofstream &filepointer)
{
    (filepointer) << "Edges:" << endl;
    for (int i = 0; i < d->NumberOfEdges; i++)
    {
        (filepointer) << i << endl;
    }
}
/*Function to print all the information about the edges by iterating through all the faces*/
void printHalfEdges(class DCEL *d, ofstream &filepointer)
{
    (filepointer) << "Halfedges: " << endl;
    for (int j = 0; j < d->NumberOfFaces; j++)
    {
        HalfEdge *edge = d->Faces[j]->IncidentEdge;
        do
        {
            (filepointer) << "startVertexIndex=" << edge->Origin->index << " ";
            (filepointer) << "endVertexIndex=" << edge->Destination->index << " ";
            (filepointer) << "nextEdge=" << edge->NextEdge->index << " ";
            (filepointer) << "previousEdge=" << edge->PreviousEdge->index << " ";
            (filepointer) << "faceIndex=" << d->Faces[j]->index << " ";
            (filepointer) << "edgeIndex=" << edge->index << endl;

            edge = edge->NextEdge;
        } while (edge != d->Faces[j]->IncidentEdge);
    }
}
/*Function to print the edges of each face by traversing through the linked list of edges*/
void printFaceEdges(class DCEL *d, ofstream &filepointer)
{
    (filepointer) << "Faces:" << endl;
    for (int i = 0; i < d->Faces.size(); i++)
    {
        HalfEdge *edge = d->Faces[i]->IncidentEdge;
        if (edge != NULL)
        {
            (filepointer) << "FaceIndex:" << d->Faces[i]->index << " Edges ";
            do
            {
                if (edge->NextEdge != d->Faces[i]->IncidentEdge)
                    (filepointer) << edge->index << "->";
                else
                    (filepointer) << edge->index << endl;
                edge = edge->NextEdge;
            } while (edge != d->Faces[i]->IncidentEdge);
        }
    }
}
/*Function to split two edges and create 2 new edges and a new face*/
DCEL *Split(DCEL *d, Face *face, HalfEdge *e_source, HalfEdge *e_destination)
{

    Vertex *source1_temp;
    Vertex *source2_temp;
    Vertex *destination1_temp;
    Vertex *destination2_temp;
    Vertex *midpoint1 = new Vertex;
    Vertex *midpoint2 = new Vertex;
    source1_temp = e_source->Origin;
    source2_temp = e_source->Destination;
    destination1_temp = e_destination->Origin;
    destination2_temp = e_destination->Destination;
    midpoint1 = getMidPoint(e_source, d->Vertices.size());
    d->NumberOfVertices++;
    d->Vertices.push_back(midpoint1);
    midpoint2 = getMidPoint(e_destination, d->Vertices.size());
    d->NumberOfVertices++;
    d->Vertices.push_back(midpoint2);
    HalfEdge *splitedge = new HalfEdge;
    HalfEdge *temp_source_next;
    HalfEdge *temp_dest_prev;

    splitedge = SplitEdge(d, midpoint1, midpoint2, face, e_destination, e_source, d->NumberOfEdges, &(temp_source_next), &(temp_dest_prev));
    Face *new_face = new Face;
    new_face->index = d->NumberOfFaces;
    new_face->IncidentEdge = splitedge->Twin;
    HalfEdge *NewEdge1 = new HalfEdge;
    HalfEdge *NewEdge2 = new HalfEdge;
    CreateNewEdges(d, new_face, splitedge->Twin, midpoint1, source2_temp, destination1_temp, midpoint2, &NewEdge1, &NewEdge2, temp_source_next, temp_dest_prev, d->NumberOfEdges);
    e_destination = ChangeDestinationEdge(d, e_destination, splitedge, midpoint2, NewEdge2);
    e_source = ChangeSourceEdge(d, e_source, splitedge, midpoint1, NewEdge1);
    if (face != face->IncidentEdge->IncidentFace)
    {
        face->IncidentEdge = splitedge;
    }
    d->NumberOfFaces++;
    d->Faces.push_back(new_face);
    return d;
}
int main(int argc, char *argv[])
{
    DCEL *d = new DCEL;
    ifstream file_input;
    d->NumberOfEdges = 0;
    Face *face = new Face;
    face->index = 0;
    d->NumberOfFaces = 1;
    d->Faces.push_back(face);
    int number_of_vertices = 4;
    int number_of_edges = 4;
    int index1, index2;
    Vertex_input vertices[number_of_vertices];
    file_input.open(argv[1]);

    for (int i = 0; i < number_of_vertices; i++)
    {
        file_input >> vertices[i].x >> vertices[i].y;
    }

    for (int i = 0; i < number_of_vertices; i++)
    {
        Vertex *v = new Vertex;
        v->x = vertices[i].x;
        v->y = vertices[i].y;
        v->index = i;
        d->Vertices.push_back(v);
        d->NumberOfVertices++;
    }
    int i;
    for (i = 0; i < number_of_vertices - 1; i++) //making the initial square
    {
        makeEdge(d, d->Vertices[i], d->Vertices[i + 1], face, i);
    }
    makeEdge(d, d->Vertices[i], d->Vertices[0], face, i);

    ofstream file_output;
    file_output.open(argv[3]);
    printEdges(d, file_output);
    printHalfEdges(d, file_output);
    int splits;
    printFaceEdges(d, file_output);
    ifstream file_input2;
    file_input2.open(argv[2]);
    HalfEdge *edge1 = new HalfEdge;
    HalfEdge *edge2 = new HalfEdge;
    string ch = "";
    int id;
    file_input2 >> ch;
    while (file_input2.eof() == false)
    {

        if (ch == "Split")
        {
            file_input2 >> index1 >> index2;
            edge1 = FindEdgeInfo(d, index1);
            edge2 = FindEdgeInfo(d, index2);
            if (edge1->IncidentFace == edge2->IncidentFace) //before passing the edges to the split function,the edges should have the same incident face.
            {
                if (edge1->Twin)
                    edge1->Twin->Twin = edge1;
                if (edge2->Twin)
                    edge2->Twin->Twin = edge2;
            }
            if (edge1->Twin)
            {
                if (edge1->Twin->IncidentFace == edge2->IncidentFace)
                {
                    HalfEdge *tempTwin = new HalfEdge;
                    tempTwin = edge1;
                    edge1 = edge1->Twin;
                    edge1->Twin = tempTwin;
                }
            }
            if (edge2->Twin)
            {
                if (edge1->IncidentFace == edge2->Twin->IncidentFace)
                {
                    HalfEdge *tempTwin = new HalfEdge;
                    tempTwin = edge2;
                    edge2 = edge2->Twin;
                    edge2->Twin = tempTwin;
                }
            }
            if (edge1->Twin && edge2->Twin)
            {
                if (edge1->Twin->IncidentFace == edge2->Twin->IncidentFace)
                {
                    HalfEdge *tempTwin1 = new HalfEdge;
                    HalfEdge *tempTwin2 = new HalfEdge;
                    tempTwin1 = edge1;
                    edge1 = edge1->Twin;
                    edge1->Twin = tempTwin1;
                    tempTwin2 = edge2;
                    edge2 = edge2->Twin;
                    edge2->Twin = tempTwin2;
                }
            }

            d = Split(d, edge1->IncidentFace, edge1, edge2);
            file_output << "\n//After split\n\n";
            printEdges(d, file_output);
            printHalfEdges(d, file_output);
            printFaceEdges(d, file_output);
        }
        else if (ch == "Id:")
        {
            Vertex input;
            file_input2 >> id >> input.x >> input.y;
            int answer = getFace(d, &input);
            file_output << "Id: " << id << " " << answer << endl;
        }
        file_input2 >> ch;
    }
    return 0;
}
