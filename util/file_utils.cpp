#include "lucene.h"
#include "lucene_thread.h"
#include "file_utils.h"

// #include <unistd.h>

namespace Lucene {
namespace FileUtils {

bool file_exists(const String& path) {
    boost::system::error_code ec;
    return boost::filesystem::exists(path.c_str(), ec);
}

uint64_t file_modified(const String& path) {
    boost::system::error_code ec;
    uint64_t t = (uint64_t)boost::filesystem::last_write_time(path.c_str(), ec);
    return ec ? 0 : t;
}

bool touch_file(const String& path) {
    boost::system::error_code ec;
    boost::filesystem::last_write_time(path.c_str(), time(NULL), ec);
    return !ec;
}

// 为什么要这么干？
int64_t file_length(const String& path) {
    boost::system::error_code ec;
    int64_t fileSize = (int64_t)boost::filesystem::file_size(path.c_str(), ec);

    for (int32_t i = 0; !ec && fileSize == 0 && i < 100; ++i) {
        LuceneThread::thread_yield();
        fileSize = (int64_t)boost::filesystem::file_size(path.c_str(), ec);
    }

    return ec ? 0 : fileSize;
}

bool set_file_length(const String& path, int64_t length) {
    if (!file_exists(path)) {
        return false;
    }

    return truncate(boost::filesystem::path(path).c_str(), (off_t)length) == 0;
}

bool remove_file(const String& path) {
    boost::system::error_code ec;
    return boost::filesystem::remove(path.c_str(), ec);
}

bool copy_file(const String& source, const String& dest) {
    boost::system::error_code ec;
    boost::filesystem::copy_file(source.c_str(), dest.c_str(),
                        boost::filesystem::copy_option::overwrite_if_exists, ec);
    return !ec;
}

bool create_directory(const String& path) {
    boost::system::error_code ec;
    return boost::filesystem::create_directory(path.c_str(), ec) && !ec;
}

bool remove_directory(const String& path) {
    boost::system::error_code ec;
    boost::filesystem::remove_all(path.c_str(), ec);
    return !ec;
}

bool is_directory(const String& path) {
    boost::system::error_code ec;
    return boost::filesystem::is_directory(path.c_str(), ec);
}

bool list_directory(const String& path, bool filesOnly, HashSet<String> dirList) {
    boost::system::error_code ec;
    boost::filesystem::directory_iterator dir(path.c_str(), ec);

    if (ec) {
        return false;
    }

    for (; dir != boost::filesystem::directory_iterator(); ++dir) {
        if (!filesOnly || !boost::filesystem::is_directory(dir->status())) {
            dirList.add(dir->path().filename().string());
        }
    }

    return true;
}

bool copy_directory(const String& source, const String& dest) {
    HashSet<String> dirList(HashSet<String>::new_instance());

    if (!list_directory(source, true, dirList)) {
        return false;
    }

    create_directory(dest);

    for (HashSet<String>::iterator file = dirList.begin(); file != dirList.end(); ++file) {
        copy_file(join_path(source, *file), join_path(dest, *file));
    }

    return true;
}

String join_path(const String& path, const String& file) {
    boost::filesystem::path join(path.c_str());
    join /= file.c_str();
    // return join.wstring().c_str();
    return join.string();
}

String extract_path(const String& path) {
    boost::filesystem::path parentPath(path.c_str());
    return parentPath.parent_path().string();
}

String extract_file(const String& path) {
    boost::filesystem::path fileName(path.c_str());
    return fileName.filename().string();
}

} // namespace FileUtils
} // namespace Lucene

