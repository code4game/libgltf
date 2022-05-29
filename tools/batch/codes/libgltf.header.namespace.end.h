    enum class EAccessorComponentType : uint32_t
    {
        NONE,
        BYTE,
        UNSIGNED_BYTE,
        SHORT,
        UNSIGNED_SHORT,
        INT,
        UNSIGNED_INT,
        FLOAT,
        MAX,
    };
    
    template<typename TType>
    class TComponentData
    {
    public:
        typedef TType value_type;

    public:
        operator EAccessorComponentType() const
        {
            return EAccessorComponentType::NONE;
        }
    };

#define LIBGLTF_ACCESSORCOMPONENTDATA(TType, EType)\
    template<>\
    class TComponentData<TType>\
    {\
    public:\
        operator EAccessorComponentType() const\
        {\
            return EType;\
        }\
        template<typename TTypeAnother>\
        bool operator==(TComponentData<TTypeAnother> _Another) const\
        {\
            return (EAccessorComponentType(_Another) == EType);\
        }\
    }
    LIBGLTF_ACCESSORCOMPONENTDATA(int8_t    , EAccessorComponentType::BYTE          );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint8_t   , EAccessorComponentType::UNSIGNED_BYTE );
    LIBGLTF_ACCESSORCOMPONENTDATA(int16_t   , EAccessorComponentType::SHORT         );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint16_t  , EAccessorComponentType::UNSIGNED_SHORT);
    LIBGLTF_ACCESSORCOMPONENTDATA(int32_t   , EAccessorComponentType::INT           );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint32_t  , EAccessorComponentType::UNSIGNED_INT  );
    LIBGLTF_ACCESSORCOMPONENTDATA(float     , EAccessorComponentType::FLOAT         );

    struct SAccessorComponentType
    {
        size_t value;
        size_t size;
    };

    const SAccessorComponentType GSAccessorComponentTypes[uint32_t(EAccessorComponentType::MAX)] = {
        SAccessorComponentType{ 0   , 0                },
        SAccessorComponentType{ 5120, sizeof(int8_t)   },
        SAccessorComponentType{ 5121, sizeof(uint8_t)  },
        SAccessorComponentType{ 5122, sizeof(int16_t)  },
        SAccessorComponentType{ 5123, sizeof(uint16_t) },
        SAccessorComponentType{ 5124, sizeof(int32_t)  },
        SAccessorComponentType{ 5125, sizeof(uint32_t) },
        SAccessorComponentType{ 5126, sizeof(float)    }
    };

    enum class EAccessorType : uint8_t
    {
        NONE,
        SCALAR,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4,
        MAX,
    };

    struct SAccessorType
    {
        std::string text;
        size_t dimension;
    };

    const SAccessorType GSAccessorTypes[uint8_t(EAccessorType::MAX)] = {
        SAccessorType{ ""         , 0  },
        SAccessorType{ "SCALAR"   , 1  },
        SAccessorType{ "VEC2"     , 2  },
        SAccessorType{ "VEC3"     , 3  },
        SAccessorType{ "VEC4"     , 4  },
        SAccessorType{ "MAT2"     , 4  },
        SAccessorType{ "MAT3"     , 9  },
        SAccessorType{ "MAT4"     , 16 }
    };

    int32_t AccessorComponentTypeToInt32(EAccessorComponentType _eType);
    EAccessorComponentType Int32ToAccessorComponentType(int32_t _iValue);
    const std::string& AccessorTypeToText(EAccessorType _eType);
    EAccessorType TextToAccessorType(const std::string& _eText, bool _bCaseCensitive = true);
    size_t SizeOfAccessorComponentType(EAccessorComponentType _eType);
    size_t DimensionOfAccessorType(EAccessorType _eType);
    size_t SizeOfAccessor(EAccessorComponentType _eAccessorComponentType, size_t _iCount, EAccessorType _eAccessorType);

    struct SBufferData
    {
        SBufferData();

        const uint8_t* buffer;
        size_t bufferSize;
        size_t bufferStride;
    };

