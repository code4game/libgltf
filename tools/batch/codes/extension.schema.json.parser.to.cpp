if (!_rData.doc) return false;
_JsonValue.SetObject();
for (const std::pair<std::string, std::shared_ptr<SObject>>& extensionProperty : _rData.data.properties)
{
    const std::shared_ptr<SObject>& extensionValue = extensionProperty.second;
    if (!extensionValue) return false;
    if (extensionValue->schemaType == "node.KHR_draco_mesh_compression.schema.json")
    {
        const SKHR_draco_mesh_compressionextension& extension = *((const SKHR_draco_mesh_compressionextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_draco_mesh_compressionextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_draco_mesh_compression", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "node.KHR_lights_punctual.schema.json")
    {
        const SKHR_lights_punctualnodeextension& extension = *((const SKHR_lights_punctualnodeextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_lights_punctualnodeextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_lights_punctual", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_lights_punctual.schema.json")
    {
        const SKHR_lights_punctualglTFextension& extension = *((const SKHR_lights_punctualglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_lights_punctualglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_lights_punctual", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_materials_clearcoat.schema.json")
    {
        const SKHR_materials_clearcoatglTFextension& extension = *((const SKHR_materials_clearcoatglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_materials_clearcoatglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_materials_clearcoat", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_materials_ior.schema.json")
    {
        const SKHR_materials_iorglTFextension& extension = *((const SKHR_materials_iorglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_materials_iorglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_materials_ior", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_materials_sheen.schema.json")
    {
        const SKHR_materials_sheenglTFextension& extension = *((const SKHR_materials_sheenglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_materials_sheenglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_materials_sheen", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_materials_specular.schema.json")
    {
        const SKHR_materials_specularglTFextension& extension = *((const SKHR_materials_specularglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_materials_specularglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_materials_specular", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_materials_transmission.schema.json")
    {
        const SKHR_materials_transmissionglTFextension& extension = *((const SKHR_materials_transmissionglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_materials_transmissionglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_materials_transmission", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_materials_unlit.schema.json")
    {
        const SKHR_materials_unlitglTFextension& extension = *((const SKHR_materials_unlitglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_materials_unlitglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_materials_unlit", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_materials_variants.schema.json")
    {
        const SKHR_materials_variantsglTFextension& extension = *((const SKHR_materials_variantsglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_materials_variantsglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_materials_variants", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_materials_volume.schema.json")
    {
        const SKHR_materials_volumeglTFextension& extension = *((const SKHR_materials_volumeglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_materials_volumeglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_materials_volume", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.KHR_texture_transform.schema.json")
    {
        const SKHR_texture_transformtextureInfoextension& extension = *((const SKHR_texture_transformtextureInfoextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SKHR_texture_transformtextureInfoextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("KHR_texture_transform", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.ADOBE_materials_thin_transparency.schema.json")
    {
        const SADOBE_materials_thin_transparencyglTFextension& extension = *((const SADOBE_materials_thin_transparencyglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SADOBE_materials_thin_transparencyglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("ADOBE_materials_thin_transparency", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.AGI_articulations.schema.json")
    {
        const SAGI_articulationsglTFextension& extension = *((const SAGI_articulationsglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SAGI_articulationsglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("AGI_articulations", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.AGI_stk_metadata.schema.json")
    {
        const SAGI_stk_metadataglTFextension& extension = *((const SAGI_stk_metadataglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SAGI_stk_metadataglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("AGI_stk_metadata", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.CESIUM_primitive_outline.schema.json")
    {
        const SCESIUM_primitive_outlineglTFprimitiveextension& extension = *((const SCESIUM_primitive_outlineglTFprimitiveextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("CESIUM_primitive_outline", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.EXT_mesh_gpu_instancing.schema.json")
    {
        const SEXT_mesh_gpu_instancingglTFextension& extension = *((const SEXT_mesh_gpu_instancingglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SEXT_mesh_gpu_instancingglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("EXT_mesh_gpu_instancing", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.EXT_texture_webp.schema.json")
    {
        const SEXT_texture_webpglTFextension& extension = *((const SEXT_texture_webpglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SEXT_texture_webpglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("EXT_texture_webp", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.MSFT_lod.schema.json")
    {
        const SMSFT_lodglTFextension& extension = *((const SMSFT_lodglTFextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SMSFT_lodglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("MSFT_lod", json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == "glTF.MSFT_texture_dds.schema.json")
    {
        const SMSFT_texture_ddsextension& extension = *((const SMSFT_texture_ddsextension*)extensionValue.get());
        JSONCharValue json_value;
        if (!(TDataDoc<SMSFT_texture_ddsextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember("MSFT_texture_dds", json_value, _rData.doc->GetAllocator());
    }
}
return true;