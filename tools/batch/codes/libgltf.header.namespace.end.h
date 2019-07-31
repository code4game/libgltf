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
        string_t text;
        size_t dimension;
    };

    const SAccessorType GSAccessorTypes[uint8_t(EAccessorType::MAX)] = {
        SAccessorType{ GLTFTEXT("")         , 0  },
        SAccessorType{ GLTFTEXT("SCALAR")   , 1  },
        SAccessorType{ GLTFTEXT("VEC2")     , 2  },
        SAccessorType{ GLTFTEXT("VEC3")     , 3  },
        SAccessorType{ GLTFTEXT("VEC4")     , 4  },
        SAccessorType{ GLTFTEXT("MAT2")     , 4  },
        SAccessorType{ GLTFTEXT("MAT3")     , 9  },
        SAccessorType{ GLTFTEXT("MAT4")     , 16 }
    };

    int32_t AccessorComponentTypeToInt32(EAccessorComponentType _eType);
    EAccessorComponentType Int32ToAccessorComponentType(int32_t _iValue);
    const string_t& AccessorTypeToText(EAccessorType _eType);
    EAccessorType TextToAccessorType(const string_t& _eText, bool _bCaseCensitive = true);
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
            for (size_t j = 0; j < dimensionof_accessor_type; ++j)\
            {\
                _Vector[i][j] = static_cast<typename TVector::TComponent>(*((TType*)bufferData.buffer + i * dimensionof_accessor_type + j));\
            }\
        }\
    } break

    /// help to operate the accessor data
    struct SAccessorData
    {
        SAccessorData();

        EAccessorComponentType componentType;
        size_t count;
        EAccessorType type;
        size_t bufferStride;
        SBufferData bufferData;

        template<typename TVector>
        bool operator>>(TVector& _Vector) const
        {
            const size_t dimensionof_accessor_type = DimensionOfAccessorType(type);
            // not allow to convert to another with the different dimension
            if (dimensionof_accessor_type != TVector::Dimension) return false;
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
                    ::memcpy(_Vector.Data(), bufferData.buffer, sizeof_accessor);
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
    template<size_t VDimension, typename TType>
    class TDimensionData
    {
    public:
        static const size_t Dimension = VDimension;

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
    template<size_t VDimension, typename TType>
    class TDimensionVector : public std::vector<TDimensionData<VDimension, TType> >
    {
    public:
        typedef std::vector<TDimensionData<VDimension, TType> >     TSuper;
        typedef typename TSuper::value_type                         TValue;
        typedef TType                                               TComponent;

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
        static std::shared_ptr<IglTFLoader> Create(const string_t& file);

    public:
        /// get the glTF structure
        virtual std::weak_ptr<struct SGlTF> glTF() = 0;

        /// load the indices data
        virtual bool GetOrLoadMeshPrimitiveIndicesData(size_t mesh_index, size_t primitive_index, std::shared_ptr<IAccessorStream> accessor_stream) = 0;

        /// load the attribute data like position, normal, texcoord, etc
        virtual bool GetOrLoadMeshPrimitiveAttributeData(size_t mesh_index, size_t primitive_index, const string_t& attribute, std::shared_ptr<IAccessorStream> accessor_stream) = 0;

        /// load the image data and type
        virtual bool GetOrLoadImageData(size_t index, std::vector<uint8_t>& data, string_t& data_type) = 0;
    };
