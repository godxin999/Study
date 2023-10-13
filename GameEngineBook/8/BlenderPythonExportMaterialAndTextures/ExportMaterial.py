import bpy
import json #导入python的json模块
import os
from shutil import copyfile #从shutil模块中导入copyfile函数，用于赋值文件

print("----------COLLECT----------")

object=bpy.context.object#获取当前的活动对象
print("Object:",object.name_full)#打印对象的名称

for materialSlot in object.material_slots:#遍历对象的所有材质插槽
    material=materialSlot.material#获取当前插槽的材质
    print("Material:",material.name_full)#打印材质的名称

    print(" ","Params:")
    print(" ","blend_method:",material.blend_method)#打印材质的混合模式
    print(" ","diffuse_color:",material.diffuse_color[0],material.diffuse_color[1],material.diffuse_color[2],material.diffuse_color[3])#打印材质的漫反射颜色
    print(" ","metallic:",material.metallic)#打印材质的金属度
    print(" ","pass_index:",material.pass_index)#打印材质的通道索引
    print(" ","line_color:",material.line_color)#打印材质的线条颜色
    print(" ","line_priority:",material.line_priority)#打印材质的线条优先级
    print(" ","roughness:",material.roughness)#打印材质的粗糙度
    print(" ","shadow_method:",material.shadow_method)#打印材质的阴影模式
    print(" ","specular_color:",material.specular_color)#打印材质的镜面反射颜色
    print(" ","specular_intensity:",material.specular_intensity)#打印材质的镜面反射强度
    print(" ","use_backface_culling:",material.use_backface_culling)#打印是否使用背面剔除
    print(" ","use_nodes:",material.use_nodes)#打印是否使用节点
    print(" ","use_screen_refraction:",material.use_screen_refraction)#打印是否使用屏幕折射
    print(" ","use_sss_translucency:",material.use_sss_translucency)#打印是否使用SSS半透明(次表面散射透明度)


    print("Nodes:")
    for node in material.node_tree.nodes:#遍历材质的所有节点
        print(" ",node,node.type)
        if node.type=="TEX_IMAGE":#如果节点类型是TEX_IMAGE(图像纹理节点)
            print(" "," ",node.image.filepath,node.image.file_format)#打印图像纹理节点的图像路径和格式

print("----------EXPORT----------")

blender_project_path = os.path.dirname(bpy.data.filepath)#获取blender工程路径

if os.path.exists(blender_project_path+"/export")==False:
    os.mkdir("export")

dir_path=blender_project_path+"/export/"+object.name_full#定义导出目录路径

if os.path.exists(dir_path)==False:
    os.mkdir(dir_path)


for materialSlot in object.material_slots:
    material=materialSlot.material
    print("Material:",material.name_full)

    export_material={#定义一个字典，用于存储导出的材质数据
        "blend_method":material.blend_method,
        "diffuse_color":"{},{},{},{}".format(material.diffuse_color[0],material.diffuse_color[1],material.diffuse_color[2],material.diffuse_color[3]),
        "metallic":material.metallic,
        "roughness":material.roughness,
        "specular_color":"{},{},{}".format(material.specular_color.r,material.specular_color.g,material.specular_color.b),
        "specular_intensity":material.specular_intensity,
        "image_paths":[]#定义一个空列表，用于存储图像路径名称
    }

    for node in material.node_tree.nodes:#遍历材质的所有节点
        if node.type=="TEX_IMAGE":
            img_path=node.image.filepath#获取图像纹理节点的图像路径
            img_path=img_path.replace("//","").replace("\\","/")#将路径中的\\替换为/
            img_name=img_path[img_path.rfind("/")+1:]
            copyfile(blender_project_path+"/"+img_path,dir_path+"/"+img_name)#复制图像文件到导出目录
            export_material["image_paths"].append(img_name)#将图像名称添加到列表中

    print(json.dumps(export_material,indent=2))#打印导出的材质数据(使用json格式)

    with open(dir_path+"/"+material.name_full+".json","w") as f:
        json.dump(export_material,f,indent=2)#将导出的材质数据写入文件

print("----------SUCCESS----------")