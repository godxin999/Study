#include "my_gl.hpp"

Model* model = nullptr;
constexpr int height=2000;
constexpr int width=2000;
constexpr float depth=2000.f;
std::vector<float> zbuffer(width*height,min_float);
std::vector<float> shadowbuffer(width*height,min_float);
Vec3f light_dir={1,1,1};
Vec3f eye{1,1,3};
Vec3f center{0,0,0};
Vec3f up{0,1,0};

Mat44f Model_;
Mat44f View;
Mat44f Projection;
Mat44f Viewport;

void render(TGAImage &image){
    Model_=model_matrix();
    View=view_matrix(eye,center,up);
    Projection=projection_matrix((eye-center).norm());
    Viewport=viewport_matrix(width/8,height/8,width*3/4,height*3/4);
    PhongShader shader(model);
    
    for(int i=0;i<model->nfaces();++i){
        Vec4f clip_vert[3];
        for(int j=0;j<3;++j){
            shader.vertex(i,j,clip_vert[j]);
        }
        triangle(clip_vert,shader,image,zbuffer);
    }
}

int main() {
    model=new Model("../../../../assets/african_head/african_head.obj");
    //model=new Model("../../../../assets/diablo3_pose/diablo3_pose.obj");
    TGAImage image(width, height, TGAImage::RGB);
    /*{
        TGAImage shadow(width,height,TGAImage::RGB);
        Model_=model_matrix();
        View=view_matrix(light_dir.normalize(),center,up);
        Projection=projection_matrix(0);
        Viewport=viewport_matrix(width/8,height/8,width*3/4,height*3/4);
        DepthShader shader(model);
        for(int i=0;i<model->nfaces();++i){
            Vec4f clip_vert[3];
            for(int j=0;j<3;++j){
                shader.vertex(i,j,clip_vert[j]);
            }
            triangle(clip_vert,shader,shadow,shadowbuffer);
        }
        shadow.flip_vertically();
        shadow.write_tga_file("depth.tga");
    }*/



    render(image);
    image.flip_vertically();
	image.write_tga_file("output.tga");

    delete model;
    return 0;
}
