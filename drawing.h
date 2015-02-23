struct camera {
    int iFrameWidth;
    int iFrameHeight;
    double lfPixelScale;
    double lfFocus;
    double A[3][3];
    double X0[3];
};

struct vector {
    double X[3];
};

struct point {
    int x;
    int y;
};

struct straightLine {
    double K[3];
    double B[3];
};

void CalculatePoints(const camera &cam);

void DrawSomething(HDC hdc,const camera &cam);

bool ReadCamera(const char* filename, camera &cam);

point SpaceToScreen(const camera &cam,const vector &v); 

vector StraightLine(const straightLine &line, double t);
vector StraightLineDiff(const straightLine &line, double t);
