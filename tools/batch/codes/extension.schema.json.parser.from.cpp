if (_JsonValue.HasMember("KHR_draco_mesh_compression") && _JsonValue["KHR_draco_mesh_compression"].IsObject())
{
    std::shared_ptr<SKHR_draco_mesh_compressionextension> extension;
    if (!(extension << _JsonValue["KHR_draco_mesh_compression"])) return false;
    _rData.properties.insert(std::make_pair("KHR_draco_mesh_compression", extension));
}
if (_JsonValue.HasMember("KHR_lights_punctual") && _JsonValue["KHR_lights_punctual"].IsObject())
{
    const JSONCharValue& json_extension = _JsonValue["KHR_lights_punctual"];
    if (json_extension.HasMember("lights"))
    {
        std::shared_ptr<SKHR_lights_punctualglTFextension> extension;
        if (extension << json_extension)
        {
            _rData.properties.insert(std::make_pair("KHR_lights_punctual", extension));
        }
    }
    if (json_extension.HasMember("light"))
    {
        std::shared_ptr<SKHR_lights_punctualnodeextension> extension;
        if (extension << json_extension)
        {
            _rData.properties.insert(std::make_pair("KHR_lights_punctual", extension));
        }
    }
}
if (_JsonValue.HasMember("KHR_materials_clearcoat") && _JsonValue["KHR_materials_clearcoat"].IsObject())
{
    std::shared_ptr<SKHR_materials_clearcoatglTFextension> extension;
    if (!(extension << _JsonValue["KHR_materials_clearcoat"])) return false;
    _rData.properties.insert(std::make_pair("KHR_materials_clearcoat", extension));
}
if (_JsonValue.HasMember("KHR_materials_ior") && _JsonValue["KHR_materials_ior"].IsObject())
{
    std::shared_ptr<SKHR_materials_iorglTFextension> extension;
    if (!(extension << _JsonValue["KHR_materials_ior"])) return false;
    _rData.properties.insert(std::make_pair("KHR_materials_ior", extension));
}
if (_JsonValue.HasMember("KHR_materials_sheen") && _JsonValue["KHR_materials_sheen"].IsObject())
{
    std::shared_ptr<SKHR_materials_sheenglTFextension> extension;
    if (!(extension << _JsonValue["KHR_materials_sheen"])) return false;
    _rData.properties.insert(std::make_pair("KHR_materials_sheen", extension));
}
if (_JsonValue.HasMember("KHR_materials_specular") && _JsonValue["KHR_materials_specular"].IsObject())
{
    std::shared_ptr<SKHR_materials_specularglTFextension> extension;
    if (!(extension << _JsonValue["KHR_materials_specular"])) return false;
    _rData.properties.insert(std::make_pair("KHR_materials_specular", extension));
}
if (_JsonValue.HasMember("KHR_materials_transmission") && _JsonValue["KHR_materials_transmission"].IsObject())
{
    std::shared_ptr<SKHR_materials_transmissionglTFextension> extension;
    if (!(extension << _JsonValue["KHR_materials_transmission"])) return false;
    _rData.properties.insert(std::make_pair("KHR_materials_transmission", extension));
}
if (_JsonValue.HasMember("KHR_materials_unlit") && _JsonValue["KHR_materials_unlit"].IsObject())
{
    std::shared_ptr<SKHR_materials_unlitglTFextension> extension;
    if (!(extension << _JsonValue["KHR_materials_unlit"])) return false;
    _rData.properties.insert(std::make_pair("KHR_materials_unlit", extension));
}
if (_JsonValue.HasMember("KHR_materials_variants") && _JsonValue["KHR_materials_variants"].IsObject())
{
    std::shared_ptr<SKHR_materials_variantsglTFextension> extension;
    if (!(extension << _JsonValue["KHR_materials_variants"])) return false;
    _rData.properties.insert(std::make_pair("KHR_materials_variants", extension));
}
if (_JsonValue.HasMember("KHR_materials_volume") && _JsonValue["KHR_materials_volume"].IsObject())
{
    std::shared_ptr<SKHR_materials_volumeglTFextension> extension;
    if (!(extension << _JsonValue["KHR_materials_volume"])) return false;
    _rData.properties.insert(std::make_pair("KHR_materials_volume", extension));
}
if (_JsonValue.HasMember("KHR_texture_transform") && _JsonValue["KHR_texture_transform"].IsObject())
{
    std::shared_ptr<SKHR_texture_transformtextureInfoextension> extension;
    if (!(extension << _JsonValue["KHR_texture_transform"])) return false;
    _rData.properties.insert(std::make_pair("KHR_texture_transform", extension));
}
if (_JsonValue.HasMember("ADOBE_materials_thin_transparency") && _JsonValue["ADOBE_materials_thin_transparency"].IsObject())
{
    std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension> extension;
    if (!(extension << _JsonValue["ADOBE_materials_thin_transparency"])) return false;
    _rData.properties.insert(std::make_pair("ADOBE_materials_thin_transparency", extension));
}
if (_JsonValue.HasMember("AGI_articulations") && _JsonValue["AGI_articulations"].IsObject())
{
    std::shared_ptr<SAGI_articulationsglTFextension> extension;
    if (!(extension << _JsonValue["AGI_articulations"])) return false;
    _rData.properties.insert(std::make_pair("AGI_articulations", extension));
}
if (_JsonValue.HasMember("AGI_stk_metadata") && _JsonValue["AGI_stk_metadata"].IsObject())
{
    std::shared_ptr<SAGI_stk_metadataglTFNodeextension> extension;
    if (!(extension << _JsonValue["AGI_stk_metadata"])) return false;
    _rData.properties.insert(std::make_pair("AGI_stk_metadata", extension));
}
if (_JsonValue.HasMember("CESIUM_primitive_outline") && _JsonValue["CESIUM_primitive_outline"].IsObject())
{
    std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension> extension;
    if (!(extension << _JsonValue["CESIUM_primitive_outline"])) return false;
    _rData.properties.insert(std::make_pair("CESIUM_primitive_outline", extension));
}
if (_JsonValue.HasMember("EXT_mesh_gpu_instancing") && _JsonValue["EXT_mesh_gpu_instancing"].IsObject())
{
    std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension> extension;
    if (!(extension << _JsonValue["EXT_mesh_gpu_instancing"])) return false;
    _rData.properties.insert(std::make_pair("EXT_mesh_gpu_instancing", extension));
}
if (_JsonValue.HasMember("EXT_texture_webp") && _JsonValue["EXT_texture_webp"].IsObject())
{
    std::shared_ptr<SEXT_texture_webpglTFextension> extension;
    if (!(extension << _JsonValue["EXT_texture_webp"])) return false;
    _rData.properties.insert(std::make_pair("EXT_texture_webp", extension));
}
if (_JsonValue.HasMember("MSFT_lod") && _JsonValue["MSFT_lod"].IsObject())
{
    std::shared_ptr<SMSFT_lodglTFextension> extension;
    if (!(extension << _JsonValue["MSFT_lod"])) return false;
    _rData.properties.insert(std::make_pair("MSFT_lod", extension));
}
if (_JsonValue.HasMember("MSFT_texture_dds") && _JsonValue["MSFT_texture_dds"].IsObject())
{
    std::shared_ptr<SMSFT_texture_ddsextension> extension;
    if (!(extension << _JsonValue["MSFT_texture_dds"])) return false;
    _rData.properties.insert(std::make_pair("MSFT_texture_dds", extension));
}
_rData.schemaType = "extension.schema.json";
return true;