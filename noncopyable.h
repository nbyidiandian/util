/**
 * @file   noncopyable.h
 * @author jianhao@taobao.com
 * @date   2013-01-28
 *
 * @brief non-copyable interface
 *
 */

/*! @addtogroup util
 * @{
 */

#ifndef UTIL_NONCOPYABLE_H
#define UTIL_NONCOPYABLE_H

namespace util
{

/**
 * @brief non-copyable interface
 *
 * Inherite from this interface to declare the class to be non-copyable:
 * You can't copy a object of this class by either copy constructor or assignment operator
 */
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
    
private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

}

#endif /* UTIL_NONCOPYABLE_H */


/*! @} */
