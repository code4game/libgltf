#include "structgenerator.h"

#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <map>

#include <rapidjson/schema.h>

#if defined(PLATFORM_WINDOWS)
#include <crtdbg.h>
#endif

class CSchemaDocument : public rapidjson::SchemaDocument
{
public:
    explicit CSchemaDocument(const rapidjson::SchemaDocument::ValueType& document, rapidjson::SchemaDocument::IRemoteSchemaDocumentProviderType* remoteProvider = 0, rapidjson::SchemaDocument::AllocatorType* allocator = 0)
        : GenericSchemaDocument(document, remoteProvider, allocator)
    {
        //
    }

public:
    /*rapidjson::SizeType GetPropertyCount() const
    {
        const rapidjson::internal::Schema<rapidjson::SchemaDocument>& schema_root = GetRoot();
        return schema_root.propertyCount_;
    }*/
};

typedef std::map<std::string, std::shared_ptr<CSchemaDocument>> TSchemaDocumentDict;

std::shared_ptr<CSchemaDocument> LoadSchemaDocument(const char* _pcUri, TSchemaDocumentDict& _mSchemaDocument)
{
    std::string schema_file_path = "schema/";
    schema_file_path.append(_pcUri);
    TSchemaDocumentDict::iterator found_schema_document = _mSchemaDocument.find(schema_file_path);
    if (found_schema_document != _mSchemaDocument.end())
    {
        return found_schema_document->second;
    }

    std::ifstream schema_file(schema_file_path.c_str(), std::ios::in);
    if (!schema_file.is_open())
    {
        return nullptr;
    }

    std::stringstream schema_content;
    schema_content << schema_file.rdbuf();

    rapidjson::Document document;
    if (document.Parse(schema_content.str().c_str()).HasParseError())
    {
        return nullptr;
    }

    class CRemoteSchemaDocumentProvider : public rapidjson::IRemoteSchemaDocumentProvider
    {
    public:
        explicit CRemoteSchemaDocumentProvider(TSchemaDocumentDict& _mSchemaDocument)
            : m_mSchemaDocument(_mSchemaDocument)
        {
            //
        }

    public:
        virtual const rapidjson::SchemaDocument* GetRemoteDocument(const char* _pcUri, rapidjson::SizeType _iLength)
        {
            return LoadSchemaDocument(_pcUri, m_mSchemaDocument).get();
        }

    private:
        TSchemaDocumentDict& m_mSchemaDocument;
    };

    CRemoteSchemaDocumentProvider schema_provider(_mSchemaDocument);
    std::shared_ptr<CSchemaDocument> schema_document(new CSchemaDocument(document, &schema_provider));
    _mSchemaDocument.insert(std::make_pair(schema_file_path, schema_document));
    return schema_document;
}

int32_t main(int32_t _iArgc, uint8_t* _pcArgv[])
{
#ifdef _INC_CRTDBG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    TSchemaDocumentDict schema_documents;

    std::shared_ptr<CSchemaDocument> schema_document = LoadSchemaDocument("glTF.schema.json", schema_documents);
    const CSchemaDocument::SchemaType& schema_root = schema_document->GetRoot();
    rapidjson::SchemaValidator schema_validator(*schema_document);
    //schema_validator.EndObject()
    return 0;
}
