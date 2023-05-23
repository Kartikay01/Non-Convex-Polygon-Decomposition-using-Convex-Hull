//#include <iostream>
#include<bits/stdc++.h>
#include <chrono>
using namespace std::chrono;

using namespace std;
//Assumed vertices provided in clockwise direction


struct Point {
    int x, y;int id;
};
// for implementation of DCEL data structure
struct HalfEdge;struct Face;
struct Vertex {

    int x;
    int y;
    int id;
    vector<HalfEdge*> incident_edge;
};

struct HalfEdge {

    HalfEdge* twin;
    HalfEdge* next;
    HalfEdge* prev;
    Vertex* incident_vertex;
    Face* incident_face;
};

struct Face {
public:
    vector<HalfEdge*> incident_edge;
};

//functions for implementation

    vector<Vertex*> vertices;
    vector<HalfEdge*> half_edges;
    vector<Face*> faces;
    unordered_map<int,int> u;
    Vertex* add_vertex(Point P) {
        int f1=P.id;
        auto it=u.find(f1);
        if(it!=u.end())
        {
            int j1=it->second;return vertices[j1];
        }

        Vertex* v=new Vertex();
        v->x =P.x;
        v->y =P.y;
        v->id=P.id;
       // v.incident_edge = -1;
        vertices.push_back(v);int l=vertices.size()-1;
        u.insert({f1,l});
        return v;
    }

//function for adding face
    Face* add_face(HalfEdge* incident_edge) {
        Face *f=new Face();
        f->incident_edge.push_back(incident_edge);
        faces.push_back(f);
        return f;
    }

//function for adding half edge
    HalfEdge* add_half_edge(HalfEdge* twin, HalfEdge* prev, Vertex* incident_vertex) {
        HalfEdge* e=new HalfEdge();
        e->twin = twin;

        e->prev = prev;
        if(twin!=nullptr)
        twin->twin=e;
        if(prev!=nullptr)
        prev->next=e;
        e->incident_vertex = incident_vertex;
        e->incident_face =add_face(e);
        half_edges.push_back(e); incident_vertex->incident_edge.push_back(e);
        return e;
    }





//end of structures and functions for implementation of DCEL data structure

//functions to find notches in a polygon

// calculate the angle between two line segments
double angle(Point p1, Point p2, Point p3) {
    double dx1 = p1.x - p2.x;
    double dy1 = p1.y - p2.y;
    double dx2 = p3.x - p2.x;
    double dy2 = p3.y - p2.y;
    double dot = dx1 * dx2 + dy1 * dy2;
    double cross = dx1 * dy2 - dy1 * dx2;
    return atan2(cross, dot);
}

// check if a vertex is a notch
bool isNotch(Point p1, Point p2, Point p3) {
    double a = angle(p1, p2, p3);
    return a > M_PI;
}

// find all notches in a polygon
unordered_set<int> findNotches(vector<Point> points) {
    int n = points.size();
    unordered_set<int> notches;
    for (int i = 0; i < n; i++) {
        Point p1 = points[(i-1+n)%n];
        Point p2 = points[i];
        Point p3 = points[(i+1)%n];
        if (isNotch(p1, p2, p3)) {
            notches.insert(i);
        }
    }
    return notches;
}


//end of functions to find notches in a polygon

// function to check if a point lies inside a polygon using ray casting algorithm
bool isPointInsidePolygon( vector<Point>& polygon,  Point& p) {
    int n = polygon.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}
