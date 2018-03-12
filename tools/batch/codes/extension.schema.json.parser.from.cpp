if (_JsonValue.HasMember(GLTFTEXT("KHR_draco_mesh_compression")) && _JsonValue[GLTFTEXT("KHR_draco_mesh_compression")].IsObject())
{
    std::shared_ptr<SKHR_draco_mesh_compressionextension> extension;
    if (!(extension << _JsonValue[GLTFTEXT("KHR_draco_mesh_compression")])) return false;
    _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_draco_mesh_compression"), extension));
}
if (_JsonValue.HasMember(GLTFTEXT("KHR_materials_pbrSpecularGlossiness")) && _JsonValue[GLTFTEXT("KHR_materials_pbrSpecularGlossiness")].IsObject())
{
    std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension> extension;
    if (!(extension << _JsonValue[GLTFTEXT("KHR_materials_pbrSpecularGlossiness")])) return false;
    _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_materials_pbrSpecularGlossiness"), extension));
}
_rData.schemaType = GLTFTEXT("extension.schema.json");
return true;