#define LIBGLTF_ACCESSORCOMPONENT_CASE(TType, EType)\
    case EType: {\
        _Vector.Resize(count);\
        for (size_t i = 0; i < count; ++i)\
        {\
            for (size_t j = 0; j < dimension_of_accessor_type; ++j)\
            {\
                _Vector[i][j] = static_cast<typename TVector::TComponent>(*((TType*)bufferData.buffer + i * dimension_of_accessor_type + j));\
            }\
        }\
    } break

    /// help to operate the accessor data
    struct SAccessorData
    {
        SAccessorData();

        EAccessorComponentType componentType;
        size_t                 count;
        EAccessorType          type;
        size_t                 bufferStride;
        SBufferData            bufferData;

        template<typename TVector>
        bool operator>>(TVector& _Vector) const
        {
            const size_t dimension_of_accessor_type = DimensionOfAccessorType(type);
            // not allow to convert to another with the different dimension
            if (dimension_of_accessor_type != TVector::Dimension) return false;

            const size_t sizeof_data = SizeOfAccessor(componentType, 1, type);
            if (bufferStride != 0 && bufferStride != sizeof_data) return false;

            const size_t sizeof_accessor = sizeof_data * count;
            if (sizeof_accessor > bufferData.bufferSize) return false;

            if (count > 0)
            {
                EAccessorComponentType v_component_type = TComponentData<typename TVector::TComponent>();
                if (componentType == v_component_type)
                {
                    _Vector.Resize(count);
                    memcpy(_Vector.Data(), bufferData.buffer, sizeof_accessor);
                }
                else
                {
                    switch (componentType)
                    {
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int8_t   , EAccessorComponentType::BYTE          );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint8_t  , EAccessorComponentType::UNSIGNED_BYTE );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int16_t  , EAccessorComponentType::SHORT         );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint16_t , EAccessorComponentType::UNSIGNED_SHORT);
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int32_t  , EAccessorComponentType::INT           );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint32_t , EAccessorComponentType::UNSIGNED_INT  );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(float    , EAccessorComponentType::FLOAT         );

                    default:
                        // not support
                        return false;
                    }
                }
            }
            return true;
        }
    };

    class IAccessorStream
    {
    public:
        virtual bool operator<<(const SAccessorData& accessor_data) = 0;
    };

    /// a dimension vertex that supports to construct some vertex variables like vec2, vec3, vec4, etc
    template<size_t VDim, typename TType>
    class TVertex
    {
    public:
        static const size_t Dimension = VDim;

    public:
        const TType& operator[](size_t index) const
        {
            return m_aData[index];
        }

        TType& operator[](size_t index)
        {
            return m_aData[index];
        }

    private:
        TType m_aData[Dimension];
    };

    /// a vector, contains some vertex datas
    template<size_t VDim, typename TType>
    class TVertexList : public std::vector<TVertex<VDim, TType> >
    {
    public:
        typedef std::vector<TVertex<VDim, TType> > TSuper;
        typedef typename TSuper::value_type        TValue;
        typedef TType                              TComponent;

    public:
        static const size_t Dimension = TValue::Dimension;

    public:
        void Resize(size_t new_size)
        {
            TSuper::resize(new_size);
        }

        const TValue* Data() const
        {
            return TSuper::data();
        }

        TValue* Data()
        {
            return TSuper::data();
        }
    };

    /// help to pass the vector
    template<typename TVector>
    class TAccessorStream : public libgltf::IAccessorStream
    {
    public:
        explicit TAccessorStream(TVector& _Vector)
            : m_Vector(_Vector)
        {
            //
        }

    public:
        virtual bool operator<<(const libgltf::SAccessorData& accessor_data) override
        {
            return (accessor_data >> m_Vector);
        }

    private:
        TVector& m_Vector;
    };

    /// gltf loader
    class IglTFLoader
    {
    public:
        static std::shared_ptr<IglTFLoader> Create(std::function<std::shared_ptr<std::istream>(const std::string&)> _reader);

    public:
        /// get the glTF structure
        virtual const std::unique_ptr<SGlTF>& glTF() const = 0;

        /// load the indices data
        virtual bool LoadMeshPrimitiveIndicesData(size_t _mesh_index, size_t _primitive_index, std::shared_ptr<IAccessorStream> _accessor_stream) = 0;

        /// load the attribute data like position, normal, texcoord, etc
        virtual bool LoadMeshPrimitiveAttributeData(size_t _mesh_index, size_t _primitive_index, const std::string& _attribute, std::shared_ptr<IAccessorStream> _accessor_stream) = 0;

        /// load the image data and type
        virtual bool LoadImageData(size_t _index, std::vector<uint8_t>& _data, std::string& _type) = 0;
    };
