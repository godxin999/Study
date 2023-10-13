import bpy #Blender的Python模块
import bmesh #Blender的网格模块
import struct #python的二进制数据处理模块
import os #python的操作系统模块

class EngineVertex:
    x=y=z=r=g=b=a=u=v=0

    def __init__(self,x,y,z,r,g,b,a,u,v):
        self.x=x
        self.y=y
        self.z=z
        self.r=r
        self.g=g
        self.b=b
        self.a=a
        self.u=u
        self.v=v
    
    def __eq__(self,other):
        return self.x==other.x and self.y==other.y and self.z==other.z and self.r==other.r and self.g==other.g and self.b==other.b and self.a==other.a and self.u==other.u and self.v==other.v
    
    def __str__(self) -> str:
        return "x:{:.2f},y:{:.2f},z:{:.2f},r:{:.2f},g:{:.2f},b:{:.2f},a:{:.2f},u:{:.2f},v:{:.2f}".format(self.x,self.y,self.z,self.r,self.g,self.b,self.a,self.u,self.v)
    
    def __repr__(self) -> str:
        return str(self)
    
    #将引擎节点写入文件
    def write(self,f):
        f.write(struct.pack('f',self.x))
        f.write(struct.pack('f',self.y))
        f.write(struct.pack('f',self.z))
        f.write(struct.pack('f',self.r))
        f.write(struct.pack('f',self.g))
        f.write(struct.pack('f',self.b))
        f.write(struct.pack('f',self.a))
        f.write(struct.pack('f',self.u))
        f.write(struct.pack('f',self.v))

#在blender中显示一个包含指定消息、标题、图标的消息框
def ShowMessageBox(message = "", title = "Message Box", icon = 'INFO'):
    def draw(self, context):
        self.layout.label(text=message)
    bpy.context.window_manager.popup_menu(draw, title = title, icon = icon)

#当前选中的Mesh
mesh_selected = bpy.context.object.data
uv_layer = mesh_selected.uv_layers.active.data

print("----------Exporting the Mesh----------")

#顶点数组和顶点索引数组
engine_vertices=[]
engine_vertex_indices=[]

for poly in mesh_selected.polygons:
    if poly.loop_total==4:
        ShowMessageBox("The mesh has quads, please triangulate it first!","Polygon Error",'ERROR')
        break

    for loop_index in range(poly.loop_start, poly.loop_start + poly.loop_total):
        vertex_index=mesh_selected.loops[loop_index].vertex_index
        v=mesh_selected.vertices[vertex_index]
        uv=uv_layer[loop_index].uv

        #构造引擎节点，并调转Y轴和Z轴
        engine_vertex=EngineVertex(v.co.x,v.co.z,v.co.y,1,1,1,1,uv.x,uv.y)

        find_engine_vertex_index=-1
        for engine_vertex_index in range(len(engine_vertices)):
            if engine_vertex==engine_vertices[engine_vertex_index]:
                find_engine_vertex_index=engine_vertex_index
                break
        
        if find_engine_vertex_index==-1:
            find_engine_vertex_index=len(engine_vertices)
            engine_vertices.append(engine_vertex)

        engine_vertex_indices.append(find_engine_vertex_index)


blender_project_path = os.path.dirname(bpy.data.filepath)#获取blender工程路径
if os.path.exists(blender_project_path+"/export")==False:
    os.mkdir(blender_project_path+"/export")#没有import文件夹就创建一个

with open(blender_project_path+"/export/export.mesh","wb") as f:#打开文件
    #写入文件头
    f.write("mesh".encode())
    f.write(struct.pack('H',len(engine_vertices)))#顶点数量
    f.write(struct.pack('H',len(engine_vertex_indices)))#顶点索引数量
    
    #写入顶点数据
    for engine_vertex in engine_vertices:
        engine_vertex.write(f)

    #写入顶点索引数据
    kVertexIndexVectorInverse=engine_vertex_indices[::-1]#修改顶点绕序
    for engine_vertex_index in kVertexIndexVectorInverse:
        f.write(struct.pack('H',engine_vertex_index))
    
    f.close()

print("----------Exporting the Mesh Successfully----------")