/**
 * @file RangedTree.cpp
 * @author Guy Marino (gmarino2048@gmail.com)
 * @brief The implementation file for the RangedTree class.
 * @version 0.1
 * @date 2021-11-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <xregex/common/RangedTree.hpp>

template <class T>
using RangedTree = xregex::common::RangedTree<T>;


template <class T>
RangedTree<T>::RangedTreeNode::RangedTreeNode(const T value):
RangedTreeNode(RangedTreeNode::VALUE, value) { }


template <class T>
RangedTree<T>::RangedTreeNode::RangedTreeNode
(
    const Type node_type,
    const std::optional<T>& value
):
_node_type(node_type),
_value(value) { }


template <class T>
RangedTree<T>::RangedTreeNode::RangedTreeNode(const RangedTreeNode& other):
_node_type(other._node_type),
_value(other._value),
_tree_height(other._tree_height),
_parent(other._parent)              // Preserve the parent. Worst case it's unreachable
{
    if( other._left_child )
    {
        _left_child = new RangedTreeNode(*other._left_child);
    }

    if( other._right_child )
    {
        _right_child = new RangedTreeNode(*other._right_child);
    }
}


template <class T>
RangedTree<T>::RangedTreeNode::RangedTreeNode(RangedTreeNode&& other) noexcept:
_node_type(other._node_type),
_value(other._value),
_tree_height(other.tree_height),
_parent(other._parent)              // Preserve the parent. Worst case it's unreachable
{
    // Make sure no self-assignment
    if( this == *other )
    {
        return;
    }

    _left_child = other._left_child;
    other._left_child = nullptr;

    _right_child = other._right_child;
    other._right_child = nullptr;
}


template struct xregex::common::RangedTree<char>;
template struct xregex::common::RangedTree<wchar_t>;
