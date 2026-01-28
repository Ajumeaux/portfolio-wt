#ifndef LANGCACHE_HPP
#define LANGCACHE_HPP

#include <map>
#include <string>
#include <utility>

namespace LangCache {

template <typename T, typename Builder>
const T& get(const std::string& lang, Builder&& builder)
{
    static std::map<std::string, T> cache;

    auto it = cache.find(lang);
    if (it != cache.end()) {
        return it->second;
    }

    T value = builder();
    return cache.emplace(lang, std::move(value)).first->second;
}

} // namespace LangCache

#endif // LANGCACHE_HPP