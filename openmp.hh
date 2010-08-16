#ifdef _OPENMP
#include <omp.h>
struct MutexType
{
    MutexType() : lock() { omp_init_lock(&lock); }
    ~MutexType() { omp_destroy_lock(&lock); }
    void Lock() { omp_set_lock(&lock); }
    void Unlock() { omp_unset_lock(&lock); }
    bool TryLock() { return omp_test_lock(&lock); }

    MutexType(const MutexType& ) : lock() { omp_init_lock(&lock); }
    MutexType& operator= (const MutexType& ) { return *this; }
public:
    omp_lock_t lock;
};
template<typename MutexType>
struct BasicScopedLock
{
    // lock
    explicit BasicScopedLock(MutexType& m) : mut(m), locked(true) { mut.Lock(); }
    // try locking
    BasicScopedLock(MutexType& m, int) : mut(m), locked(mut.TryLock()) { }
    ~BasicScopedLock() { Unlock(); }
    void Unlock() { if(!locked) return; locked=false; mut.Unlock(); }
    void LockAgain() { if(locked) return; mut.Lock(); locked=true; }
    bool IsLocked() const { return locked; }
private: // prevent copying the scoped lock.
    void operator=(const BasicScopedLock&);
    BasicScopedLock(const BasicScopedLock&);
private:
    MutexType& mut;
    bool locked;
};
typedef BasicScopedLock<MutexType> ScopedLock;
#endif
