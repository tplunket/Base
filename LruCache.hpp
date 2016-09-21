//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include <Base/Types.hpp>

#include <map>
#include <list>

namespace NxA {

template<typename K, typename V>
class LruCache : public std::list<std::pair<K, V>> {
    using List = std::list<std::pair<K, V>>;
    std::map<K, typename List::iterator> map;
    size_t limit;

public:
#ifdef LUR_TRACK_HITS
    int hits, misses;
#endif

    using key_type = K;
    using value_type = V;

    void resizeCache(size_t nth)
    {
        List &list = *this;
        limit = nth;
        while (list.size() > limit) {
            map.erase(list.back().first);
            list.pop_back();
        }
    }

    typename List::iterator find(const K &k)
    {
        auto found = map.find(k);
        if (found == std::end(map)) {
#ifdef LUR_TRACK_HITS
            misses++;
#endif
            return std::end(*this);
        }
        auto copy = found->second->second;
        insert(k, copy);
#ifdef LUR_TRACK_HITS
        hits++;
#endif
        return std::begin(*this);
    }

    typename List::iterator insert(const K &k, const V &v)
    {
        List &list = *this;
        auto found = map.find(k);
        if (found == std::end(map)) {
            list.emplace_front(k, v);
#ifdef LUR_TRACK_HITS
            misses++;
#endif
            map.emplace(k, std::begin(list));
            if (list.size() > limit) {
                map.erase(list.back().first);
                list.pop_back();
            }
            found = map.find(k);
        }
        else {
            list.erase(found->second);
#ifdef LUR_TRACK_HITS
            hits++;
#endif
            list.emplace_front(k, v);
            found->second = std::begin(list);
        }

        return found->second;
    }

    void erase(const K &k)
    {
        List &list = *this;
        auto found = map.find(k);
        if (found != std::end(map)) {
            list.erase(found->second);
            map.erase(found);
        }
    }

    void clear()
    {
        List &list = *this;
        list.clear();
        map.clear();
    }

    V &operator[](const K &k)
    {
        auto found = find(k);
        if (found == this->end()) {
            return insert(k, V{})->second;
        }
        else {
            return found->second;
        }
    }

    const V &operator[]( const K &k ) const
    {
        return find(k)->second;
    }

    template<typename ostream>
    friend inline ostream &operator<<(ostream &os, const LruCache &self)
    {
        os << "LRU ";
#if LUR_TRACK_HITS
        os << "(hit-rate: " << double(self.hits) / double(self.hits + self.misses) << ") ";
#endif
        os << "{";
        for (auto const &in : self) {
            os << std::endl;
            os << "  " << in.first << ":";
            os << in.second << std::endl;
        }
        os << "}" << std::endl;
        return os;
    }
};
}
