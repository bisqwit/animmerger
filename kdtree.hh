#ifndef bqtKdTreeHH
#define bqtKdTreeHH

#include <utility> // for std::pair
#include <limits>  // for std::numeric_limits
#include "alloc/FSBAllocator.hh"

/* kd-tree implementation translated to C++
 * from java implementation in VideoMosaic
 * at http://www.intelegance.net/video/videomosaic.shtml.
 */

template<typename V, unsigned K = 3, typename CoordType = double>
class KDTree
{
public:
    struct KDPoint
    {
        CoordType coord[K];

        template<typename... Args>
        KDPoint(Args&&... args)
        {
            static_assert(sizeof...(args) == K || sizeof...(args) == 0u, "Incorrect number of parameters");
            Assign<0u>( std::forward<Args>(args)... );
        }

        template<unsigned index, typename... Args>
        inline void Assign(CoordType c, Args&&... args)
        {
            coord[index] = c;
            Assign<index+1>( std::forward<Args>(args)... );
        }
        template<unsigned index>
        static inline void Assign() {}

        KDPoint(CoordType v[K])
        {
            for(unsigned n=0; n<K; ++n)
                coord[n] = v[n];
        }

        bool operator==(const KDPoint& b) const
        {
            for(unsigned n=0; n<K; ++n)
                if(coord[n] != b.coord[n]) return false;
            return true;
        }
        CoordType sqrdist(const KDPoint& b) const
        {
            CoordType result = 0;
            for(unsigned n=0; n<K; ++n)
                { CoordType diff = coord[n] - b.coord[n];
                  result += diff*diff; }
            return result;
        }
    };
private:
    struct KDRect
    {
        KDPoint min, max;

        KDPoint bound(const KDPoint& t) const
        {
            KDPoint p;
            for(unsigned i=0; i<K; ++i)
                if(t.coord[i] <= min.coord[i])
                    p.coord[i] = min.coord[i];
                else if(t.coord[i] >= max.coord[i])
                    p.coord[i] = max.coord[i];
                else
                    p.coord[i] = t.coord[i];
            return p;
        }
        void MakeInfinite()
        {
            for(unsigned i=0; i<K; ++i)
            {
                min.coord[i] = std::numeric_limits<CoordType>::min();
                max.coord[i] = std::numeric_limits<CoordType>::max();
            }
        }
    };

    struct KDNode;

    static KDNode*  NewNode(const KDNode&);
    static KDNode*  NewNode(const KDPoint& k, const V& v);
    static void DisposeNode(KDNode*);

    struct KDNode
    {
        KDPoint k;
        V       v;
        KDNode  *left, *right;
    public:
        KDNode() : k(),v(),left(nullptr),right(nullptr) { }
        KDNode(const KDPoint& kk, const V& vv)
            : k(kk), v(vv), left(nullptr), right(nullptr) { }

        virtual ~KDNode() { DisposeNode(left); DisposeNode(right); }

        static KDNode* ins( const KDPoint& key, const V& val,
                            KDNode*& t,
                            int lev)
        {
            if(!t)
                return (t = NewNode(key, val));
            else if(key == t->k)
                return nullptr; /* key duplicate */
            else if(key.coord[lev] > t->k.coord[lev])
                return ins(key, val, t->right, (lev+1)%K);
            else
                return ins(key, val, t->left,  (lev+1)%K);
        }
        /*static KDNode* srch( const KDPoint& key, const KDNode* t)
        {
            for(int lev=0; t; lev=(lev+1)%K)
            {
                if(key == t->k)
                    return t;
                else if(key.coord[lev] > t->k.coord[lev])
                    t = t->right;
                else
                    t = t->left;
            }
            return 0;
        }
        static void rsearch( const KDPoint& lowk, const KDPoint& uppk,
                             const KDNode* t, int lev,
                             std::vector<KDNode*>& v)
        {
            if(!t) return;
            if(lowk.coord[lev] <= t->k.coord[lev])
                rsearch(lowk, uppk, t->left, (lev+1)%K, v);
            int j;
            for(j=0; j<K && lowk.coord[j] <= t->k.coord[j]
                         && uppk.coord[j] >= t->k.coord[j]; ++j)
            if(j == K) v.push_back(t);
            if(uppk.coord[lev] > t->k.coord[lev])
                rsearch(lowk, uppk, t->right, (lev+1)%K, v);
        }*/

