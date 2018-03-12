if (!_rData || !g_json_doc_ptr) return false;
_JsonValue.SetObject();
for (const std::pair<GLTFString, std::shared_ptr<SObject>>& extensionProperty : _rData.properties)
{
    const std::shared_ptr<SObject>& extensionValue = extensionProperty.second;
    if (!extensionValue) return false;
    if (extensionValue->schemaType == GLTFTEXT("node.KHR_draco_mesh_compression.schema.json"))
    {
        const SKHR_draco_mesh_compressionextension& extension = *((const SKHR_draco_mesh_compressionextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(extension >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_draco_mesh_compression"), json_value, g_json_doc_ptr->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_materials_pbrSpecularGlossiness.schema.json"))
    {
        const SKHR_materials_pbrSpecularGlossinessglTFextension& extension = *((const SKHR_materials_pbrSpecularGlossinessglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(extension >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_materials_pbrSpecularGlossiness"), json_value, g_json_doc_ptr->GetAllocator());
    }
}
return true;