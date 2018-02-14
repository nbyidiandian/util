/**
 * @file   stable_set.h
 * @author jianhao@taobao.com
 * @date   2013-01-28
 *
 * @brief stable set
 *
 * a stl set iterate by insert order
 */

/*! @addtogroup util
 * @{
 */


#ifndef UTIL_STABLE_SET_H
#define UTIL_STABLE_SET_H

#include <map>
#include <list>
#include <string>

namespace util
{

/**
 * @brief 有序的set。
 *        在提供std::set的查找功能同时，还能通过插入的顺序进行遍历（迭代）。
 *        接口与std::set基本保持一致。
 *
 * @param T       需要插入的数据类型
 * @param Compare 比较函数，默认为std::less<T>
 */
template <typename T>
class StableSet
{
private :
    typedef std::list<T>                List;
    typedef std::map<T, List::iterator> Map;

    typedef StableSet<T>                self;

public :
    typedef T                                   key_type;
    typedef T                                   value_type;
    typedef Map::key_compare                    key_compare;
    typedef Map::key_compare                    value_compare;
    typedef List::iterator                      iterator;
    typedef List::const_iterator                const_iterator;
    typedef List::reverse_iterator              reverse_iterator;
    typedef List::const_reverse_iterator        const_reverse_iterator;
    typedef List::difference_type               difference_type;
    typedef List::size_type                     size_type;

public :
    explicit StableSet()
    { }

    template <class InputIterator>
    StableSet(InputIterator first, InputIterator last);

    StableSet(const StableSet &x);

    self &operator=(const self &x);

    virtual ~StableSet()
    { }

    iterator begin()
    {
        return _l.begin();
    }
    
    const_iterator begin() const
    {
        return _l.begin();
    }
    
    iterator end()
    {
        return _l.end();
    }
    
    const_iterator end() const
    {
        return _l.end();
    }

    reverse_iterator rbegin()
    {
        return _l.rbegin();
    }

    const_reverse_iterator rbegin() const
    {
        return _l.rbegin();
    }

    reverse_iterator rend()
    {
        return _l.rend();
    }

    const_reverse_iterator rend() const
    {
        return _l.rend();
    }

    bool empty() const
    {
        // 只要list里为空就认为是空的，map里只是存放了索引，不为空也没关系
        return _l.empty();
    }

    size_type size() const
    {
        return _l.size();
    }

    void swap(StableSet<T> &x);

    /**
     * @brief 插入一条新数据
     * @param v 待插入的数据
     * @return  返回两个值(pair)，first为插入数据所在位置。second标志插入操作是否成功
     *
     * insert函数只有当前StableSet内不包含待插入数据时才会执行插入操作，否则返回值的second为false
     * 插入操作时间复杂度为2*log(2,n) + 1
     */
    std::pair<iterator,bool> insert(const T &v)
    {
        Map::const_iterator it = _m.find(v);
        if (it != _m.end())
        {
            return std::pair(end(),false);
        }
        
        _l.push_back(v);
        List::iterator pos = _l.back();
        _m.insert(Map::value_type(v, pos));

        return std::pair(pos, true);
    }

    /**
     * @brief 从StableSet里删除一条数据
     * @param pos 待删除数据的位置
     *
     * 删除操作首先需要将其从保存元素顺序的链表里删除，还要将其从用于查找的索引里删除
     * 所以时间复杂度为log(2,n) + 1
     */
    void erase(iterator pos)
    {
        const key_type &k = *pos;
        _m.erase(k);
        _l.erase(pos);
    }

    void erase(const key_type &k)
    {
        Map::iterator mpos = _m.find(k);
        if (mpos == _m.end())
        {
            return;
        }
        iterator lpos = it->second;
        _m.erase(mpos);
        _l.erase(lpos);
    }

    void erase(iterator first, iterator last)
    {
        for (; first != last; ++first)
        {
            erase(first);
        }
    }

    void clear()
    {
        _m.clear();
        _l.clear();
    }

    iterator find(const key_type &k)
    {
        Map::const_iterator mpos = _m.find(k);
        iterator lpos = mpos.second;
        return lpos;
    }

    const_iterator find(const key_type &k) const
    {
        Map::const_iterator mpos = _m.find(k);
        iterator lpos = mpos.second;
        return const_iterator(lpos);
    }

private :
    Map               _m;
    List              _l;
};

} /* end of namespace util */

#endif


/*! @} */
