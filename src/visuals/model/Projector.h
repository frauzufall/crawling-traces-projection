#pragma once

#include "MappingQuad.h"
#include "Visuals.h"
#include "Traces.h"

namespace guardacaso {

    typedef std::shared_ptr<MappingQuad> MappingQuad_ptr;

	class Projector {
	
	public:
	
		Projector() {
            quads.clear();
            quads.resize(0);
            start_point = ofPoint(0,0);
            plane[0] = ofPoint(0, 0, 0);
            plane[1] = ofPoint(1, 0, 0);
            plane[2] = ofPoint(1, 1, 0);
            plane[3] = ofPoint(0, 1, 0);
            camera.addVertex(ofPoint(0, 0, 0));
            camera.addVertex(ofPoint(1, 0, 0));
            camera.addVertex(ofPoint(1, 1, 0));
            camera.addVertex(ofPoint(0, 1, 0));
            camera.close();
		}

        ~Projector() {
            stringstream sstr_svg;
            sstr_svg << Traces::get().historyDir() << "/mapping.svg";
            Visuals::get().saveMappingAsSvg(sstr_svg.str());
        }

        void update() {
            MappingQuad_ptr mq;
            for(uint i = 0; i < quadCount(); i++) {
                mq = getQuad(i);
                if(mq) {
                    if(mq->newpos) {
                        findHomography(mq->dst, mq->src, (GLfloat*) mq->matrix_src_dst.getPtr(), true);
                        findHomography(plane, mq->dst, (GLfloat*) mq->matrix_dst_norm.getPtr(), true);
                        Visuals::get().updateOutlines();
                        mq->newpos = false;
                    }
                }
            }
        }

        ofPoint relative(ofPoint orig) {
            return orig*getMatrixOfImageAtPoint(orig);
        }

        ofPoint inContent(ofPoint orig) {
            ofPoint res = orig;
            res.x*=Visuals::get().contentWidth();
            res.y*=Visuals::get().contentHeight();
            return res;
        }

        ofPoint inOutput(ofPoint orig) {
            ofPoint res = orig;
            res.x*=Visuals::get().outputWidth();
            res.y*=Visuals::get().outputHeight();
            return res;
        }
		
        MappingQuad_ptr addQuad(string type, bool swap = false, bool update_mapping = true) {
            quads.push_back(MappingQuad_ptr(new MappingQuad(type)));
            if(update_mapping)
                update();
            if(swap) {
                for(int i = quads.size()-1; i>0;i--) {
                 swapQuads(i, i-1);
                }
                return quads.at(0);
            }
            return quads.at(quads.size()-1);
		}

        void addObstacle() {
            addQuad("window");
        }

        void addCanvas() {
            addQuad("painting");
        }

        void addImage(string url) {
            MappingQuad_ptr mq = addQuad("picture");
            mq->img_src.set(url);
            mq->image.load(url);

        }
		
		void removeQuad(int id) {
            quads.at(id).reset();
            quads.erase(quads.begin()+id);
            if(getFirstImageQuad() == 0) {
                addQuad("painting");
            }
		}

        void removeAllQuads() {
            quads.clear();
        }

        bool swapQuads(int index1, int index2) {
            if(index1 < 0 || index2 < 0) {
                return false;
            }
            if((uint)index1<quads.size() && (uint)index2 < quads.size()) {
                std::swap( quads[index1], quads[index2] );
                std::swap( Visuals::get().outlinesRaw()->at(index1), Visuals::get().outlinesRaw()->at(index2) );
                std::swap( Visuals::get().outlines()->at(index1), Visuals::get().outlines()->at(index2) );
                return true;
            }
            return false;
        }
		
        MappingQuad_ptr getQuad(int id) {
			if(id < (int)quads.size()) {
                return quads[id];
			}
			else {
                cout << "ERROR: projector: trying to get quad " << id << " but quads size is " << quads.size() << endl;
                return MappingQuad_ptr();
			}
				
		}
		
        uint quadCount() {
			return quads.size();
		}

        MappingQuad_ptr getFirstImageQuad() {
            MappingQuad_ptr mq;
            MappingQuad_ptr mq_res;
            for(uint i = 0; i < quadCount(); i++) {
                mq = quads.at(i);
                if(mq) {
                    if(mq->nature == "painting") {
                        mq_res = mq;
                        break;
                    }
                }
            }
            return mq_res;
        }

        vector<MappingQuad_ptr> getQuadsOfType(string type) {
            vector<MappingQuad_ptr> res;
            res.clear();
            MappingQuad_ptr mq;
            for(uint i = 0; i < quadCount(); i++) {
                mq = getQuad(i);
                if(mq) {
                    if(mq->nature == type) {
                        res.push_back(mq);
                    }
                }
            }
            return res;
        }

        ofMatrix4x4 getMatrixOfImageAtPoint(ofPoint p) {
            vector<MappingQuad_ptr> images = getQuadsOfType("painting");
            for(uint ii = 0; ii < images.size(); ii++) {
                if( pointVisibleInQuad(p,images.at(ii))) {
                    return images.at(ii)->matrix_src_dst;
                }
            }
            return ofMatrix4x4::newIdentityMatrix();
        }

