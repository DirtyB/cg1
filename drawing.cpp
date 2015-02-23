#include <windows.h>
#include <stdio.h>

#ifndef _DRAWING_H_
#include "drawing.h"
#define _DRAWING_H_
#endif

point p[8];

void CalculatePoints(const camera &cam){
    
    vector v[8];
    v[0].X[0]=0; v[0].X[1]=0; v[0].X[2]=0;
    v[1].X[0]=1; v[1].X[1]=0; v[1].X[2]=0;
    v[2].X[0]=0; v[2].X[1]=1; v[2].X[2]=0;
    v[3].X[0]=1; v[3].X[1]=1; v[3].X[2]=0;
    v[4].X[0]=0; v[4].X[1]=0; v[4].X[2]=1;
    v[5].X[0]=1; v[5].X[1]=0; v[5].X[2]=1;
    v[6].X[0]=0; v[6].X[1]=1; v[6].X[2]=1;
    v[7].X[0]=1; v[7].X[1]=1; v[7].X[2]=1;
    
    
    for(int i=0;i<8;i++){
        p[i] = SpaceToScreen(cam,v[i]);
    }
}


void DrawSomething(HDC hdc,const camera &cam){
    /*for(int i=0;i<10;i++){
        SetPixel(hdc,i*10,i,RGB(0,0,0));
    }*/
    /*MoveToEx(hdc, 100,100, NULL); 
    LineTo(hdc, 200, 200);*/
    
    RECT r;
    
   
    MoveToEx(hdc, p[0].x, p[0].y, NULL); 
    LineTo(hdc, p[1].x, p[1].y);
    r.left = p[1].x;
    r.right = r.left+20;
    r.top = p[1].y;
    r.bottom = r.top+20;
    DrawText(hdc, "x", -1, &r, 0);
    
    MoveToEx(hdc, p[0].x, p[0].y, NULL); 
    LineTo(hdc, p[2].x, p[2].y);
    r.left = p[2].x;
    r.right = r.left+20;
    r.top = p[2].y;
    r.bottom = r.top+20;
    DrawText(hdc, "y", -1, &r, 0);
    
    MoveToEx(hdc, p[0].x, p[0].y, NULL); 
    LineTo(hdc, p[4].x, p[4].y);
    r.left = p[4].x;
    r.right = r.left+20;
    r.top = p[4].y;
    r.bottom = r.top+20;
    DrawText(hdc, "z", -1, &r, 0);
}

bool ReadCamera(const char* filename, camera &cam){
    
    FILE* file = fopen(filename,"r");
    
    if(!file){
        return false;   
    }
    
    fscanf(file,"%i",&cam.iFrameWidth);
    fscanf(file,"%i",&cam.iFrameHeight);
    fscanf(file,"%lf",&cam.lfFocus);
    fscanf(file,"%lf",&cam.lfPixelScale);
        
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            fscanf(file,"%lf",&cam.A[i][j]);
        }
    }
    for(int i=0;i<3;i++){
        fscanf(file,"%lf",&cam.X0[i]);
    }
    
    fclose(file);
    
    file = fopen("debug.txt","w");
    
    fprintf(file,"%i\n",cam.iFrameWidth);
    fprintf(file,"%i\n",cam.iFrameHeight);
    fprintf(file,"%lf\n",cam.lfFocus);
    fprintf(file,"%lf\n",cam.lfPixelScale);
        
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            fprintf(file,"%lf ",cam.A[i][j]);
        }
        fprintf(file,"\n");
    }
    for(int i=0;i<3;i++){
        fprintf(file,"%lf ",cam.X0[i]);
    }
    fprintf(file,"\n");
    
    fclose(file);
    
    
    return true;
}

point SpaceToScreen(const camera &cam,const vector &v){
    double relX[3];
    for(int i=0;i<3;i++){
        relX[i] = v.X[i]-cam.X0[i];
    }
    double x[3];
    for(int i=0;i<3;i++){
        x[i] = 0;
        for(int j=0;j<3;j++){
            x[i] += cam.A[j][i]*relX[j];
        }
    }
    double p[2];
    int pPix[2];
    for(int i=0;i<2;i++){
        p[i] = x[i]*cam.lfFocus/(x[2]+cam.lfFocus);
        pPix[i] = (int)(p[i]/cam.lfPixelScale);
    }
    point P;
    P.x = (cam.iFrameWidth/2)+pPix[0];
    P.y = (cam.iFrameHeight/2)-pPix[1];
    
    FILE* file = fopen("debug.txt","a");
    fprintf(file,"====\n");
    for(int i=0;i<3;i++){
        fprintf(file,"%lf %lf %lf ",v.X[i],relX[i],x[i]);
        if(i!=2){
            fprintf(file,"%lf %i ",p[i],pPix[i]);
            if(i==0){
                fprintf(file,"%i ",P.x);
            }
            else{
                fprintf(file,"%i ",P.y);
            }
        }
        fprintf(file,"\n");
    }
    
    fclose(file);
    
    return P;
}

vector StraightLine(const straightLine &line, double t){
    vector res;
    for(int i=0;i<3;i++){
        res.X[i] = line.K[i]*t + line.B[i];
    }
    return res;
}
vector StraightLineDiff(const straightLine &line, double t){
    vector res;
    for(int i=0;i<3;i++){
        res.X[i] = line.K[i];
    }
    return res;
}