        struct Nearest
        {
            const KDNode* kd;
            CoordType     dist_sqd;
        };
        // Method Nearest Neighbor from Andrew Moore's thesis. Numbered
        // comments are direct quotes from there. Step "SDL" is added to
        // make the algorithm work correctly.
        template<unsigned lev>
        static void nnbr(const KDPoint& target, const KDNode* kd,
                         KDRect&& hr, // in-param and temporary; not an out-param.
                         Nearest& nearest)
        {
            // 1. if kd is empty then set dist-sqd to infinity and exit.
            if (!kd) return;

            // 2. s := split field of kd
            //const int s = lev % K;

            // 3. pivot := dom-elt field of kd
            const KDPoint& pivot = kd->k;
            CoordType pivot_to_target = pivot.sqrdist(target);

            // 5. target-in-left := target_s <= pivot_s
            const bool target_in_left = target.coord[lev] < pivot.coord[lev];
            // 6. if target-in-left     then nearer is left, further is right
            // 7. if not target-in-left then nearer is right, further is left

            // 4. Cut hr into to sub-hyperrectangles left-hr and right-hr.
            //    The cut plane is through pivot and perpendicular to the s
            //    dimension.
            KDRect& farther_hr = hr; // optimize by not cloning
            {KDRect nearer_hr  = hr;
            (target_in_left ? nearer_hr : farther_hr).max.coord[lev] = pivot.coord[lev];
            (target_in_left ? farther_hr : nearer_hr).min.coord[lev] = pivot.coord[lev];

            // 8. Recursively call Nearest Neighbor with parameters
            //    (nearer-kd, target, nearer-hr, max-dist-sqd), storing the
            //    results in nearest and dist-sqd
            nnbr<(lev+1)%K>(target,
                            target_in_left ? kd->left : kd->right,
                            std::move(nearer_hr),
                            nearest);} // end scope for nearer_hr

            // 10. A nearer point could only lie in further-kd if there were some
            //     part of further-hr within distance sqrt(max-dist-sqd) of
            //     target.  If this is the case then
            const KDPoint closest = farther_hr.bound(target);
            if (closest.sqrdist(target) < nearest.dist_sqd)
            {
                // 10.1 if (pivot-target)^2 < dist-sqd then
                if (pivot_to_target < nearest.dist_sqd)
                {
                    // 10.1.1 nearest := (pivot, range-elt field of kd)
                    nearest.kd = kd;
                    // 10.1.2 dist-sqd = (pivot-target)^2
                    nearest.dist_sqd = pivot_to_target;
                }

                // 10.2 Recursively call Nearest Neighbor with parameters
                //      (further-kd, target, further-hr, max-dist_sqd)
                nnbr<(lev+1)%K>(target,
                                target_in_left ? kd->right : kd->left,
                                std::move(farther_hr),
                                nearest);
            }
            // SDL: otherwise, current point is nearest
            else if (pivot_to_target < nearest.dist_sqd)
            {
                nearest.kd       = kd;
                nearest.dist_sqd = pivot_to_target;
            }
        }
    private:
        void operator=(const KDNode&);
    public:
        KDNode(const KDNode& b)
            : k(b.k), v(b.v),
              left( b.left ? NewNode(*b.left) : nullptr),
              right( b.right ? NewNode(*b.right) : nullptr ) { }
    };
private:
    KDNode*     m_root;
    std::size_t m_count;
public:
    KDTree() : m_root(nullptr), m_count(0) { }
    virtual ~KDTree() { DisposeNode(m_root); }

    void insert(const KDPoint& key, const V& val)
    {
        if(KDNode::ins(key, val, m_root, 0))
            m_count += 1;
    }

    /*const V* search(const KDPoint& key) const
    {
        const KDNode* kd = KDNode::srch(key, m_root);
        return kd ? &(kd->v) : 0;
    }*/

    const V* nearest(const KDPoint& key) const
    {
        KDRect hr;
        hr.MakeInfinite();

        typename KDNode::Nearest nn;
        nn.kd       = 0;
        nn.dist_sqd = std::numeric_limits<CoordType>::max();
        KDNode::template nnbr<0>(key, m_root, std::move(hr), nn);
        if(nn.kd)
            return & nn.kd->v;
        return nullptr;
    }

    const std::pair<V,CoordType> nearest_info(const KDPoint& key) const
    {
        KDRect hr;
        hr.MakeInfinite();

        typename KDNode::Nearest nn;
        nn.kd       = 0;
        nn.dist_sqd = std::numeric_limits<CoordType>::max();
        KDNode::template nnbr<0>(key, m_root, std::move(hr), nn);
        if(nn.kd)
            return std::pair<V,CoordType> ( nn.kd->v, nn.dist_sqd);
        return std::pair<V,CoordType> ( V(), std::numeric_limits<CoordType>::max() );
    }

    std::size_t size() const { return m_count; }
public:
    KDTree& operator=(const KDTree&b)
    {
        if(this != &b)
        {
            if(m_root) DisposeNode(m_root);
            m_root = b.m_root ? NewNode(*b.m_root) : nullptr;
            m_count = b.m_count;
        }
        return *this;
    }
    KDTree(const KDTree& b)
        : m_root( b.m_root ? NewNode(*b.m_root) : nullptr ),
          m_count( b.m_count ) { }
};

template<typename V, unsigned K, typename D>
typename KDTree<V,K,D>::KDNode* KDTree<V,K,D>::NewNode(const KDNode& b)
{
    KDNode* result = FSBAllocator<KDNode>().allocate(1);
    new ((void*)result) KDNode(b);
    return result;
}

template<typename V, unsigned K, typename D>
typename KDTree<V,K,D>::KDNode* KDTree<V,K,D>::NewNode(const KDPoint& k, const V& v)
{
    KDNode* result = FSBAllocator<KDNode>().allocate(1);
    new ((void*)result) KDNode(k,v);
    return result;
}

template<typename V, unsigned K, typename D>
void KDTree<V,K,D>::DisposeNode(KDNode* p)
{
    if(!p) return;
    p->~KDNode();
    FSBAllocator<KDNode>().deallocate(p, 1);
}

#endif
