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
#include <utility>
#include <variant>
#include <vector>

namespace xregex::common
{

/**
 * @brief This class allows for ranged lookup of elements, and will
 *        report a match if an indexed element is within that range.
 * 
 * This class is implemented as a modified AVL tree, which should allow
 * the lookup to be as short as possible given the tree structure. It's
 * not possible to use a normal `std::map` as it does not support ranged
 * lookup/insertion during balancing.
 * 
 * @tparam T The type of element contained within the tree. This type 
 *           must support inequality operators `<`, `>`, and `==`.
 */
template <class T>
class RangedTree final
{
public:

    /**
     * @brief Structure used to insert a single element into the `RangedTree`.
     * 
     * This object can be conveniently constructed from an object instance
     * thanks to the conversion constructor.
     * 
     */
    struct SingleEntry final
    {
        /// The value to be entered into the `RangedTree`
        T value;

        /**
         * @brief Convenience constructor for implicit conversion.
         * 
         * @param value The value to be added to the tree
         */
        SingleEntry(const T value): value(value) { }
    };

    /**
     * @brief Structure used to insert a range into the `Rangedtree`.
     * 
     * This object can be constructed implicitly from a `std::pair` object
     * with two object methods.
     * 
     */
    struct RangedEntry final
    {
        /// The first element of the range to add
        T range_start;

        /// The final element of the range to add, inclusive
        T range_end;

        /**
         * @brief Convenience method to implicitly create a `RangedEntry`
         *        from a `std::pair` object.
         * 
         * @param values The `std::pair` of objects to add to the list.
         */
        RangedEntry(const std::pair<const T, const T> values):
            range_start(values.first),
            range_end(values.second)
        { }
    };

    /// Group the entries together for convenience
    typedef std::variant<SingleEntry, RangedEntry> Entry;

private:

    /**
     * @brief A single node entry in the tree, which contains the value
     *        and some metadata information to help keep the tree balanced.
     * 
     * This node either contains a value or the enumerator type is `LESS_THAN`
     * or `GREATER_THAN`. If the node does not contain a value, then it must be
     * a leaf. However, nodes with values may also be leaves so a null check must
     * be performed.
     * 
     */
    struct RangedTreeNode final
    {
    public:

        /**
         * @brief The type of the node, either a value or LT/GT placeholder.
         * 
         */
        enum Type : uint8_t
        {
            VALUE,          //!< The node contains a value
            LESS_THAN,      //!< The node is a less-than placeholder
            GREATER_THAN    //!< The node is a greater-than placeholder
        };

    private:

        /// The value of the node. Can be `std::nullopt`.
        std::optional<T> _value;

        /// The type of the node.
        Type _node_type;

        /// The current height of the tree. Adjusted on insertion
        size_t _tree_height;


        /// The parent of this node.
        RangedTreeNode* _parent;

        /// The left child of the node, less than the current node.
        RangedTreeNode* _left_child;

        /// The right child of the node, greater than the current node.
        RangedTreeNode* _right_child;

    public:

        /**
         * @brief Construct a `RangedTreeNode` given a certain value. 
         * 
         * Since other tags cannot have values, this initializes the tree with
         * the `VALUE` tag.
         * 
         * @param value The value to add to the node.
         */
        RangedTreeNode(const T value);

        /**
         * @brief Construct a `RangedTreeNode` given a certain tag.
         * 
         * If a value is provided for anything other than the `VALUE`
         * tag, then it will be ignored.
         * 
         * @param node_type The `Type` of this node.
         * @param value The value of this node.
         */
        RangedTreeNode
        (
            const Type node_type,
            const std::optional<T>& value = std::nullopt
        );


        /**
         * @brief Copy constructor.
         * 
         * @param other The other instance.
         */
        RangedTreeNode(const RangedTreeNode& other);

        /**
         * @brief Move constructor.
         * 
         * @param other The other instance.
         */
        RangedTreeNode(RangedTreeNode&& other) noexcept;

        /**
         * @brief Destructor.
         * 
         * This method will destroy this node and the child nodes.
         * 
         */
        ~RangedTreeNode();

        /**
         * @brief Gets the node type.
         * 
         * @return Type The node type.
         */
        inline Type type() const noexcept { return _node_type; }

        /**
         * @brief Gets the value.
         * 
         * @return std::optional<T> The value of the node, which may be empty.
         */
        inline std::optional<T> value() const noexcept { return _value; }

