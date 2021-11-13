/**
 * @file RangedTree.hpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief A search tree which supports inter-element ranges.
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <cstdint>
#include <optional>

namespace xregex::common
{

/**
 * @brief This class allows for ranged lookup of elements, and will
 *        report a match if an indexed element is within that range.
 * 
 * @tparam T The type of element contained within the tree. This type 
 *           must support inequality operators `<`, `>`, and `==`.
 */
template <class T>
class RangedTree final
{
public:

    struct SingleEntry final
    {
        T value;
    };

    struct RangedEntry final
    {
        T range_start;
        T range_end;
    };


private:

    struct RangedTreeNode final
    {
    public:

        enum Type : uint8_t
        {
            VALUE,
            LESS_THAN,
            GREATER_THAN
        };

    private:

        std::optional<T> _value;
        Type _node_type;

        size_t _tree_height;

        RangedTreeNode* _parent;
        RangedTreeNode* _left_child;
        RangedTreeNode* _right_child;

    public:

        RangedTreeNode(const T &value);

        RangedTreeNode
        (
            const Type node_type,
            const std::optional<T> &value = std::nullopt
        );

        inline Type type() const noexcept { return _type; }

        inline std::optional<T> value() const noexcept { return _value; }

        inline size_t height() const noexcept { return _tree_height; }

        inline RangedTreeNode* parent() const noexcept { return _parent; }
        inline void parent(RangedTreeNode* ptr) noexcept { _parent = ptr; }

        inline RangedTreeNode* left() const noexcept { return _left_child; }
        inline void left(RangedTreeNode* ptr) noexcept { _left_child = ptr; }

        inline RangedTreeNode* right() const noexcept { return _right_child; }
        inline void right(RangedTreeNode* ptr) noexcept { _right_child = ptr; }

        void recalc_height();

        void rotate_right();

        void rotate_left();

    };

    void _insert_node(RangedTreeNode *node);

public:

    RangedTree();

    RangedTree(const RangedTree& other);
    RangedTree(RangedTree&& other) noexcept;

    bool contains(const T obj) const;

    void insert(const SingleEntry& entry);
    void insert(const RangedEntry& entry);

    RangedTree& operator=(const RangedTree& other);
    RangedTree& operator=(RangedTree&& other) noexcept;

    bool operator[](const T obj) const;

};

}