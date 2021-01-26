#include "Path.h"
#include "Logging.h"
#include "Unicode.h"
#ifdef GP_PLATFORM_WINDOWS
#   include <pathcch.h>
#endif
#include <functional>
#include <utility>
#include <vector>

namespace gameplay
{

Path::Path()
{
}

Path::Path(const char* path, size_t pathLen)
{
    if (path && pathLen)
    {
        _pathStr.assign(path, pathLen);
    }
    _sanitize_path();
}

Path::Path(const char* path)
{
    if (path)
    {
        _pathStr = path;
    }
    _sanitize_path();
}

Path::Path(std::string path) :
    _pathStr(std::move(path))
{
    _sanitize_path();
}

Path::Path(const Path& other) :
    _pathStr(other._pathStr)
{
}

Path::Path(Path&& other) noexcept :
    _pathStr(std::move(other._pathStr))
{
}

Path::~Path()
{
}

Path& Path::operator=(const Path& other)
{
    _pathStr = other._pathStr;
    return *this;
}

Path& Path::operator=(Path&& other) noexcept
{
    _pathStr = std::move(other._pathStr);
    return *this;
}

std::string Path::get_string() const
{
    return _pathStr;
}

Path::operator std::string() const
{
    return _pathStr;
}


const char* Path::c_str() const
{
    return _pathStr.c_str();
}

bool Path::operator==(const Path& other) const
{
    return _pathStr == other._pathStr;
}

bool Path::operator==(const std::string& other) const
{
    return _pathStr == other;
}

bool Path::operator==(const char* other) const
{
    if (other == nullptr)
    {
        return false;
    }
    return _pathStr == other;
}

bool Path::operator!=(const Path& other) const
{
    return !(*this == other);
}

bool Path::operator!=(const std::string& other) const
{
    return !(*this == other);
}

bool Path::operator!=(const char* other) const
{
    return !(*this == other);
}

size_t Path::len() const
{
    return _pathStr.size();
}

Path& Path::clear()
{
    _pathStr.clear();
    return *this;
}

bool Path::is_empty() const
{
    return _pathStr.empty();
}

Path Path::get_filename() const
{
    const char* filenamePtr = _get_filename_ptr();
    if (!filenamePtr)
    {
        return Path();
    }
    const size_t filenameOffset = filenamePtr - _pathStr.data();
    return Path(_pathStr.substr(filenameOffset, _pathStr.size() - filenameOffset));
}

Path Path::get_extension() const
{
    const char* extPtr = _get_extension_ptr();
    if (!extPtr)
    {
        return Path();
    }
    const size_t extOffset = extPtr - _pathStr.data();
    return Path(_pathStr.substr(extOffset, _pathStr.size() - extOffset));
}

Path Path::get_parent() const
{
    const char* parentPathEndPtr = _get_filename_ptr();
    const char* pathDataStart = _pathStr.data();
    if (parentPathEndPtr == nullptr)
    {
        parentPathEndPtr = pathDataStart + _pathStr.size();
    }
    // Cleaning up the trailing slashes;
    while (parentPathEndPtr > pathDataStart && parentPathEndPtr[-1] == FORWARD_SLASH_CHAR)
    {
        --parentPathEndPtr;
    }
    if (parentPathEndPtr == pathDataStart)
    {
        return Path();
    }
    return Path(_pathStr.substr(0, parentPathEndPtr - pathDataStart));
}

Path Path::get_stem() const
{
    const char* extPtr = _get_extension_ptr();
    if (extPtr == nullptr)
    {
        return get_filename();
    }
    const char* filenamePtr = _get_filename_ptr();
    return Path(_pathStr.substr(filenamePtr - _pathStr.data(), extPtr - filenamePtr));
}

Path Path::get_root_name() const
{
    const char* rootNameEndPtr = _get_root_name_end_ptr();
    if (!rootNameEndPtr)
    {
        return Path();
    }
    return Path(_pathStr.substr(0, rootNameEndPtr - _pathStr.data()));
}

Path Path::get_relative_part() const
{
    const char* relativePartPtr = _get_relative_part_ptr();
    if (relativePartPtr == nullptr)
    {
        return Path();
    }
    const size_t relativePartOffset = relativePartPtr - _pathStr.data();
    return Path(_pathStr.substr(relativePartOffset, _pathStr.size() - relativePartOffset));
}

Path Path::get_root_directory() const
{
    const char* rootDirectoryEndPtr = _get_root_directory_end_ptr();
    if (!rootDirectoryEndPtr)
    {
        return Path();
    }
    const char* rootNameEndPtr = _get_root_name_end_ptr();
    return Path(rootDirectoryEndPtr == rootNameEndPtr ? EMPTY_STRING : FORWARD_SLASH_STRING);
}

bool Path::has_root_directory() const noexcept
{
    return !get_root_directory().is_empty();
}

Path Path::get_root() const
{
    const char* rootDirectoryEndPtr = _get_root_directory_end_ptr();
    if (!rootDirectoryEndPtr)
    {
        return Path();
    }
    return Path(_pathStr.substr(0, rootDirectoryEndPtr - _pathStr.data()));
}

Path Path::concat(const Path& concatedPart) const
{
    if (is_empty())
    {
        return concatedPart;
    }
    if (concatedPart.is_empty())
    {
        return *this;
    }
    PathPartDesc parts[] = { { c_str(), len() },
                                { concatedPart.c_str(), concatedPart.len() } };
    return _concat(parts, GP_COUNTOF(parts));
}


Path& Path::operator/=(const Path& path)
{
    return *this = *this / path;
}


Path& Path::operator+=(const Path& path)
{
    return *this = *this + path;
}

Path& Path::replace_extension(const Path& newExtension)
{
    const char* extPtr = _get_extension_ptr();
    // check if we need to just remove the extension
    if (newExtension.is_empty())
    {
        if (extPtr)
        {
            _pathStr = _pathStr.substr(0, extPtr - _pathStr.data());
        }
        return *this;
    }
    const char* newExtensionData = newExtension.c_str();
    size_t newExtensionSize = newExtension.len();
    if (*newExtensionData == DOT_CHAR)
    {
        ++newExtensionData;
        --newExtensionSize;
    }
    size_t remainingPathSize = len();
    if (extPtr)
    {
        remainingPathSize = extPtr - _pathStr.data();
        size_t oldExtensionSize = len() - (extPtr - _pathStr.data());
        // skipping starting dot
        --oldExtensionSize;
        // checking for trying to use the same extension
        if (oldExtensionSize == newExtensionSize && ::memcmp(extPtr + 1, newExtensionData, newExtensionSize) == 0)
        {
            return *this;
        }
    }
    PathPartDesc parts[] = { { this->c_str(), remainingPathSize },
                                    { DOT_STRING, DOT_STRING_LENGTH },
                                    { newExtensionData, newExtensionSize } };
    return *this = _concat(parts, GP_COUNTOF(parts));
}

Path Path::get_absolute(const Path& root) const
{
    if (is_absolute() || root.is_empty())
    {
        return this->get_normalized();
    }
    return root.join(*this).get_normalized();
}

bool Path::is_absolute() const
{
#if GP_PLATFORM_LINUX
    return !is_empty() && _pathStr[0] == FORWARD_SLASH_CHAR;
#elif GP_PLATFORM_WINDOWS
    // drive root (D:/abc) case.
    // the only position where ':' is allowed on Windows.
    // check for separator is important, because D:temp.txt is a relative path on windows.
    const char* pathDataStart = _pathStr.data();
    const size_t pathDataLength = len();
    if (pathDataLength > 2 && pathDataStart[1] == COLON_CHAR && pathDataStart[2] == FORWARD_SLASH_CHAR)
    {
        return true;
    }
    // drive letter (D:) case
    if (pathDataLength == 2 && pathDataStart[1] == COLON_CHAR)
    {
        return true;
    }
    // extended drive letter path (ie: prefixed with "//./D:").
    if (pathDataLength > 4 && pathDataStart[0] == FORWARD_SLASH_CHAR && pathDataStart[1] == FORWARD_SLASH_CHAR &&
        pathDataStart[2] == DOT_CHAR && pathDataStart[3] == FORWARD_SLASH_CHAR)
    {
        // at least a drive name was specified.
        if (pathDataLength > 6 && pathDataStart[5] == COLON_CHAR)
        {
            // drive plus an absolute path was specified (ie: "//./d:/abc") => succeed.
            if (pathDataStart[6] == FORWARD_SLASH_CHAR)
            {
                return true;
            }
            // drive and relative path was specified (ie: "//./d:temp.txt") => fail.  We need to
            // specifically fail here because this path would also get picked up by the generic
            // special path check below and report success erroneously.
            else
            {
                return false;
            }
        }
        // requesting the full drive volume (ie: "//./d:") => report absolute to match behaviour
        // in the "d:" case above.
        if (pathDataLength == 6 && pathDataStart[5] == COLON_CHAR)
        {
            return true;
        }
    }

    // check for special paths.  this includes all windows paths that begin with "\\" (converted
    // to unix path separators for our purposes).  this class of paths includes extended path
    // names (ie: prefixed with "\\?\"), device names (ie: prefixed with "\\.\"), physical drive
    // paths (ie: prefixed with "\\.\PhysicalDrive<n>"), removeable media access (ie: "\\.\X:")
    // COM ports (ie: "\\.\COM*"), and UNC paths (ie: prefixed with "\\servername\sharename\").
    //
    // note: that it is not necessarily sufficient to get absolute vs relative based solely on
    // the "//" prefix here without needing to dig further into the specific name used and what
    // it actually represents.  for now, we'll just assume that device, drive, volume, and
    // port names will not be used here and treat it as a UNC path.  since all extended paths
    // and UNC paths must always be absolute, this should hold up at least for those.  If a
    // path for a drive, volume, or device is actually passed in here, it will still be treated
    // as though it were an absolute path.  the results of using such a path further may be
    // undefined however.
    if (pathDataLength > 2 &&
        pathDataStart[0] == FORWARD_SLASH_CHAR &&
        pathDataStart[1] == FORWARD_SLASH_CHAR &&
        pathDataStart[2] != FORWARD_SLASH_CHAR)
    {
        return true;
    }
    return false;
#endif
}

Path Path::get_normalized() const
{
    if (is_empty())
    {
        return Path();
    }
    constexpr size_t kDefaultTokenCount = 128;
    enum class NormalizePartType
    {
        SLASH,
        ROOT_NAME,
        ROOT_SLASH,
        DOT,
        DOTDOT,
        NAME,
        ERR
    };

    struct ParsedPathPartDesc : PathPartDesc
    {
        NormalizePartType type;
        ParsedPathPartDesc(const char* partData, size_t partSize, PathTokenType partType)
            : PathPartDesc{ partData, partSize }
        {
            switch (partType)
            {
            case PathTokenType::SLASH:
                type = NormalizePartType::SLASH;
                break;
            case PathTokenType::ROOT_NAME:
                type = NormalizePartType::ROOT_NAME;
                break;
            case PathTokenType::DOT:
                type = NormalizePartType::DOT;
                break;
            case PathTokenType::DOTDOT:
                type = NormalizePartType::DOTDOT;
                break;
            case PathTokenType::NAME:
                type = NormalizePartType::NAME;
                break;

            default:
                type = NormalizePartType::ERR;
                GP_LOG_ERROR("Invalid internal token state while normalizing a path.");
                GP_ASSERT(false);
                break;
            }
        }

        ParsedPathPartDesc(const char* partData, size_t partSize, NormalizePartType partType)
            : PathPartDesc{ partData, partSize }, type(partType)
        {
        }
    };

    std::vector<ParsedPathPartDesc> resultPathTokens;

    resultPathTokens.reserve(kDefaultTokenCount);

    const char* prevTokenEnd = _get_root_directory_end_ptr();
    const char* pathDataStart = _pathStr.data();
    const size_t pathDataLength = len();
    if (prevTokenEnd && prevTokenEnd > pathDataStart)
    {
        // add the root name and the root directory as different elements
        const char* possibleSlashPos = prevTokenEnd - 1;
        if (*possibleSlashPos == FORWARD_SLASH_CHAR)
        {
            if (possibleSlashPos > pathDataStart)
            {
                resultPathTokens.emplace_back(
                    pathDataStart, static_cast<size_t>(possibleSlashPos - pathDataStart), PathTokenType::ROOT_NAME);
            }
            resultPathTokens.emplace_back(FORWARD_SLASH_STRING, FORWARD_SLASH_STRING_LENGTH, NormalizePartType::ROOT_SLASH);
        }
        else
        {
            resultPathTokens.emplace_back(
                pathDataStart, static_cast<size_t>(prevTokenEnd - pathDataStart), PathTokenType::ROOT_NAME);
        }
    }
    else
    {
        prevTokenEnd = pathDataStart;
    }

    bool alreadyNormalized = true;
    const char* bufferEnd = pathDataStart + pathDataLength;
    PathTokenType curTokenType = PathTokenType::NAME;
    for (const char* curTokenEnd = _get_token_end(prevTokenEnd, bufferEnd, curTokenType); curTokenEnd != nullptr;
         prevTokenEnd = curTokenEnd, curTokenEnd = _get_token_end(prevTokenEnd, bufferEnd, curTokenType))
    {
        switch (curTokenType)
        {
        case PathTokenType::SLASH:
            if (resultPathTokens.empty() || resultPathTokens.back().type == NormalizePartType::SLASH ||
                resultPathTokens.back().type == NormalizePartType::ROOT_SLASH)
            {
                // skip if we already have a slash at the end
                alreadyNormalized = false;
                continue;
            }
            break;

        case PathTokenType::DOT:
            // skip it
            alreadyNormalized = false;
            continue;

        case PathTokenType::DOTDOT:
            if (resultPathTokens.empty())
            {
                break;
            }
            // check if the previous element is a part of the root name (even without a slash)
            // and skip dot-dot in such case
            if (resultPathTokens.back().type == NormalizePartType::ROOT_NAME ||
                resultPathTokens.back().type == NormalizePartType::ROOT_SLASH)
            {
                alreadyNormalized = false;
                continue;
            }
            if (resultPathTokens.size() > 1)
            {
                GP_ASSERT(resultPathTokens.back().type == NormalizePartType::SLASH);
                const NormalizePartType tokenTypeBeforeSlash = resultPathTokens[resultPathTokens.size() - 2].type;
                // remove <name>/<dot-dot> pattern
                if (tokenTypeBeforeSlash == NormalizePartType::NAME)
                {
                    // remove the last slash
                    resultPathTokens.pop_back();
                    // remove the last named token
                    resultPathTokens.pop_back();
                    alreadyNormalized = false;
                    // skip the addition of the dot-dot
                    continue; 
                }
            }
            break;

        case PathTokenType::NAME:
            // no special processing needed
            break;

        default:
            GP_LOG_ERROR("Invalid internal state while normalizing the path {%s}", c_str());
            GP_ASSERT(false);
            alreadyNormalized = false;
            continue;
        }
        resultPathTokens.emplace_back(prevTokenEnd, static_cast<size_t>(curTokenEnd - prevTokenEnd), curTokenType);
    }
    if (resultPathTokens.empty())
    {
        return Path(DOT_STRING);
    }
    else if (resultPathTokens.back().type == NormalizePartType::SLASH && resultPathTokens.size() > 1)
    {
        // remove the trailing slash for special cases like "./" and "../"
        const size_t indexOfTokenBeforeSlash = resultPathTokens.size() - 2;
        const NormalizePartType typeOfTokenBeforeSlash = resultPathTokens[indexOfTokenBeforeSlash].type;
        if (typeOfTokenBeforeSlash == NormalizePartType::DOT || typeOfTokenBeforeSlash == NormalizePartType::DOTDOT)
        {
            resultPathTokens.pop_back();
            alreadyNormalized = false;
        }
    }
    if (alreadyNormalized)
    {
        return *this;
    }
    std::vector<PathPartDesc> partsToJoin;
    partsToJoin.reserve(resultPathTokens.size());
    for (const auto& curTokenInfo : resultPathTokens)
    {
        partsToJoin.emplace_back(PathPartDesc{ curTokenInfo.data, curTokenInfo.size });
    }
    return _concat(partsToJoin.data(), partsToJoin.size());
}

Path& Path::normalize()
{
    return *this = get_normalized();
}

Path Path::get_relative(const Path& base) const noexcept
{
    // check if the operation is possible
    if (get_root_name() != base.get_root_name() ||
        is_absolute() != base.is_absolute() ||
        (!has_root_directory() && base.has_root_directory()))
    {
        return Path();
    }
    PathTokenType curPathTokenType = PathTokenType::ROOT_NAME;
    const char* curPathTokenEnd = _get_root_directory_end_ptr();
    const char* curPathTokenStart = curPathTokenEnd;
    const char* curPathEnd = _pathStr.data() + _pathStr.length();

    PathTokenType basePathTokenType = PathTokenType::ROOT_NAME;
    const char* basePathTokenEnd = base._get_root_directory_end_ptr();
    const char* basePathEnd = base._pathStr.data() + base._pathStr.length();

    // find the first mismatch
    for (;;)
    {
        curPathTokenStart = curPathTokenEnd;
        curPathTokenEnd = _get_token_end(curPathTokenEnd, curPathEnd, curPathTokenType);

        const char* baseTokenStart = basePathTokenEnd;
        basePathTokenEnd = _get_token_end(basePathTokenEnd, basePathEnd, basePathTokenType);

        if (!curPathTokenEnd || !basePathTokenEnd)
        {
            // check if both are null
            if (curPathTokenEnd == basePathTokenEnd)
            {
                return Path(DOT_STRING);
            }
            break;
        }
        if (curPathTokenType != basePathTokenType ||
            !std::equal(curPathTokenStart, curPathTokenEnd, baseTokenStart, basePathTokenEnd))
        {
            break;
        }
    }
    int requiredDotDotCount = 0;
    while (basePathTokenEnd)
    {
        if (basePathTokenType == PathTokenType::DOTDOT)
        {
            --requiredDotDotCount;
        }
        else if (basePathTokenType == PathTokenType::NAME)
        {
            ++requiredDotDotCount;
        }
        basePathTokenEnd = _get_token_end(basePathTokenEnd, basePathEnd, basePathTokenType);
    }
    if (requiredDotDotCount < 0)
    {
        return Path();
    }
    if (requiredDotDotCount == 0 && !curPathTokenEnd)
    {
        return Path(DOT_STRING);
    }
    const size_t leftoverCurPathSymbols = curPathTokenEnd != nullptr ? curPathEnd - curPathTokenStart : 0;
    const size_t requiredResultSize =
        (FORWARD_SLASH_STRING_LENGTH + DOTDOT_STRING_LENGTH) * requiredDotDotCount + leftoverCurPathSymbols;

    Path result;
    result._pathStr.reserve(requiredResultSize);

    if (requiredDotDotCount > 0)
    {
        result._pathStr += DOTDOT_STRING;
        --requiredDotDotCount;

        for (int i = 0; i < requiredDotDotCount; ++i)
        {
            result._pathStr += FORWARD_SLASH_CHAR;
            result._pathStr += DOTDOT_STRING;
        }
    }

    bool needsSeparator = !result._pathStr.empty();
    while (curPathTokenEnd)
    {
        if (curPathTokenType != PathTokenType::SLASH)
        {
            if (needsSeparator)
            {
                result._pathStr += FORWARD_SLASH_CHAR;
            }
            else
            {
                needsSeparator = true;
            }
            result._pathStr.append(curPathTokenStart, curPathTokenEnd - curPathTokenStart);
        }
        curPathTokenStart = curPathTokenEnd;
        curPathTokenEnd = _get_token_end(curPathTokenEnd, curPathEnd, curPathTokenType);
    }
    return result;
}

bool Path::is_relative() const
{
    return !is_absolute();
}

Path Path::join(const Path& joinedPart) const
{
    if (is_empty())
    {
        return joinedPart;
    }
    if (joinedPart.is_empty())
    {
        return *this;
    }
    const bool haveSeparator =
        _pathStr.back() == FORWARD_SLASH_CHAR || joinedPart._pathStr.front() == FORWARD_SLASH_CHAR;
    PathPartDesc parts[3] = {};
    size_t numParts = 0;
    parts[numParts++] = { c_str(), len() };
    if (!haveSeparator)
    {
        parts[numParts++] = { FORWARD_SLASH_STRING, FORWARD_SLASH_STRING_LENGTH };
    }
    parts[numParts++] = { joinedPart.c_str(), joinedPart.len() };
    return _concat(parts, numParts);
}

#if GP_PLATFORM_WINDOWS
std::wstring Path::convert_utf8_to_windows_path(const std::string& path)
{
    std::wstring pathW = Unicode::convert_utf8_to_wide(path);
    if (pathW == Unicode::UTF8_TO_WIDE_FAILURE)
    {
        return L"";
    }
    std::replace(pathW.begin(), pathW.end(), L'/', L'\\');
    bool hasPrefix = (pathW.compare(0, 4, L"\\\\?\\") == 0);
    if (pathW.size() >= MAX_PATH && !hasPrefix)
    {
        return L"\\\\?\\" + pathW;
    }
    if (pathW.size() < MAX_PATH && hasPrefix)
    {
        return pathW.substr(4, pathW.size() - 4);
    }
    return pathW;
}
    
std::string Path::convert_windows_to_utf8_path(const std::wstring& pathW)
{
    bool hasPrefix = (pathW.compare(0, 4, L"\\\\?\\") == 0);
    std::string path = Unicode::convert_wide_to_utf8(pathW.c_str() + (hasPrefix ? 4 : 0));
    if (path == Unicode::WIDE_TO_UTF8_FAILURE)
    {
        return "";
    }
    std::replace(path.begin(), path.end(), '\\', '/');
    return path;
}

std::wstring Path::get_windows_canonical_path(const std::wstring& path)
{
    wchar_t* canonical = nullptr;
    if (::PathAllocCanonicalize(path.c_str(), PATHCCH_ALLOW_LONG_PATHS, &canonical) == S_OK)
    {
        std::wstring result = canonical;
        LocalFree(canonical);
        return result;
    }
    GP_LOG_WARN("The path '%s' could not be canonicalized!", Path::convert_windows_to_utf8_path(path).c_str());
    return path;
}

std::wstring Path::get_windows_full_path(const std::wstring& path)
{
    DWORD size = ::GetFullPathNameW(path.c_str(), 0, nullptr, nullptr);
    if (size != 0)
    {
        std::wstring fullPathName;
        fullPathName.resize(size - 1);
        if (::GetFullPathNameW(path.c_str(), size, &fullPathName[0], nullptr) != 0)
        {
            return fullPathName;
        }
    }
    GP_LOG_WARN("Can't retrieve the full path of '%s'!", Path::convert_windows_to_utf8_path(path).c_str());
    return path;
}

std::wstring Path::fix_windows_path_prefixes(const std::wstring& path)
{
    bool hasPrefix = (path.compare(0, 4, L"\\\\?\\") == 0);
    if (path.size() >= MAX_PATH && !hasPrefix)
    {
        return L"\\\\?\\" + path;
    }
    if (path.size() < MAX_PATH && hasPrefix)
    {
        return path.substr(4, path.size() - 4);
    }
    return path;
}
#endif

const char* Path::_get_token_end(const char* bufferBegin, const char* bufferEnd, PathTokenType& resultType)
{
     /**
     * Gets the next path token starting at bufferStart till bufferEnd (points after the end of the
     * buffer data). On success returns pointer immediately after the token data and returns token type in the
     * resultType. On failure returns null and the value of the resultType is undetermined.
     * Note: it doesn't determine if a NAME is a ROOT_NAME. (ROOT_NAME is added to enum for convenience)
     */
    if (bufferBegin == nullptr || bufferEnd == nullptr || bufferEnd <= bufferBegin)
    {
        return nullptr;
    }
    // trying to find the next slash
    const char* tokenEnd = _find_from_start(bufferBegin, bufferEnd - bufferBegin, FORWARD_SLASH_CHAR);
    // if found a slash as the first symbol then return pointer to the data after it
    if (tokenEnd == bufferBegin)
    {
        resultType = PathTokenType::SLASH;
        return tokenEnd + 1;
    }
    // if no slash found we consider all passed data as a single token
    if (!tokenEnd)
    {
        tokenEnd = bufferEnd;
    }
    const size_t tokenSize = tokenEnd - bufferBegin;
    if (tokenSize == 1 && *bufferBegin == DOT_CHAR)
    {
        resultType = PathTokenType::DOT;
    }
    else if (tokenSize == 2 && bufferBegin[0] == DOT_CHAR && bufferBegin[1] == DOT_CHAR)
    {
        resultType = PathTokenType::DOTDOT;
    }
    else
    {
        resultType = PathTokenType::NAME;
    }
    return tokenEnd;
}

Path Path::_concat(const PathPartDesc* pathParts, size_t numParts)
{
    if (!pathParts || numParts == 0)
    {
        return Path();
    }
    size_t totalSize = 0;
    for (size_t i = 0; i < numParts; ++i)
    {
        if (pathParts[i].data)
        {
            totalSize += pathParts[i].size;
        }
    }
    if (totalSize == 0)
    {
        return Path();
    }
    std::string buffer;
    buffer.reserve(totalSize);
    for (size_t i = 0; i < numParts; ++i)
    {
        const char* partData = pathParts[i].data;
        const size_t partSize = pathParts[i].size;

        if (partData && partSize > 0)
        {
            buffer.append(partData, partSize);
        }
    }
    return Path(std::move(buffer));
}

const char* Path::_get_filename_ptr() const
{
    if (is_empty())
    {
        return nullptr;
    }
    const char* pathDataStart = _pathStr.data();
    const size_t pathDataSize = _pathStr.size();
    // find the last slash
    const char* slashPtr = _find_from_end(pathDataStart, pathDataSize, FORWARD_SLASH_CHAR);
    // oo slash == only filename
    if (!slashPtr)
    {
        return pathDataStart;
    }
    const char* filenamePtr = slashPtr + 1;
    // check that there is any data after the last slash
    if (filenamePtr >= pathDataStart + pathDataSize)
    {
        return nullptr;
    }
    return filenamePtr;
}

const char* Path::_get_extension_ptr() const
{
    const char* filenamePtr = _get_filename_ptr();
    if (filenamePtr == nullptr)
    {
        return nullptr;
    }
    const char* pathDataStart = _pathStr.data();
    const size_t pathDataSize = _pathStr.size();
    const size_t filenameOffset = filenamePtr - pathDataStart;
    const char* extStart = _find_from_end(filenamePtr, pathDataSize - filenameOffset, DOT_CHAR);
    // checking if the pointer is at the last position (i.e. filename ends with just a dot(s))
    if (extStart == pathDataStart + pathDataSize - 1)
    {
        extStart = nullptr;
    }
    return extStart != filenamePtr ? extStart : nullptr;
}

const char* Path::_get_root_name_end_ptr() const
{
    if (is_empty())
    {
        return nullptr;
    }
    const char* pathDataStart = _pathStr.data();
    const size_t pathDataSize = _pathStr.size();
    if (pathDataSize < 2)
    {
        return pathDataStart;
    }
#if GP_PLATFORM_WINDOWS
    // check if the path starts with a drive letter followed by a colon (ex: "A:/...")
    if (pathDataStart[1] == COLON_CHAR)
    {
        const char firstLetter = static_cast<char>(std::tolower(pathDataStart[0]));
        if (FIRST_LOWERCASE_LETTER <= firstLetter && firstLetter <= LAST_LOWERCASE_LETTER)
        {
            return pathDataStart + 2;
        }
    }
#endif
    // check if it's a UNC name ("//location/...")
    // note: just checking if the first 2 chars are forward slashes and the third symbol is not
    if (pathDataSize >= 3 && pathDataStart[0] == FORWARD_SLASH_CHAR && pathDataStart[1] == FORWARD_SLASH_CHAR &&
        pathDataStart[2] != FORWARD_SLASH_CHAR)
    {
        // search for a root directory
        const char* const slashPtr = _find_from_start(pathDataStart + 3, pathDataSize - 3, FORWARD_SLASH_CHAR);
        return slashPtr ? slashPtr : pathDataStart + pathDataSize;
    }
    return pathDataStart;
}

const char* Path::_get_relative_part_ptr() const
{
    const char* rootNameEndPtr = _get_root_name_end_ptr();
    // the rootNameEndPtr is always not null if the path is not empty
    if (!rootNameEndPtr)
    {
        return nullptr;
    }
    const size_t rootEndOffset = rootNameEndPtr - _pathStr.data();
    // find the pointer to the first symbol after the root name that is not a forward slash
    return _find_from_start<std::not_equal_to<char>>(
        rootNameEndPtr, _pathStr.size() - rootEndOffset, FORWARD_SLASH_CHAR);
}

const char* Path::_get_root_directory_end_ptr() const
{
    const char* rootNameEndPtr = _get_root_name_end_ptr();
    const char* relativePartPtr = _get_relative_part_ptr();
    if (relativePartPtr != rootNameEndPtr)
    {
        const char* result = rootNameEndPtr + 1;
        if (result > _pathStr.data() + _pathStr.size())
        {
            result = rootNameEndPtr;
        }
        return result;
    }
    return rootNameEndPtr;
}

void Path::_sanitize_path()
{
#if GP_PLATFORM_WINDOWS
    // change the backward slashes for Windows to forward ones
    for (auto& curChar : _pathStr)
    {
        if (curChar == BACKWARD_SLASH_CHAR)
        {
            curChar = FORWARD_SLASH_CHAR;
        }
    }
#elif GP_PLATFORM_LINUX
    // no change needed
#endif
}
}
