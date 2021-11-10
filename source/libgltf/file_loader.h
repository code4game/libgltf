/*
 * This software is released under the MIT license.
 *
 * Copyright (c) 2017-2021 Alex Chi, The Code 4 Game Organization
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

#pragma once

#include "common.h"

namespace libgltf
{
    /// a simple path class
    class CPath
    {
    public:
        CPath();
        CPath(const CPath& _Another);
        explicit CPath(const string_t& _sPath);

    public:
        void SetPath(const string_t& _sPath);
        bool IsRelative() const;
        CPath Parent() const;

    public:
        operator string_t() const;
        CPath& operator=(const CPath& _Another);
        CPath& operator=(const string_t& _sPath);
        CPath operator/(const CPath& _Another) const;
        bool operator<(const CPath& _Another) const;

    private:
        string_t m_sPath;
    };

    /// a file loader, a file pool
    class CFileLoader
    {
        typedef std::map<CPath, std::vector<uint8_t>> TFileDatas;

    public:
        explicit CFileLoader(const string_t& _sRootPath = GLTFTEXT(""));

    public:
        bool Load(const string_t& _sFilePath);
        void Release();

    public:
        const std::vector<uint8_t>& operator[](const string_t& file) const;
        bool ReadByte(const string_t& _sFilePath, size_t _Offset, void* _pData, size_t _Size) const;
        string_t AsString(const string_t& file, size_t _Offset = 0, size_t _Size = 0) const;

    protected:
        const std::vector<uint8_t>& Find(const CPath& _sFilePath) const;
        std::vector<uint8_t>& FindOrAdd(const CPath& _sFilePath);

    private:
        CPath m_RootPath;
        CPath m_FilePath;
        TFileDatas m_FileDatas;
    };
}
