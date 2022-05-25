/*
 * This software is released under the MIT license.
 *
 * Copyright (c) 2017-2022 Alex Chi, The Code 4 Game Organization
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "file_loader.h"

#include <fstream>

namespace libgltf
{
    CPath::CPath()
        : m_sPath("")
    {
        //
    }

    CPath::CPath(const CPath& _Another)
        : m_sPath("")
    {
        SetPath(_Another);
    }

    CPath::CPath(const std::string& _sPath)
        : m_sPath("")
    {
        SetPath(_sPath);
    }

    void CPath::SetPath(const std::string& _sPath)
    {
        m_sPath = _sPath;
        for (auto& c : m_sPath)
        {
#if defined(LIBGLTF_PLATFORM_WINDOWS)
            if (c != '/') continue;
            c = '\\';
#else
            if (c != '\\') continue;
            c = '/';
#endif
        }
    }

    bool CPath::IsRelative() const
    {
#if defined(LIBGLTF_PLATFORM_WINDOWS)
        return (m_sPath.find_first_of(":") > m_sPath.size());
#else
        return (m_sPath[0] != '/');
#endif
    }

    CPath CPath::Parent() const
    {
#if defined(LIBGLTF_PLATFORM_WINDOWS)
        size_t index = m_sPath.find_last_of('\\');
#else
        size_t index = m_sPath.find_last_of('/');
#endif
        return CPath(index < m_sPath.size() ? m_sPath.substr(0, index) : "");
    }

    CPath CPath::Filename() const
    {
#if defined(LIBGLTF_PLATFORM_WINDOWS)
        size_t index = m_sPath.find_last_of('\\');
#else
        size_t index = m_sPath.find_last_of('/');
#endif
        return CPath(index < m_sPath.size() ? m_sPath.substr(index + 1, m_sPath.size() - 1) : "");
    }

    CPath::operator std::string() const
    {
        return m_sPath;
    }

    CPath& CPath::operator=(const CPath& _Another)
    {
        SetPath(std::string(_Another));
        return (*this);
    }

    CPath& CPath::operator=(const std::string& _sPath)
    {
        SetPath(_sPath);
        return (*this);
    }

    CPath CPath::operator/(const CPath& _Another) const
    {
#if defined(LIBGLTF_PLATFORM_WINDOWS)
        return CPath(m_sPath + "\\" + std::string(_Another));
#else
        return CPath(m_sPath + "/" + std::string(_Another));
#endif
    }

    bool CPath::operator<(const CPath& _Another) const
    {
        return (m_sPath < std::string(_Another));
    }

#if defined(LIBGLTF_PLATFORM_WINDOWS)
#include <direct.h>
#else
#include <unistd.h>
#endif
    std::string GetCWD()
    {
        std::string current_path;
        char current_path_temp[FILENAME_MAX];
#if defined(LIBGLTF_PLATFORM_WINDOWS)
        _getcwd(current_path_temp, sizeof(current_path_temp));
#else
        getcwd(current_path_temp, sizeof(current_path_temp));
#endif
        current_path = current_path_temp;
        return current_path;
    }

    CFileLoader::CFileLoader(const std::string& _sRootPath /*= ""*/)
        : m_RootPath(_sRootPath.empty() ? GetCWD() : _sRootPath)
        , m_FilePath()
        , m_FileDatas()
    {
        if (m_RootPath.IsRelative())
            m_RootPath = GetCWD().append("/").append(m_RootPath);
    }

    bool CFileLoader::Load(const std::string& _sFilePath)
    {
        CPath file_path(_sFilePath);
        if (file_path.IsRelative())
            file_path = m_RootPath / file_path;

        std::vector<uint8_t>& found_filedata = FindOrAdd(CPath(_sFilePath));
        if (!found_filedata.empty()) return true;
        std::ifstream file_stream(file_path, std::ios::in | std::ios::binary);
        if (!file_stream.is_open()) return false;

        file_stream.seekg(0, std::ios::end);
        size_t file_size = static_cast<size_t>(file_stream.tellg());
        if (file_size > 0)
        {
            file_stream.seekg(0, std::ios::beg);
            found_filedata.resize(file_size);
            size_t read_size = file_stream.read((char*)found_filedata.data(), file_size).gcount();
            if (read_size == -1 || read_size != file_size)
                return false;
        }
        m_FilePath.SetPath(_sFilePath);
        return true;
    }

    void CFileLoader::Release()
    {
        m_FileDatas.clear();
    }

    const std::vector<uint8_t>& CFileLoader::operator[](const std::string& file) const
    {
        return Find(CPath(file.empty() ? std::string(m_FilePath) : file.c_str()));
    }

    bool CFileLoader::ReadByte(const std::string& _sFilePath, size_t _Offset, void* _pData, size_t _Size) const
    {
        const std::vector<uint8_t>& file_data = (*this)[_sFilePath];
        if (file_data.empty()) return false;
        if (file_data.size() < (_Offset + _Size)) return false;
        ::memcpy(_pData, file_data.data() + _Offset, _Size);
        return true;
    }

    std::string CFileLoader::AsString(const std::string& _sFilePath, size_t _Offset /*= 0*/, size_t _Size /*= 0*/) const
    {
        const std::vector<uint8_t>& file_data = (*this)[_sFilePath];
        if (file_data.empty()) return std::string();
        if (_Size != 0 && file_data.size() < (_Offset + _Size)) return std::string();

        std::string file_content;
        file_content.resize(_Size == 0 ? (sizeof(uint8_t) * file_data.size() / sizeof(std::string::value_type) + 1) : (sizeof(uint8_t) * _Size / sizeof(std::string::value_type) + 1), 0);
        ::memcpy((uint8_t*)file_content.data(), _Size == 0 ? ((uint8_t*)file_data.data()) : ((uint8_t*)file_data.data() + _Offset), _Size == 0 ? (sizeof(uint8_t) * file_data.size()) : (sizeof(uint8_t) * _Size));

        return file_content;
    }

    const std::vector<uint8_t>& CFileLoader::Find(const CPath& file) const
    {
        TFileDatas::const_iterator found_filedata = m_FileDatas.find(file);
        if (found_filedata == m_FileDatas.end()) return GSDataEmpty;
        return found_filedata->second;
    }

    std::vector<uint8_t>& CFileLoader::FindOrAdd(const CPath& file)
    {
        TFileDatas::iterator found_filedata = m_FileDatas.find(file);
        if (found_filedata == m_FileDatas.end())
        {
            found_filedata = m_FileDatas.insert(std::make_pair(file, std::vector<uint8_t>())).first;
        }
        return found_filedata->second;
    }
}
