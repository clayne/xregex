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


