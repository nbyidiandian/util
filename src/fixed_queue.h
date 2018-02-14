/**
 * @file   fixed_queue.h
 * @author jianhao@taobao.com
 * @date   2013-01-28
 *
 * @brief lock-free-queue
 *
 * 
 */

/*! @addtogroup util
 * @{
 */

#ifndef UTIL_FIXED_QUEUE_H
#define UTIL_FIXED_QUEUE_H


namespace util
{


#define LF_SUCCESS 0
#define LF_NOT_INIT 1
#define LF_INIT_TWICE 2
#define LF_INVALID_ARGUMENT 3
#define LF_ALLOCATE_MEMORY_FAILED 4
#define LF_SIZE_OVERFLOW 5
#define LF_ENTRY_NOT_EXIST 6
#define ATOMIC_CAS(val, cmpv, newv) __sync_val_compare_and_swap((val), (cmpv), (newv))
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

bool is2n(int64_t input)
{
  return (((~input + 1) & input) == input);
};

template <typename T>
class FixedQueue
{
  struct ArrayItem
  {
    volatile int64_t cur_pos;
    T *volatile data;
    T *volatile prev;
  };
  public:
    FixedQueue();
    ~FixedQueue();
  public:
    int init(const int64_t max_num);
    void destroy();
  public:
    inline int push(T *ptr);
    inline int pop(T *&ptr);
    inline int64_t get_total() const;
    inline int64_t get_free() const;
  private:
    inline int64_t get_total_(const int64_t consumer, const int64_t producer) const;
    inline int64_t get_free_(const int64_t consumer, const int64_t producer) const;
  private:
    bool inited_;
    int64_t max_num_;
    int64_t num_mask_;
    ArrayItem *array_;
    volatile int64_t consumer_;
    volatile int64_t producer_;
} __attribute__ ((aligned (64)));

template <typename T>
FixedQueue<T>::FixedQueue() : inited_(false),
                              max_num_(0),
                              num_mask_(0),
                              array_(NULL),
                              consumer_(0),
                              producer_(0)
{
}

template <typename T>
FixedQueue<T>::~FixedQueue()
{
  destroy();
}

template <typename T>
int FixedQueue<T>::init(const int64_t max_num)
{
  int ret = LF_SUCCESS;
  if (inited_)
  {
    ret = LF_INIT_TWICE;
  }
  else if (0 >= max_num
          || !is2n(max_num))
  {
    ret = LF_INVALID_ARGUMENT;
  }
  else if (NULL == (array_ = (ArrayItem*)::valloc(sizeof(ArrayItem) * max_num)))
  {
    ret = LF_ALLOCATE_MEMORY_FAILED;
  }
  else
  {
    memset(array_, 0, sizeof(ArrayItem) * max_num);
    max_num_ = max_num;
    num_mask_ = max_num_ - 1;
    consumer_ = 1;
    producer_ = 1;
    inited_ = true;
  }
  return ret;
}

template <typename T>
void FixedQueue<T>::destroy()
{
  if (inited_)
  {
    ::free(array_);
    array_ = NULL;
    max_num_ = 0;
    num_mask_ = 0;
    consumer_ = 0;
    producer_ = 0;
    inited_ = false;
  }
}

template <typename T>
inline int64_t FixedQueue<T>::get_total() const
{
  return get_total_(consumer_, producer_);
}

template <typename T>
inline int64_t FixedQueue<T>::get_free() const
{
  return get_free_(consumer_, producer_);
}

template <typename T>
inline int64_t FixedQueue<T>::get_total_(const int64_t consumer, const int64_t producer) const
{
  return (producer - consumer);
}

template <typename T>
inline int64_t FixedQueue<T>::get_free_(const int64_t consumer, const int64_t producer) const
{
  return max_num_ - get_total_(consumer, producer);
}

template <typename T>
int FixedQueue<T>::push(T *ptr)
{
  int ret = LF_SUCCESS;
  if (!inited_)
  {
    ret = LF_NOT_INIT;
  }
  else
  {
    register int64_t old_pos = 0;
    register int64_t new_pos = 0;
    while (true)
    {
      old_pos = producer_;
      new_pos = old_pos;

      if (unlikely(0 >= get_free_(consumer_, old_pos)))
      {
        ret = LF_SIZE_OVERFLOW;
        break;
      }

      new_pos++;
      if (old_pos == ATOMIC_CAS(&producer_, old_pos, new_pos))
      {
        break;
      }
    }
    if (likely(LF_SUCCESS == ret))
    {
      register int64_t index = old_pos & num_mask_;
      array_[index].prev = array_[index].data;
      array_[index].data = ptr;
      array_[index].cur_pos = old_pos;
    }
  }
  return ret;
}

template <typename T>
int FixedQueue<T>::pop(T *&ptr)
{
  int ret = LF_SUCCESS;
  if (!inited_)
  {
    ret = LF_NOT_INIT;
  }
  else
  {
    register int64_t old_pos = 0;
    register int64_t new_pos = 0;
    while (true)
    {
      old_pos = consumer_;
      new_pos = old_pos;

      if (unlikely(0 >= get_total_(old_pos, producer_)))
      {
        ret = LF_ENTRY_NOT_EXIST;
        break;
      }

      new_pos += 1;
      if (old_pos == ATOMIC_CAS(&consumer_, old_pos, new_pos))
      {
        break;
      }
    }
    if (likely(LF_SUCCESS == ret))
    {
      register int64_t index = old_pos & num_mask_;
      while (true)
      {
        register int64_t cmp_ret = old_pos - array_[index].cur_pos;
        if (likely(0 == cmp_ret))
        {
          ptr = array_[index].data;
          if (likely(old_pos == array_[index].cur_pos))
          {
            break;
          }
          else
          {
            continue;
          }
        }
        else if (0 < cmp_ret)
        {
          continue;
        }
        else //(0 > cmp_ret)
        {
          ptr = array_[index].prev;
          break;
        }
      }
    }
  }
  return ret;
}

}

#endif /* UTIL_FIXED_QUEUE_H */

/*! @} */
