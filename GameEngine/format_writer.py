import bpy

file = open("C:\\Users\\danie\\Documents\\Visual Studio 2017\\Projects\\GameEngine\\GameEngine\\models\\newformat\\test.mesh", "w")
tab_count = 0

def pad_tabs():
    global tab_count
    global file
    for tab in range(tab_count):
        file.write("\t")

def start_open_element(title, attribute_map):
    global tab_count
    global file
    pad_tabs()
    file.write("<" + title)
    tab_count += 1
    for key, value in attribute_map.items():
        if len(attribute_map) > 1:
            file.write("\n")
            pad_tabs()
        else:
            file.write(" ");
        file.write(key + "='" + value + "'")
    tab_count -= 1
    #if len(attribute_map) > 1:
    #    file.write("\n")
    #    pad_tabs()
    file.write(">")
    file.write("\n")
    tab_count += 1
    
def end_open_element(title):
    global tab_count
    tab_count -= 1
    pad_tabs()
    file.write("</" + title + ">")
    file.write("\n")
    
def write_closed_element(title, attribute_map):
    global tab_count
    global file
    pad_tabs()
    file.write("<" + title)
    tab_count += 1
    for key, value in attribute_map.items():
        if len(attribute_map) > 1:
            file.write("\n")
            pad_tabs()
        else:
            file.write(" ");
        file.write(key + "='" + value + "'")
    tab_count -= 1
    #if len(attribute_map) > 1:
    #    file.write("\n")
    #    pad_tabs()
    file.write("/>")
    file.write("\n")
    
def set_active_object(object):
    for obj in bpy.data.objects:
        obj.select = False
    bpy.context.scene.objects.active = object
    object.select = True
    
def apply_all_modifiers(object):
    set_active_object(object)
    for mod in object.modifiers:
        bpy.ops.object.modifier_apply(modifier=mod.name)
        
def copy_object(object):
    set_active_object(object)
    bpy.ops.object.duplicate()
    new_object = bpy.context.scene.objects.active
    set_active_object(new_object)
    return new_object
    
mesh_objects = []
for object in bpy.data.objects:
    if(object.type == "MESH"):
        mesh_objects.append(object)

start_open_element("Textures", {"count":str(len(bpy.data.textures))})
image_textures = [texture for texture in bpy.data.textures if ((texture.type == "IMAGE") and (texture.image != None))]
print(image_textures)
for texture in image_textures:
	write_closed_element("Texture", {
        "name":texture.name, 
        "fileName":texture.image.name
    })
end_open_element("Textures")

start_open_element("Materials", {"count":str(len(bpy.data.materials))})
for material in bpy.data.materials:
    material_texture_slots = [texture_slot for texture_slot in material.texture_slots if texture_slot != None]
    material_textures = [texture_slot.texture for texture_slot in material_texture_slots]
    start_open_element("Material", {
    "name":material.name, 
    "hasTextures":str(len(material_textures) > 0 ).lower(),
    "alpha":str(material.alpha),
    "useLighting":str(not(material.use_shadeless)).lower(),
    "blendType":"none"
    })
    write_closed_element("AmbientColor", {"values":(" ".join([str(n * material.ambient) for n in bpy.data.worlds[0].ambient_color]))})
    write_closed_element("DiffuseColor", {"values":(" ".join([str(n * material.diffuse_intensity) for n in material.diffuse_color]))})
    write_closed_element("SpecularColor", {
        "hardness":str(material.specular_hardness),
        "values":" ".join([str(n * material.specular_intensity) for n in material.specular_color])
    })
    write_closed_element("EmissionColor", {"values":(" ".join([str(n * material.emit) for n in material.diffuse_color]))})
    
    if(len(material_textures) > 0):
        start_open_element("TextureIndices", {"count":str(len(material_textures))})
        pad_tabs()
        file.write(" ".join([str(image_textures.index(texture)) for texture in material_textures]))
        file.write("\n")
        end_open_element("TextureIndices")
        
        start_open_element("TextureAmbientColorFactors", {})
        pad_tabs()
        file.write(" ".join([str(texture_slot.ambient_factor if texture_slot.use_map_ambient else 0.0) for texture_slot in material_texture_slots]))
        file.write("\n")
        end_open_element("TextureAmbientColorFactors")
        
        start_open_element("TextureDiffuseColorFactors", {})
        pad_tabs()
        file.write(" ".join([str(texture_slot.diffuse_color_factor if texture_slot.use_map_color_diffuse else 0.0) for texture_slot in material_texture_slots]))
        file.write("\n")
        end_open_element("TextureDiffuseColorFactors")
        
        start_open_element("TextureSpecularColorFactors", {})
        pad_tabs()
        file.write(" ".join([str(texture_slot.specular_color_factor if texture_slot.use_map_color_spec else 0.0) for texture_slot in material_texture_slots]))
        file.write("\n")
        end_open_element("TextureSpecularColorFactors")
        
        start_open_element("TextureSpecularHardnessFactors", {})
        pad_tabs()
        file.write(" ".join([str(texture_slot.hardness_factor if texture_slot.use_map_hardness else 0.0) for texture_slot in material_texture_slots]))
        file.write("\n")
        end_open_element("TextureSpecularHardnessFactors")
        
        start_open_element("TextureEmissionColorFactors", {})
        pad_tabs()
        file.write(" ".join([str(texture_slot.emission_factor if texture_slot.use_map_emission else 0.0) for texture_slot in material_texture_slots]))
        file.write("\n")
        end_open_element("TextureEmissionColorFactors")
        
        start_open_element("TextureIntensityOnlyFactors", {})
        pad_tabs()
        file.write(" ".join([str(texture_slot.diffuse_factor if texture_slot.use_map_diffuse else 0.0) for texture_slot in material_texture_slots]))
        file.write("\n")
        end_open_element("TextureIntensityOnlyFactors")
        
        start_open_element("TextureAlphaOnlyFactors", {})
        pad_tabs()
        file.write(" ".join([str(texture_slot.alpha_factor if texture_slot.use_map_alpha else 0.0) for texture_slot in material_texture_slots]))
        file.write("\n")
        end_open_element("TextureAlphaOnlyFactors")
        
        start_open_element("TextureNormalFactors", {})
        pad_tabs()
        file.write(" ".join([str(texture_slot.normal_factor if texture_slot.use_map_normal else 0.0) for texture_slot in material_texture_slots]))
        file.write("\n")
        end_open_element("TextureNormalFactors")
    
    end_open_element("Material")
