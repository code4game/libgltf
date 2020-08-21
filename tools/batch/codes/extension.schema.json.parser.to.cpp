if (!_rData.doc) return false;
_JsonValue.SetObject();
for (const std::pair<string_t, std::shared_ptr<SObject>>& extensionProperty : _rData.data.properties)
{
    const std::shared_ptr<SObject>& extensionValue = extensionProperty.second;
    if (!extensionValue) return false;
    if (extensionValue->schemaType == GLTFTEXT("node.KHR_draco_mesh_compression.schema.json"))
    {
        const SKHR_draco_mesh_compressionextension& extension = *((const SKHR_draco_mesh_compressionextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SKHR_draco_mesh_compressionextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_draco_mesh_compression"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_lights_punctual.schema.json"))
    {
        const SKHR_lights_punctualnodeextension& extension = *((const SKHR_lights_punctualnodeextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SKHR_lights_punctualnodeextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_lights_punctual"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_materials_clearcoat.schema.json"))
    {
        const SKHR_materials_clearcoatglTFextension& extension = *((const SKHR_materials_clearcoatglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SKHR_materials_clearcoatglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_materials_clearcoat"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_materials_pbrSpecularGlossiness.schema.json"))
    {
        const SKHR_materials_pbrSpecularGlossinessglTFextension& extension = *((const SKHR_materials_pbrSpecularGlossinessglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SKHR_materials_pbrSpecularGlossinessglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_materials_pbrSpecularGlossiness"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_materials_unlit.schema.json"))
    {
        const SKHR_materials_unlitglTFextension& extension = *((const SKHR_materials_unlitglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SKHR_materials_unlitglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_materials_unlit"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_techniques_webgl.schema.json"))
    {
        const SKHR_techniques_webglmaterialextension& extension = *((const SKHR_techniques_webglmaterialextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SKHR_techniques_webglmaterialextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_techniques_webgl"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_texture_transform.schema.json"))
    {
        const SKHR_texture_transformtextureInfoextension& extension = *((const SKHR_texture_transformtextureInfoextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SKHR_texture_transformtextureInfoextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("KHR_texture_transform"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.ADOBE_materials_thin_transparency.schema.json"))
    {
        const SADOBE_materials_thin_transparencyglTFextension& extension = *((const SADOBE_materials_thin_transparencyglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SADOBE_materials_thin_transparencyglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("ADOBE_materials_thin_transparency"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.AGI_articulations.schema.json"))
    {
        const SAGI_articulationsglTFextension& extension = *((const SAGI_articulationsglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SAGI_articulationsglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("AGI_articulations"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.AGI_stk_metadata.schema.json"))
    {
        const SAGI_stk_metadataglTFextension& extension = *((const SAGI_stk_metadataglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SAGI_stk_metadataglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("AGI_stk_metadata"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.CESIUM_primitive_outline.schema.json"))
    {
        const SCESIUM_primitive_outlineglTFprimitiveextension& extension = *((const SCESIUM_primitive_outlineglTFprimitiveextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("CESIUM_primitive_outline"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.EXT_mesh_gpu_instancing.schema.json"))
    {
        const SEXT_mesh_gpu_instancingglTFextension& extension = *((const SEXT_mesh_gpu_instancingglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SEXT_mesh_gpu_instancingglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("EXT_mesh_gpu_instancing"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.EXT_texture_webp.schema.json"))
    {
        const SEXT_texture_webpglTFextension& extension = *((const SEXT_texture_webpglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SEXT_texture_webpglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("EXT_texture_webp"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.MSFT_lod.schema.json"))
    {
        const SMSFT_lodglTFextension& extension = *((const SMSFT_lodglTFextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SMSFT_lodglTFextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("MSFT_lod"), json_value, _rData.doc->GetAllocator());
    }
    else if (extensionValue->schemaType == GLTFTEXT("glTF.MSFT_texture_dds.schema.json"))
    {
        const SMSFT_texture_ddsextension& extension = *((const SMSFT_texture_ddsextension*)extensionValue.get());
        GLTFCharValue json_value;
        if (!(TDataDoc<SMSFT_texture_ddsextension>(extension, _rData.doc) >> json_value)) return false;
        _JsonValue.AddMember(GLTFTEXT("MSFT_texture_dds"), json_value, _rData.doc->GetAllocator());
    }
}
return true;