        bool pointVisibleInQuad(ofPoint p, MappingQuad_ptr mq) {

            ofPoint poly[4];

            uint i, j=3;
            ofPoint last_p = p;
            ofPoint next_p(-100,100);
            int intersections = 0;

            for (i=0; i<4; i++) {
                poly[i] = inContent(mq->src[i]);
            }

            for (i=0; i<4; i++) {

                if(isLeft(poly[i],poly[j], last_p) != isLeft(poly[i],poly[j], next_p)
                        && isLeft(next_p,last_p, poly[i]) != isLeft(next_p, last_p, poly[j])) {
                    intersections++;
                }
                j=i;
            }

            return intersections%2;
        }

        bool isLeft(ofPoint a, ofPoint b, ofPoint c){
             return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) > 0;
        }

        void gaussian_elimination(float *input, int n)
        {
            // ported to c from pseudocode in
            // http://en.wikipedia.org/wiki/Gaussian_elimination

            float * A = input;
            int i = 0;
            int j = 0;
            int m = n-1;
            while (i < m && j < n)
            {
                // Find pivot in column j, starting in row i:
                int maxi = i;
                for(int k = i+1; k<m; k++)
                {
                    if(fabs(A[k*n+j]) > fabs(A[maxi*n+j]))
                    {
                        maxi = k;
                    }
                }
                if (A[maxi*n+j] != 0)
                {
                    //swap rows i and maxi, but do not change the value of i
                    if(i!=maxi)
                        for(int k=0; k<n; k++)
                        {
                            float aux = A[i*n+k];
                            A[i*n+k]=A[maxi*n+k];
                            A[maxi*n+k]=aux;
                        }
                    //Now A[i,j] will contain the old value of A[maxi,j].
                    //divide each entry in row i by A[i,j]
                    float A_ij=A[i*n+j];
                    for(int k=0; k<n; k++)
                    {
                        A[i*n+k]/=A_ij;
                    }
                    //Now A[i,j] will have the value 1.
                    for(int u = i+1; u< m; u++)
                    {
                        //subtract A[u,j] * row i from row u
                        float A_uj = A[u*n+j];
                        for(int k=0; k<n; k++)
                        {
                            A[u*n+k]-=A_uj*A[i*n+k];
                        }
                        //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
                    }

                    i++;
                }
                j++;
            }

            //back substitution
            for(int i=m-2; i>=0; i--)
            {
                for(int j=i+1; j<n-1; j++)
                {
                    A[i*n+m]-=A[i*n+j]*A[j*n+m];
                    //A[i*n+j]=0;
                }
            }
        }

        void findHomography(ofPoint src_norm[4], ofPoint dst_norm[4], float homography[16], bool points_normalized)
        {

            ofPoint src[4];
            ofPoint dst[4];

            if(points_normalized) {
                for(int i = 0; i < 4; i++) {
                    src[i].x=src_norm[i].x*Visuals::get().outputWidth();
                    src[i].y=src_norm[i].y*Visuals::get().outputHeight();
                    dst[i].x=dst_norm[i].x*Visuals::get().outputWidth();
                    dst[i].y=dst_norm[i].y*Visuals::get().outputHeight();
                }
            }
            else {
                for(int i = 0; i < 4; i++) {
                    src[i].x=src_norm[i].x;
                    src[i].y=src_norm[i].y;
                    dst[i].x=dst_norm[i].x;
                    dst[i].y=dst_norm[i].y;
                }
            }

            // create the equation system to be solved
            //
            // from: Multiple View Geometry in Computer Vision 2ed
            //       Hartley R. and Zisserman A.
            //
            // x' = xH
            // where H is the homography: a 3 by 3 matrix
            // that transformed to inhomogeneous coordinates for each point
            // gives the following equations for each point:
            //
            // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
            // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
            //
            // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
            // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
            // after ordering the terms it gives the following matrix
            // that can be solved with gaussian elimination:

            float P[8][9]=
            {
                {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
                {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12

                {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
                {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21

                {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
                {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23

                {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
                {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
            };

            gaussian_elimination(&P[0][0],9);

            // gaussian elimination gives the results of the equation system
            // in the last column of the original matrix.
            // opengl needs the transposed 4x4 matrix:
            float aux_H[]= { P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31
                             P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32
                             0      ,      0,0,0,       // 0    0   0 0
                             P[2][8],P[5][8],0,1
                           };      // h13  h23 0 h33

            for(int i=0; i<16; i++) homography[i] = aux_H[i];
        }

        ofPoint getStartPoint() {
            return start_point;
        }

        void setStartPoint(ofPoint p) {
            start_point = p;
        }

        ofPolyline& getCamera() {
            return camera;
        }
		
	private:
        vector<MappingQuad_ptr> quads;
        ofPoint                 start_point;
        ofPoint                 plane[4];
        ofPolyline              camera;
		
	};

}