//checks if the angle is reflex or not
bool isReflex(Point p1, Point p2, Point p3) {
    int crossProduct = (p2.x - p1.x) * (p3.y - p2.y) - (p3.x - p2.x) * (p2.y - p1.y);
    return crossProduct > 0;
}
//function to calculate cordinates of rectangle with maximum area
vector<Point> minARect(vector<Point>& polygon)
{
    int xmin=INT_MAX;int xmax=INT_MIN;int ymin=INT_MAX;int ymax=INT_MIN;

    for(int i=0;i<polygon.size();i++)
    {
        int i1=polygon[i].x;int j1=polygon[i].y;
        xmin=min(xmin,i1);xmax=max(xmax,i1);ymin=min(ymin,j1);ymax=max(ymax,j1);
    }

vector<Point> res;Point p1;Point p2;Point p3;Point p4;p1.x=xmin;p1.y=ymin;p4.x=xmax;p4.y=ymin;p2.x=xmin;p2.y=ymax;p3.x=xmax;p3.y=ymax;
res.push_back(p1);res.push_back(p2);res.push_back(p3);res.push_back(p4);

return res;
}
//function implents the algo
vector<vector<Point>> algoImp(vector<Point>& polygon)
{

    int n=polygon.size();vector<vector<Point>> res;vector<vector<int>> res1;

    //implementation of part 3 in algo 1 as given in paper
    while(n>3)
    {
        //impementation of part 3.1 and 3.2 in algo 1 as given in paper
        vector<Point> L;vector<int> index;
        Point v1=polygon[0];int ind=0;
        if(res1.size()>0)
        {

            ind=res1[res1.size()-1][res1[res1.size()-1].size()-1];v1=polygon[ind];

        }
        int i1=ind;int i2=(ind+1)%(polygon.size());
        Point v2=polygon[i2];L.push_back(v1);L.push_back(v2);index.push_back(i1);index.push_back(i2);
        int next=(index[index.size()-1]+1)%(polygon.size());

        //end of impementation of part 3.1 and 3.2 in algo 1 as given in paper

        //impementation of part 3.3 in algo 1 as given in paper
while((!isReflex(L[L.size()-2],L[L.size()-1],polygon[next])) &&(!isReflex(L[L.size()-1],polygon[next],L[0])) && (!isReflex(polygon[next],L[0],L[1])) && L.size()<n )
        {
            L.push_back(polygon[next]);index.push_back(next);next=(index[index.size()-1]+1)%(polygon.size());

        }
        //end of impementation of part 3.3 in algo 1 as given in paper
        //impementation of part 3.4 in algo 1 as given in paper
        unordered_set<int> u(index.begin(),index.end());
        if(L.size()!=n)
        {
            unordered_set<int> LPVS;
            //vector<int> notchIndex=findNotches(polygon);
            vector<Point> polygonNotch;
            for(int lp=0;lp<polygon.size();lp++)
            {
                if(u.find(lp)==u.end())
                polygonNotch.push_back(polygon[lp]);
            }
            LPVS=findNotches(polygonNotch);

            while(LPVS.size()>0 && L.size()>2)
            {
                vector<Point> miRect=minARect(L);vector<int> rem;
                for(auto x:LPVS)
                {
                        bool b=isPointInsidePolygon(miRect,polygon[x]);
                        if(b==false)
                        rem.push_back(x);
                }
                for(int lp1=0;lp1<rem.size();lp1++)
                LPVS.erase(rem[lp1]);

                vector<int> rem1;

                 for(auto x:LPVS)
                {
                        bool b=isPointInsidePolygon(L,polygon[x]);
                        if(b==false)
                        rem1.push_back(x);
                }
                 for(int lp1=0;lp1<rem1.size();lp1++)
                LPVS.erase(rem1[lp1]);

                if(LPVS.size()>0 && L.size()>0)
                {L.pop_back();u.erase(index[index.size()-1]);
                index.pop_back();}

            }

        }
//end of impementation of part 3.4 in algo 1 as given in paper
// impementation of part 3.5 in algo 1 as given in paper
        if(L.size()>2)
        {
            //adding the diagnol to DCEL
            Vertex* diagV1=add_vertex(L[0]);Vertex* diagV2=add_vertex(L[L.size()-1]);
            HalfEdge* diagEdge1=add_half_edge(nullptr,nullptr,diagV1);
            HalfEdge* diagEdge2=add_half_edge(diagEdge1,nullptr,diagV2);
            //end of adding diagnol to DCEL
            res.push_back(L);res1.push_back(index);polygon.push_back(L[0]);n=n-index.size()+2;

        }
      //end of impementation of part 3.5 in algo 1 as given in paper
    }
//end of implementation of part 3 in algo 1 as given in paper
if(n==3)
{

    int lenP=polygon.size();
    vector<Point> L;L.push_back(polygon[lenP-3]);L.push_back(polygon[lenP-2]);L.push_back(polygon[lenP-1]);
    res.push_back(L);
}
return res;
}


int main()
{
vector<Point> polygon;
int n=0;
cout<<"Enter value of n"<<endl;cin>>n;

for(int i=0;i<n;i++)
{
    Point p1;p1.id=i;
    cout<<"Enter x value"<<endl;
    cin>>p1.x;
    cout<<"Enter y value"<<endl;
    cin>>p1.y;
    polygon.push_back(p1);
}
//turning polygon to the DCEL
vector<HalfEdge*> h1;
for(int i1=0;i1<polygon.size()-1;i1++)
{
    Point P1=polygon[i1];
    Vertex* vcr=add_vertex(P1);HalfEdge* h2;
        if(h1.size()==0)
        {
             h2=add_half_edge(nullptr,nullptr,vcr);
        }
        else
        {
             h2=add_half_edge(nullptr,h1[h1.size()-1],vcr);
        }
        h1.push_back(h2);
}
 int co=h1.size()-1;vector<HalfEdge*> h3;
 for(int i1=polygon.size()-1;i1>0;--i1)
 {
    Point P1=polygon[i1];
    Vertex* vcr=add_vertex(P1);HalfEdge* h2;

         if(h3.size()==0)
        {
             h2=add_half_edge(h1[co],nullptr,vcr);
        }
        else
        {
             h2=add_half_edge(h1[co],h3[h3.size()-1],vcr);
        }
        h3.push_back(h2);co--;


 }
  Vertex* diagV1=add_vertex(polygon[0]);
  Vertex* diagV2=add_vertex(polygon[polygon.size()-1]);

HalfEdge* h2= add_half_edge(nullptr,h3[h3.size()-1],diagV1);
add_half_edge(h2,h1[h1.size()-1],diagV2);
//end of turning polygon to DCEL
//starting timer
auto start = high_resolution_clock::now();
vector<vector<Point>> res=algoImp(polygon);
auto stop = high_resolution_clock::now();
auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by algorithm: "
         << duration.count() << " microseconds" << endl;
//ending timer and printing it
//printing the decomposition just for our reference

for(int i=0;i<res.size();i++)
{

    for(int i1=0;i1<res[i].size();i1++)
    {
        Point P1=res[i][i1];cout<<"New Point"<<P1.x<<" "<<P1.y;

    }
   cout<<endl;

    }



}
