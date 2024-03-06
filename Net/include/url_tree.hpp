// url_tree.hpp
// For GitÂne's net
#ifndef URL_TREE_HPP
#define URL_TREE_HPP

#include <iostream>
#include <map>
#include <functional>

class Node {
public:
    std::map<std::string, Node> children;
    std::function<std::string(std::string method, std::string args)> action;

    Node() : action(nullptr) {}

    void addChild(const std::string& urlPart, const Node& child);
    void addLeaf(const std::string& name, std::function<std::string(std::string method, std::string args)> leafAction);
};

class UrlTree {
public:
    Node root;

    void addPath(const std::string& path, std::function<std::string(std::string method, std::string args)> action);
    void printTreeHelper(const Node& node, const std::string& currentPath) const;
    void printTree() const;
    std::function<std::string(std::string method, std::string args)> getActionForUrl(const std::string& url) const;
};

#endif  // URL_TREE_HPP