        /**
         * @brief Gets the tree height.
         * 
         * @return size_t The height of the subtree whose root is this node.
         */
        inline size_t height() const noexcept { return _tree_height; }


        /**
         * @brief Gets the parent of this node.
         * 
         * @return RangedTreeNode* The parent node.
         */
        inline RangedTreeNode* parent() const noexcept { return _parent; }

        /**
         * @brief Sets the parent.
         * 
         * @param ptr The new parent node.
         */
        inline void parent(RangedTreeNode* ptr) noexcept { _parent = ptr; }


        /**
         * @brief Gets the left child.
         * 
         * @return RangedTreeNode* The lesser child of this node.
         */
        inline RangedTreeNode* left() const noexcept { return _left_child; }

        /**
         * @brief Sets the left child.
         * 
         * @param ptr The new left child of this node.
         */
        inline void left(RangedTreeNode* ptr) noexcept { _left_child = ptr; }


        /**
         * @brief Gets the right child.
         * 
         * @return RangedTreeNode* The greater child of this node.
         */
        inline RangedTreeNode* right() const noexcept { return _right_child; }

        /**
         * @brief Sets the right child.
         * 
         * @param ptr The new right child of this node.
         */
        inline void right(RangedTreeNode* ptr) noexcept { _right_child = ptr; }


        /**
         * @brief Recalculate the height of the tree whose root is this node.
         * 
         */
        void recalc_height();

        /**
         * @brief Gets the balance factor of this node for AVL rebalancing.
         * 
         * The balance factor is the height of the right subtree minus the height
         * of the left subtree.
         * 
         * @return size_t The balance factor of this node.
         */
        size_t balance_factor() const;

        /**
         * @brief Perform a right-hand rotation of this node and its children.
         * 
         * In a right rotation, the right child becomes the parent, and the parent
         * becomes the left child.
         * 
         */
        void rotate_right();

        /**
         * @brief Perform a left-hand rotation of this node and its children
         * 
         * In a left rotation, the left child becomes the parent, and the parent
         * becomes the new right child.
         * 
         */
        void rotate_left();


        /**
         * @brief Copy assignment operator.
         * 
         * @param other The other instance.
         * @return RangedTreeNode& This instance.
         */
        RangedTreeNode& operator=(const RangedTreeNode& other);

        /**
         * @brief Move assignment operator.
         * 
         * @param other The other instance.
         * @return RangedTreeNode& This instance.
         */
        RangedTreeNode& operator=(RangedTreeNode&& other) noexcept;

    };

    /// The root of the tree
    RangedTreeNode* _root;


    /**
     * @brief Insert a single element into the tree.
     * 
     * @param entry The element to insert.
     */
    void _insert(const SingleEntry& entry);

    /**
     * @brief Insert a range into the tree.
     * 
     * @param entry The element to insert.
     */
    void _insert(const RangedEntry& entry);

public:

    /**
     * @brief Construct a new empty ranged tree.
     * 
     */
    RangedTree();

    /**
     * @brief Construct a RangedTree populated with the elements pro
     * 
     * @param elements 
     */
    RangedTree(const std::vector<Entry>& elements);


    /**
     * @brief Copy constructor.
     * 
     * @param other The other instance.
     */
    RangedTree(const RangedTree& other);

    /**
     * @brief Move constructor.
     * 
     * @param other The other instance.
     */
    RangedTree(RangedTree&& other) noexcept;

    /**
     * @brief Destructor.
     * 
     */
    ~RangedTree();


    /**
     * @brief Checks whether the object is valid within this tree.
     * 
     * @param obj The object to check.
     * @return bool Whether the object is in this tree.
     */
    bool contains(const T obj) const;


    /**
     * @brief Copy assignment operator.
     * 
     * @param other The other instance.
     * @return RangedTree& This instance.
     */
    RangedTree& operator=(const RangedTree& other);

    /**
     * @brief Move assignment operator.
     * 
     * @param other The other instance.
     * @return RangedTree& This instance.
     */
    RangedTree& operator=(RangedTree&& other) noexcept;


    /**
     * @brief Convenience operator for tree lookup.
     * 
     * @param obj The object to search for.
     * @return bool If the object is in the tree.
     */
    bool operator[](const T obj) const;

};

}