end_open_element("Materials")

start_open_element("Meshes", {})
for mesh in mesh_objects:
    new_mesh = copy_object(mesh)
    
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.object.modifier_add(type="TRIANGULATE")
    bpy.ops.object.mode_set(mode="OBJECT")
    apply_all_modifiers(new_mesh)
    
    normals = []
    for vertex in new_mesh.data.vertices:
        normals.append(vertex.normal)
    
    start_open_element("MeshObject", {
        "name":mesh.name, 
        "doubleSided":str(new_mesh.data.show_double_sided).lower(), 
        "hasUVs":str(new_mesh.data.uv_layers.active != None).lower()
    })
    start_open_element("Faces", {"count":str(len(new_mesh.data.polygons))})
    start_open_element("VertexIndices", {})
    pad_tabs()
    for face in new_mesh.data.polygons:
        for vertex in face.vertices:
            file.write(str(vertex) + " ")
    file.write("\n")
    end_open_element("VertexIndices")

    start_open_element("NormalIndices", {})
    pad_tabs()
    for face in new_mesh.data.polygons:
        if(face.use_smooth):
            for vertex in face.vertices:
                file.write(str(vertex) + " ")
        else:
            normals.append(face.normal)
            for n in range(3):
                file.write(str(len(normals) - 1) + " ")
    file.write("\n")
    end_open_element("NormalIndices")
    
    uvIndices = {} # vertex index : uv index
    
    if new_mesh.data.uv_layers.active != None:
        start_open_element("UVIndices", {})
        pad_tabs()
        for face in new_mesh.data.polygons:
            for index in face.loop_indices: 
                if new_mesh.data.loops[index].vertex_index not in uvIndices:
                    uvIndices[new_mesh.data.loops[index].vertex_index] = index
        for key in uvIndices:
            file.write(str(key) + " ")
        file.write("\n")   
        end_open_element("UVIndices")
    
    start_open_element("MaterialIndices", {})
    pad_tabs()
    for face in new_mesh.data.polygons:
        file.write(str(face.material_index) + " ")
    file.write("\n")
    end_open_element("MaterialIndices")
    end_open_element("Faces")
    
    start_open_element("Vertices", {"count":str(len(new_mesh.data.vertices))})
    start_open_element("VertexPositions", {})
    pad_tabs()
    for vertex in new_mesh.data.vertices:
        for coordinate in vertex.co:
            file.write("{:.9}".format(str(coordinate)) + " ")
    file.write("\n")
    end_open_element("VertexPositions")
    
    start_open_element("VertexNormals", {})
    pad_tabs()
    for vertex in new_mesh.data.vertices:
        for coordinate in vertex.normal:
            file.write("{:.9}".format(str(coordinate)) + " ")
    file.write("\n")
    end_open_element("VertexNormals")
    
    if new_mesh.data.uv_layers.active != None:
        start_open_element("VertexUVs", {})
        pad_tabs()
        for index in uvIndices.values():
            for coordinate in new_mesh.data.uv_layers.active.data[index].uv: 
                file.write("{:.9}".format(str(coordinate)) + " ")
        file.write("\n")
        end_open_element("VertexUVs")
    end_open_element("Vertices")
    
    end_open_element("MeshObject")
    set_active_object(new_mesh)
    bpy.ops.object.delete()
end_open_element("Meshes")


file.close()
