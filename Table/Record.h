#pragma once
template<typename TKey, typename TVal>
struct Record
{
    TKey key;
    TVal val;
    Record() : key(), val() {}
    Record(const TKey& k, const TVal& v) : key(k), val(v) {}

};
